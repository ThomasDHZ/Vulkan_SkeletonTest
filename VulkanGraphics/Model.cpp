#include "Model.h"
#include <iostream>
#include <glm\ext\matrix_transform.hpp>
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW\glfw3.h>
Model::Model()
{
}

Model::Model(const std::string& FilePath)
{
	LoadModel(FilePath);
	if (AnimationList.size() > 0)
	{
		CurrentAnimation = AnimationList[0];
	}
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
		LoadBones(scene->mRootNode, mesh, VertexList);
		LoadNodeTree(scene->mRootNode, -1);
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

void Model::LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex>& VertexList)
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
		for (int y = 0; y < bone->mNumWeights; y++)
		{
			unsigned int vertexID = bone->mWeights[y].mVertexId;
			float weight = bone->mWeights[y].mWeight;
			BoneWeightPlacement(vertexID, x, weight);
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

	UpdateSkeleton(RootNode, glm::mat4(1.0f));
}

void Model::LoadNodeTree(const aiNode* Node, int parentNodeID = -1)
{
	NodeMap node;
	node.NodeID = NodeMapList.size();
	node.ParentNodeID = parentNodeID;
	node.NodeString = Node->mName.C_Str();
	node.NodeTransform = Node->mTransformation;
	if (parentNodeID != -1)
	{
		NodeMapList[parentNodeID].ChildNodeList.emplace_back(node.NodeID);
	}
	NodeMapList.emplace_back(node);

	for (int x = 0; x < Node->mNumChildren; x++)
	{
		LoadNodeTree(Node->mChildren[x], node.NodeID);
	}
}


void Model::BoneWeightPlacement(unsigned int vertexID, unsigned int bone_id, float weight)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		if (VertexList[vertexID].BoneWeights[i] == 0.0)
		{
			VertexList[vertexID].BoneID[i] = bone_id;
			VertexList[vertexID].BoneWeights[i] = weight;
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
				RotKeyFrame.AnimationInfo = aiQuaternion(channel->mRotationKeys[z].mValue.w, channel->mRotationKeys[z].mValue.x, channel->mRotationKeys[z].mValue.y, channel->mRotationKeys[z].mValue.z);
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

void Model::UpdateSkeleton(const aiNode* p_node, const glm::mat4 ParentMatrix)
{
	glm::mat4 glmTransform = AssimpToGLMMatrixConverter(p_node->mTransformation);
	if (AnimationList.size() != 0)
	{
		auto Time = (float)glfwGetTime() * CurrentAnimation.TicksPerSec;
		float AnimationTime = fmod(Time, CurrentAnimation.AnimationTime);
		frame = GetFrame(CurrentAnimation, AnimationTime);
		for (auto bone : BoneList)
		{
			if (p_node->mName.C_Str() == bone->GetBoneName())
			{
				aiMatrix4x4 ScaleMatrix;
				aiMatrix4x4 TranslateMatrix;

				aiVector3D scaling_vector = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[frame].AnimationInfo;
				aiMatrix4x4::Scaling(scaling_vector, ScaleMatrix);

				aiQuaternion rotate_quat = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[frame].AnimationInfo;
				aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

				aiVector3D translate_vector = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[frame].AnimationInfo;
				aiMatrix4x4::Translation(translate_vector, TranslateMatrix);

				glmTransform = AssimpToGLMMatrixConverter(TranslateMatrix * rotate_matr * ScaleMatrix);
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
		UpdateSkeleton(p_node->mChildren[x], GlobalTransform);
	}
}

int Model::GetFrame(const Animation3D& animation, const float Time)
{
	for (int x = animation.BoneKeyFrameList[0].BonePosition.size() - 1; x > 0; x--)
	{
		if (Time >= animation.BoneKeyFrameList[0].BonePosition[x].Time)
		{
			return x;
		}
	}
}

void Model::Update(const std::string& FilePath)
{
	
	Assimp::Importer ModelImporter;

	const aiScene* Scene = ModelImporter.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	UpdateSkeleton(Scene->mRootNode, glm::mat4(1.0f));


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