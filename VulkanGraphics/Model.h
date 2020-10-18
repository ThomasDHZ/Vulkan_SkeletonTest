#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include "Vertex.h"
#include <memory>
#include "Bone.h"
#include "Animation3D.h"

class Model
{
private:

	void LoadModel(const std::string& FilePath);
	void ProcessNode(const std::string& FilePath, aiNode* node, const aiScene* scene);
	void InvertBoneMatrix(const glm::mat4& GlobalInverseTransformMatrix, const aiNode* p_node, const glm::mat4 ParentMatrix);

	void LoadVertices(aiMesh* mesh);
	void LoadIndices(aiMesh* mesh);
	void LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex>& VertexList);
	void LoadAnimations(const aiScene* scene);

	glm::mat4 AssimpToGLMMatrixConverter(aiMatrix4x4 matrix);

public:
	Model();
	Model(const std::string& FilePath);
	~Model();

	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;
	std::vector<std::shared_ptr<Bone>> BoneList;
	std::vector<Animation3D> AnimationList;
	glm::mat4 GlobalInverseTransform;
};

