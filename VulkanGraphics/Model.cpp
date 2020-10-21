#include "Model.h"
#include <iostream>
#include <glm\ext\matrix_transform.hpp>
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Model::Model()
{
}

Model::Model(const std::string& FilePath)
{
	LoadModel(FilePath);
}

Model::~Model()
{

}

void Model::LoadModel(const std::string& FilePath)
{
	Assimp::Importer ModelImporter;

	const aiScene* Scene = ModelImporter.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		std::cout << "Error loading model: " << ModelImporter.GetErrorString() << std::endl;
		return;
	}

	GlobalInverseTransformMatrix = AssimpToGLMMatrixConverter(Scene->mRootNode->mTransformation.Inverse());

	ProcessNode(Scene->mRootNode, Scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		LoadVertices(mesh);
		LoadIndices(mesh);
		LoadBones(scene->mRootNode, mesh, VertexList, scene);
		LoadAnimations(scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Model::LoadVertices(aiMesh* mesh)
{
	for (int x = 0; x < mesh->mNumVertices; x++)
	{
		Vertex vertex;
		vertex.Position = glm::vec3{ mesh->mVertices[x].x, mesh->mVertices[x].y, mesh->mVertices[x].z };
		vertex.Normal = glm::vec3{ mesh->mNormals[x].x, mesh->mNormals[x].y, mesh->mNormals[x].z };

		if (mesh->mTangents)
		{
			vertex.Tangant = glm::vec3{ mesh->mTangents[x].x, mesh->mTangents[x].y, mesh->mTangents[x].z };
			vertex.BiTangant = glm::vec3{ mesh->mBitangents[x].x, mesh->mBitangents[x].y, mesh->mBitangents[x].z };
		}
		else
		{
			vertex.Tangant = glm::vec3{ 0.0f, 0.0f, 0.0f };
			vertex.BiTangant = glm::vec3{ 0.0f, 0.0f, 0.0f };
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.TexureCoord = glm::vec2{ mesh->mTextureCoords[0][x].x, mesh->mTextureCoords[0][x].y };
		}
		else
		{
			vertex.TexureCoord = glm::vec2{ 0.0f, 0.0f };
		}

		vertex.BoneID = glm::vec4(0.0f);
		vertex.BoneWeights = glm::vec4(0.0f);

		VertexList.emplace_back(vertex);
	}
}

void Model::LoadIndices(aiMesh* mesh)
{
	for (int x = 0; x < mesh->mNumFaces; x++)
	{
		aiFace Faces = mesh->mFaces[x];
		for (int y = 0; y < Faces.mNumIndices; y++)
		{
			IndexList.emplace_back(Faces.mIndices[y]);
		}
	}
}

void Model::LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex>& VertexList, const aiScene* scene)
{
	for (int x = 0; x < mesh->mNumBones; x++)
	{
		auto node = RootNode->FindNode(mesh->mBones[x]->mName.data);
		auto nodeTransform = AssimpToGLMMatrixConverter(node->mTransformation);
		BoneList.emplace_back(std::make_shared<Bone>(mesh->mBones[x]->mName.data, x, nodeTransform, AssimpToGLMMatrixConverter(mesh->mBones[x]->mOffsetMatrix)));
	}

	for (int x = 0; x < mesh->mNumBones; x++)
	{
		std::vector<unsigned int> AffectedVertices;
		AffectedVertices.resize(VertexList.size(), 0);

		aiBone* bone = mesh->mBones[x];
		for (unsigned int j = 0; j < mesh->mBones[x]->mNumWeights; j++)
		{
			unsigned int vertex_id = mesh->mBones[x]->mWeights[j].mVertexId;
			float weight = mesh->mBones[x]->mWeights[j].mWeight;
			VertexList[vertex_id].addBoneData(x, weight);
		}
	}

	for (int x = 0; x < VertexList.size(); x++)
	{
		float Weight = VertexList[x].BoneWeights.x +
			VertexList[x].BoneWeights.y +
			VertexList[x].BoneWeights.z +
			VertexList[x].BoneWeights.w;
		if (Weight != 1.0f)
		{
			VertexList[x].BoneWeights = glm::vec4(
			VertexList[x].BoneWeights.x / Weight,
			VertexList[x].BoneWeights.y / Weight,
			VertexList[x].BoneWeights.z / Weight,
			VertexList[x].BoneWeights.w / Weight);
		}
	}

	UpdateSkeleton(RootNode, glm::mat4(1.0f), scene);
}

void Vertex::addBoneData(unsigned int bone_id, float weight)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		if (BoneWeights[i] == 0.0)
		{
			BoneID[i] = bone_id;
			BoneWeights[i] = weight;
			return;
		}
	}
}

