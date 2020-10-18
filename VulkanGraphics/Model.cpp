#include "Model.h"
#include <iostream>

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

	GlobalInverseTransform = AssimpToGLMMatrixConverter(Scene->mRootNode->mTransformation.Inverse());

	ProcessNode(FilePath, Scene->mRootNode, Scene);
}

void Model::ProcessNode(const std::string& FilePath, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		LoadVertices(mesh);
		LoadIndices(mesh);
		LoadBones(scene->mRootNode, mesh, VertexList);
		LoadAnimations(scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(FilePath, node->mChildren[i], scene);
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

	for (auto bone : BoneList)
	{
		auto BoneAsNode = RootNode->FindNode(bone->GetBoneName().c_str());
		for (int x = 0; x < BoneAsNode->mNumChildren; x++)
		{
			for (auto bone2 : BoneList)
			{
				if (bone2->GetBoneName() == BoneAsNode->mChildren[x]->mName.C_Str())
				{
					bone2->SetParentBone(GlobalInverseTransform, bone);
					bone->AddChildBone(bone2);
				}
			}
		}
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
			AffectedVertices[vertexID]++;
			switch (AffectedVertices[vertexID])
			{
			case 1:
				VertexList[vertexID].BoneID.x = vertexID;
				VertexList[vertexID].BoneWeights.x = weight;
				break;
			case 2:
				VertexList[vertexID].BoneID.y = vertexID;
				VertexList[vertexID].BoneWeights.y = weight;
				break;
			case 3:
				VertexList[vertexID].BoneID.z = vertexID;
				VertexList[vertexID].BoneWeights.z = weight;
				break;
			case 4:
				VertexList[vertexID].BoneID.w = vertexID;
				VertexList[vertexID].BoneWeights.w = weight;
				break;
			}
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

	InvertBoneMatrix(GlobalInverseTransform, RootNode, glm::mat4(1.0f));
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
				PosKeyFrame.AnimationInfo = glm::vec3(channel->mPositionKeys[z].mValue.x, channel->mPositionKeys[z].mValue.y, channel->mPositionKeys[z].mValue.z);
				keyframe.BonePosition.emplace_back(PosKeyFrame);
			}

			for (int z = 0; z < channel->mNumRotationKeys; z++)
			{
				KeyFrameInfo RotKeyFrame;
				RotKeyFrame.Time = channel->mRotationKeys[z].mTime;
				RotKeyFrame.AnimationInfo = glm::vec3(channel->mRotationKeys[z].mValue.x, channel->mRotationKeys[z].mValue.y, channel->mRotationKeys[z].mValue.z);
				keyframe.BoneRotation.emplace_back(RotKeyFrame);
			}

			for (int z = 0; z < channel->mNumScalingKeys; z++)
			{
				KeyFrameInfo ScaleKeyFrame;
				ScaleKeyFrame.Time = channel->mScalingKeys[z].mTime;
				ScaleKeyFrame.AnimationInfo = glm::vec3(channel->mScalingKeys[z].mValue.x, channel->mScalingKeys[z].mValue.y, channel->mScalingKeys[z].mValue.z);
				keyframe.BoneScale.emplace_back(ScaleKeyFrame);
			}

			animation.AddBoneKeyFrame(keyframe);
		}

		AnimationList.emplace_back(animation);
	}
}

