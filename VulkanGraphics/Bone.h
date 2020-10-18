#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Vertex.h"
#include <assimp\scene.h>

class Bone
{
private:



public:
	int BoneID;
	std::string BoneName;
	glm::mat4 OffsetMatrix;
	glm::mat4 BoneTransformMatrix;
	glm::mat4 FinalTransformMatrix;
	std::shared_ptr<Bone> ParentBone;
	std::vector<std::shared_ptr<Bone>> ChildrenBoneList;

	Bone();
	Bone(const std::string& boneName, int boneId, glm::mat4 boneTransformMatrix, glm::mat4 offsetMatrix);
	~Bone();

	void SetParentBone(const glm::mat4& GlobalInverseTransformMatrix, std::shared_ptr<Bone> parentBone);
	void AddChildBone(std::shared_ptr<Bone> bone);

	int GetBoneID() { return BoneID; }
	const std::string& GetBoneName() { return BoneName; }
	const std::vector<std::shared_ptr<Bone>> GetChildrenBoneList() { return ChildrenBoneList; }
	const glm::mat4 GetOffsetMatrix() { return OffsetMatrix; }
	const glm::mat4 GetBoneTransformMatrix() { return BoneTransformMatrix; }
	const glm::mat4 GetFinalBoneTransformMatrix() { return FinalTransformMatrix; }
};