void Model::LoadAnimations(const aiScene* scene)
{
	for (int x = 0; x < scene->mNumAnimations; x++)
	{
		aiAnimation* assImpAnimation = scene->mAnimations[x];

		Animation3D animation = Animation3D();
		animation.TicksPerSec = assImpAnimation->mTicksPerSecond;
		animation.AnimationTime = assImpAnimation->mDuration * animation.TicksPerSec;

		for (int y = 0; y < assImpAnimation->mNumChannels; y++)
		{
			KeyFrame keyframe;
			aiNodeAnim* channel = assImpAnimation->mChannels[y];

			for (auto bone : BoneList)
			{
				if (channel->mNodeName.C_Str() == bone->GetBoneName())
				{
					keyframe.BoneName = channel->mNodeName.C_Str();
					keyframe.BoneId = bone->GetBoneID();
					break;
				}
			}

			for (int z = 0; z < channel->mNumPositionKeys; z++)
			{
				KeyFrameInfo PosKeyFrame;
				PosKeyFrame.Time = channel->mPositionKeys[z].mTime;
				PosKeyFrame.AnimationInfo = aiVector3D(channel->mPositionKeys[z].mValue.x, channel->mPositionKeys[z].mValue.y, channel->mPositionKeys[z].mValue.z);
				keyframe.BonePosition.emplace_back(PosKeyFrame);
			}

			for (int z = 0; z < channel->mNumRotationKeys; z++)
			{
				KeyFrameRotationInfo RotKeyFrame;
				RotKeyFrame.Time = channel->mRotationKeys[z].mTime;
				RotKeyFrame.AnimationInfo = aiQuaternion(channel->mRotationKeys[z].mValue.x, channel->mRotationKeys[z].mValue.y, channel->mRotationKeys[z].mValue.z);
				keyframe.BoneRotation.emplace_back(RotKeyFrame);
			}

			for (int z = 0; z < channel->mNumScalingKeys; z++)
			{
				KeyFrameInfo ScaleKeyFrame;
				ScaleKeyFrame.Time = channel->mScalingKeys[z].mTime;
				ScaleKeyFrame.AnimationInfo = aiVector3D(channel->mScalingKeys[z].mValue.x, channel->mScalingKeys[z].mValue.y, channel->mScalingKeys[z].mValue.z);
				keyframe.BoneScale.emplace_back(ScaleKeyFrame);
			}

			animation.AddBoneKeyFrame(keyframe);
		}

		AnimationList.emplace_back(animation);
	}
}

aiVector3D Model::calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	return p_node_anim->mPositionKeys[frame].mValue;
}

aiQuaternion Model::calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	return p_node_anim->mRotationKeys[frame].mValue;
}

