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
	void LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex>& VertexList, const aiScene* scene);
	void LoadAnimations(const aiScene* scene);

	void UpdateSkeleton(const aiNode* p_node, const glm::mat4 ParentMatrix, const aiScene* Scene);
	glm::mat4 AssimpToGLMMatrixConverter(aiMatrix4x4 matrix);

	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);

	aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim);
public:
	Model();
	Model(const std::string& FilePath);
	~Model();

	void Update(const std::string& FilePath);

	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;
	std::vector<std::shared_ptr<Bone>> BoneList;
	std::vector<Animation3D> AnimationList;
};

