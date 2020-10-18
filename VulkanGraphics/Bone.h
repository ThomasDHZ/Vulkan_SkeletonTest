#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Vertex.h"

class Bone
{
private:
	int BoneID;
	std::string BoneName;
	glm::mat4 OffsetMatrix;
	glm::mat4 BoneTransformMatrix;
	std::shared_ptr<Bone> ParentBone;
	std::vector<std::shared_ptr<Bone>> ChildrenBoneList;

public:
	Bone();
	Bone(const std::string& boneName, int boneId, glm::mat4 boneTransformMatrix);
	~Bone();

	void SetParentBone(const glm::mat4& GlobalInverseTransformMatrix, std::shared_ptr<Bone> parentBone);
	void AddChildBone(std::shared_ptr<Bone> bone);
	void InvertBoneMatrix(const glm::mat4& GlobalInverseTransformMatrix);

	int GetBoneID() { return BoneID; }
	const std::string& GetBoneName() { return BoneName; }
	const std::vector<std::shared_ptr<Bone>> GetChildrenBoneList() { return ChildrenBoneList; }
	const glm::mat4 GetOffsetMatrix() { return OffsetMatrix; }
	const glm::mat4 GetBoneTransformMatrix() { return BoneTransformMatrix; }
};

