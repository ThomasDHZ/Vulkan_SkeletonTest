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
#include "glm\gtc\quaternion.hpp"


#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

struct NodeMap
{
	std::string NodeString;
	aiMatrix4x4 NodeTransform;
	 int ParentNodeID;
	 int NodeID;
	std::vector<int> ChildNodeList;
};

class Model
{
private:

	glm::mat4 GlobalInverseTransformMatrix;
	Animation3D CurrentAnimation;
	int frame = 0;

	void LoadModel(const std::string& FilePath);
	void ProcessNode(aiNode* node, const aiScene* scene);

	void LoadVertices(aiMesh* mesh);
	void LoadIndices(aiMesh* mesh);
	void LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex>& VertexList);
	void LoadNodeTree(const aiNode* RootNode,  int ParentNodeID);
	void LoadAnimations(const aiScene* scene);

	void BoneWeightPlacement(unsigned int vertexID, unsigned int bone_id, float weight);
	void UpdateSkeleton(const aiNode* p_node, const glm::mat4 ParentMatrix);
	int GetFrame(const Animation3D& animation, const float Time);

	glm::mat4 AssimpToGLMMatrixConverter(aiMatrix4x4 matrix);

public:
	Model();
	Model(const std::string& FilePath);
	~Model();

	void Update(const std::string& FilePath);

	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;
	std::vector<std::shared_ptr<Bone>> BoneList;
	std::vector<Animation3D> AnimationList;
	std::vector<NodeMap> NodeMapList;
};