void Model::InvertBoneMatrix(const glm::mat4& GlobalInverseTransformMatrix, const aiNode* p_node, const glm::mat4 ParentMatrix)
{
	auto glmTransform = AssimpToGLMMatrixConverter(p_node->mTransformation);
	glm::mat4 GlobalTransform = ParentMatrix * glmTransform;

	std::cout << "Transform" << std::endl;
	std::cout << glmTransform[0].x << "  " << glmTransform[0].y << "  " << glmTransform[0].z << "  " << glmTransform[0].w << std::endl;
	std::cout << glmTransform[1].x << "  " << glmTransform[1].y << "  " << glmTransform[1].z << "  " << glmTransform[1].w << std::endl;
	std::cout << glmTransform[2].x << "  " << glmTransform[2].y << "  " << glmTransform[2].z << "  " << glmTransform[2].w << std::endl;
	std::cout << glmTransform[3].x << "  " << glmTransform[3].y << "  " << glmTransform[3].z << "  " << glmTransform[3].w << std::endl;

	std::cout << "Parent: " << std::endl;
	std::cout << ParentMatrix[0].x << "  " << ParentMatrix[0].y << "  " << ParentMatrix[0].z << "  " << ParentMatrix[0].w << std::endl;
	std::cout << ParentMatrix[1].x << "  " << ParentMatrix[1].y << "  " << ParentMatrix[1].z << "  " << ParentMatrix[1].w << std::endl;
	std::cout << ParentMatrix[2].x << "  " << ParentMatrix[2].y << "  " << ParentMatrix[2].z << "  " << ParentMatrix[2].w << std::endl;
	std::cout << ParentMatrix[3].x << "  " << ParentMatrix[3].y << "  " << ParentMatrix[3].z << "  " << ParentMatrix[3].w << std::endl;

	for (auto bone : BoneList)
	{
		if (p_node->mName.C_Str() == bone->GetBoneName())
		{
			bone->BoneTransformMatrix = AssimpToGLMMatrixConverter(p_node->mTransformation);
			bone->FinalTransformMatrix = GlobalInverseTransformMatrix * GlobalTransform * bone->OffsetMatrix;

			std::cout << "Offset" << std::endl;
			std::cout << bone->OffsetMatrix[0].x << "  " << bone->OffsetMatrix[0].y << "  " << bone->OffsetMatrix[0].z << "  " << bone->OffsetMatrix[0].w << std::endl;
			std::cout << bone->OffsetMatrix[1].x << "  " << bone->OffsetMatrix[1].y << "  " << bone->OffsetMatrix[1].z << "  " << bone->OffsetMatrix[1].w << std::endl;
			std::cout << bone->OffsetMatrix[2].x << "  " << bone->OffsetMatrix[2].y << "  " << bone->OffsetMatrix[2].z << "  " << bone->OffsetMatrix[2].w << std::endl;
			std::cout << bone->OffsetMatrix[3].x << "  " << bone->OffsetMatrix[3].y << "  " << bone->OffsetMatrix[3].z << "  " << bone->OffsetMatrix[3].w << std::endl;

			std::cout << "global" << std::endl;
			std::cout << GlobalTransform[0].x << "  " << GlobalTransform[0].y << "  " << GlobalTransform[0].z << "  " << GlobalTransform[0].w << std::endl;
			std::cout << GlobalTransform[1].x << "  " << GlobalTransform[1].y << "  " << GlobalTransform[1].z << "  " << GlobalTransform[1].w << std::endl;
			std::cout << GlobalTransform[2].x << "  " << GlobalTransform[2].y << "  " << GlobalTransform[2].z << "  " << GlobalTransform[2].w << std::endl;
			std::cout << GlobalTransform[3].x << "  " << GlobalTransform[3].y << "  " << GlobalTransform[3].z << "  " << GlobalTransform[3].w << std::endl;

			std::cout << "GlobalInverseTransform" << std::endl;
			std::cout << GlobalInverseTransformMatrix[0].x << "  " << GlobalInverseTransformMatrix[0].y << "  " << GlobalInverseTransformMatrix[0].z << "  " << GlobalInverseTransformMatrix[0].w << std::endl;
			std::cout << GlobalInverseTransformMatrix[1].x << "  " << GlobalInverseTransformMatrix[1].y << "  " << GlobalInverseTransformMatrix[1].z << "  " << GlobalInverseTransformMatrix[1].w << std::endl;
			std::cout << GlobalInverseTransformMatrix[2].x << "  " << GlobalInverseTransformMatrix[2].y << "  " << GlobalInverseTransformMatrix[2].z << "  " << GlobalInverseTransformMatrix[2].w << std::endl;
			std::cout << GlobalInverseTransformMatrix[3].x << "  " << GlobalInverseTransformMatrix[3].y << "  " << GlobalInverseTransformMatrix[3].z << "  " << GlobalInverseTransformMatrix[3].w << std::endl;

			std::cout << bone->GetBoneName() << std::endl;
			std::cout << bone->FinalTransformMatrix[0].x << "  " << bone->FinalTransformMatrix[0].y << "  " << bone->FinalTransformMatrix[0].z << "  " << bone->FinalTransformMatrix[0].w << std::endl;
			std::cout << bone->FinalTransformMatrix[1].x << "  " << bone->FinalTransformMatrix[1].y << "  " << bone->FinalTransformMatrix[1].z << "  " << bone->FinalTransformMatrix[1].w << std::endl;
			std::cout << bone->FinalTransformMatrix[2].x << "  " << bone->FinalTransformMatrix[2].y << "  " << bone->FinalTransformMatrix[2].z << "  " << bone->FinalTransformMatrix[2].w << std::endl;
			std::cout << bone->FinalTransformMatrix[3].x << "  " << bone->FinalTransformMatrix[3].y << "  " << bone->FinalTransformMatrix[3].z << "  " << bone->FinalTransformMatrix[3].w << std::endl;

			int a = 34;
		}
	}

	for (int x = 0; x < p_node->mNumChildren; x++)
	{
		InvertBoneMatrix(GlobalInverseTransformMatrix, p_node->mChildren[x], GlobalTransform);
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