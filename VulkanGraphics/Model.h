#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <unordered_map>
#include "Animation3D.h"
#include "AnimationPlayer3D.h"

const unsigned int MAX_BONE_VERTEX_COUNT = 4;

class Node
{
	unsigned int NodeID;
	std::shared_ptr<Node> ParentNode;
	std::vector<std::shared_ptr<Node>> Children;
};

class Model
{
private:
	std::string ModelName;
	glm::mat4 ModelTransformMatrix = glm::mat4(1.0f);



	std::vector<Animation3D> AnimationList;
	std::vector<NodeMap> NodeMapList;
	glm::mat4 GlobalInverseTransformMatrix;
	AnimationPlayer3D AnimationPlayer;

	int RenderFlags;

	void LoadMesh(VulkanEngine& engine, std::shared_ptr<TextureManager>& textureManager, const std::string& FilePath, aiNode* node, const aiScene* scene);
	std::vector<Vertex> LoadVertices(aiMesh* mesh);
	std::vector<uint16_t> LoadIndices(aiMesh* mesh);
	void LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex>& VertexList);
	void LoadNodeTree(const aiNode* Node, int parentNodeID = -1);
	void LoadAnimations(const aiScene* scene);
	void LoadTextures(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, MeshData& Properties, const std::string& FilePath, aiMesh* mesh, const aiScene* scene);

	void LoadMeshTransform(const int NodeID = 0, const glm::mat4 ParentMatrix = glm::mat4(1.0f));
	void BoneWeightPlacement(std::vector<Vertex>& VertexList, unsigned int vertexID, unsigned int bone_id, float weight);

	glm::mat4 AssimpToGLMMatrixConverter(aiMatrix4x4 matrix);

public:

	std::vector<MeshData> SubMeshList;
	std::vector<std::shared_ptr<Mesh>> MeshList;
	std::vector<std::shared_ptr<Bone>> BoneList;

	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotation = glm::vec3(0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);



	Model();
	Model(VulkanEngine& engine, std::shared_ptr<TextureManager>& textureManager, const std::string& FilePath, VkDescriptorSetLayout layout, int renderFlags);
	~Model();

	void Draw(VkCommandBuffer& RenderCommandBuffer, std::shared_ptr<GraphicsPipeline> pipeline, int FrameNumber);
	void Update(VulkanEngine& engine, std::shared_ptr<PerspectiveCamera>& camera, LightBufferObject& light);
	void UpdateImGUI();
	void Destroy(VulkanEngine& engine);

	int GetRenderFlags() { return RenderFlags; }
};