aiVector3D Model::calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	return p_node_anim->mScalingKeys[frame].mValue;
}
const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}
void Model::UpdateSkeleton(const aiNode* p_node, const glm::mat4 ParentMatrix, const aiScene* Scene)
{
	const aiAnimation* animation = Scene->mAnimations[0];
	aiMatrix4x4 node_transform = p_node->mTransformation;

	auto glmTransform = AssimpToGLMMatrixConverter(p_node->mTransformation);
	if (AnimationList.size() != 0)
	{
		for (auto bone : BoneList)
		{
			if (p_node->mName.C_Str() == bone->GetBoneName())
			{
				const aiNodeAnim* node_anim = FindNodeAnim(animation, bone->GetBoneName());
				//scaling
				//aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
				aiVector3D scaling_vector = node_anim->mScalingKeys[frame].mValue;
				aiMatrix4x4 scaling_matr;
				aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

			/*	std::cout << "Scale: Bone Name: " << bone->BoneName << " Frame: " << frame << std::endl;
				std::cout << scaling_matr.a1 << "  " << scaling_matr.a2 << "  " << scaling_matr.a3 << "  " << scaling_matr.a4 << std::endl;
				std::cout << scaling_matr.b1 << "  " << scaling_matr.b2 << "  " << scaling_matr.b3 << "  " << scaling_matr.b4 << std::endl;
				std::cout << scaling_matr.c1 << "  " << scaling_matr.c2 << "  " << scaling_matr.c3 << "  " << scaling_matr.c4 << std::endl;
				std::cout << scaling_matr.d1 << "  " << scaling_matr.c1 << "  " << scaling_matr.d3 << "  " << scaling_matr.d4 << std::endl;*/

				//rotation
				//aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
				aiQuaternion rotate_quat = node_anim->mRotationKeys[frame].mValue;
				aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

		/*		std::cout << "Rotate: Bone Name: " << bone->BoneName << " Frame: " << frame << std::endl;
				std::cout << rotate_matr.a1 << "  " << rotate_matr.a2 << "  " << rotate_matr.a3 << "  " << rotate_matr.a4 << std::endl;
				std::cout << rotate_matr.b1 << "  " << rotate_matr.b2 << "  " << rotate_matr.b3 << "  " << rotate_matr.b4 << std::endl;
				std::cout << rotate_matr.c1 << "  " << rotate_matr.c2 << "  " << rotate_matr.c3 << "  " << rotate_matr.c4 << std::endl;
				std::cout << rotate_matr.d1 << "  " << rotate_matr.c1 << "  " << rotate_matr.d3 << "  " << rotate_matr.d4 << std::endl;*/

				//translation
				//aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
				aiVector3D translate_vector = node_anim->mPositionKeys[frame].mValue;
				aiMatrix4x4 translate_matr;
				aiMatrix4x4::Translation(translate_vector, translate_matr);

				//std::cout << "Translate: Bone Name: " << bone->BoneName << " Frame: " << frame << std::endl;
				//std::cout << translate_matr.a1 << "  " << translate_matr.a2 << "  " << translate_matr.a3 << "  " << translate_matr.a4 << std::endl;
				//std::cout << translate_matr.b1 << "  " << translate_matr.b2 << "  " << translate_matr.b3 << "  " << translate_matr.b4 << std::endl;
				//std::cout << translate_matr.c1 << "  " << translate_matr.c2 << "  " << translate_matr.c3 << "  " << translate_matr.c4 << std::endl;
				//std::cout << translate_matr.d1 << "  " << translate_matr.c1 << "  " << translate_matr.d3 << "  " << translate_matr.d4 << std::endl;

				glmTransform = AssimpToGLMMatrixConverter(translate_matr) * AssimpToGLMMatrixConverter(rotate_matr) * AssimpToGLMMatrixConverter(scaling_matr);

				//std::cout << "Bone Name: " << bone->BoneName << " Frame: " << frame << std::endl;
				//std::cout << glmTransform[0].x << "  " << glmTransform[0].y << "  " << glmTransform[0].z << "  " << glmTransform[0].w << std::endl;
				//std::cout << glmTransform[1].x << "  " << glmTransform[1].y << "  " << glmTransform[1].z << "  " << glmTransform[1].w << std::endl;
				//std::cout << glmTransform[2].x << "  " << glmTransform[2].y << "  " << glmTransform[2].z << "  " << glmTransform[2].w << std::endl;
				//std::cout << glmTransform[3].x << "  " << glmTransform[3].y << "  " << glmTransform[3].z << "  " << glmTransform[3].w << std::endl;

				int a = 34;
			}
		}
	}

	glm::mat4 GlobalTransform = ParentMatrix * glmTransform;

	for (auto bone : BoneList)
	{
		if (p_node->mName.C_Str() == bone->GetBoneName())
		{
			bone->BoneTransformMatrix = AssimpToGLMMatrixConverter(p_node->mTransformation);
			bone->FinalTransformMatrix = GlobalInverseTransformMatrix * GlobalTransform * bone->OffsetMatrix;
		}
	}

	for (int x = 0; x < p_node->mNumChildren; x++)
	{
		UpdateSkeleton(p_node->mChildren[x], GlobalTransform, Scene);
	}
}

void Model::Update(const std::string& FilePath)
{
	Assimp::Importer ModelImporter;

	const aiScene* Scene = ModelImporter.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	UpdateSkeleton(Scene->mRootNode, glm::mat4(1.0f), Scene);

	frame++;
	if (frame > 4)
	{
		frame = 0;
	}
}

glm::mat4 Model::AssimpToGLMMatrixConverter(aiMatrix4x4 AssMatrix)
{
	glm::mat4 GLMMatrix;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			GLMMatrix[x][y] = AssMatrix[y][x];
		}
	}
	return GLMMatrix;
}