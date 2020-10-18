#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include "Vertex.h"
#include <memory>
#include "Bone.h"

class Model
{
private:

	aiMatrix4x4 assGlobalInverseTranform;
	glm::mat4 GlobalInverseTransform;

	void LoadModel(const std::string& FilePath);
	void ProcessNode(const std::string& FilePath, aiNode* node, const aiScene* scene);
	std::vector<Vertex> LoadVertices(aiMesh* mesh);
	std::vector<uint16_t> LoadIndices(aiMesh* mesh);
	std::vector<std::shared_ptr<Bone>> LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex>& VertexList);


	glm::mat4 AssimpToGLMMatrixConverter(aiMatrix4x4 matrix);

public:
	Model();
	Model(const std::string& FilePath);
	~Model();

	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;
	std::vector<std::shared_ptr<Bone>> BoneList;
};

