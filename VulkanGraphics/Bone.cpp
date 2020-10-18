#include "Bone.h"
#include <iostream>

Bone::Bone()
{
	OffsetMatrix = glm::mat4(1.0f);
	FinalTransformMatrix = glm::mat4(1.0f);
}

Bone::Bone(const std::string& boneName, int boneId, glm::mat4 boneTransformMatrix, glm::mat4 offsetMatrix)
{
	BoneID = boneId;
	BoneName = boneName;
	OffsetMatrix = offsetMatrix; 
	BoneTransformMatrix = boneTransformMatrix;
	FinalTransformMatrix = glm::mat4(1.0f);
}

Bone::~Bone()
{
}

void Bone::SetParentBone(const glm::mat4& GlobalInverseTransformMatrix, std::shared_ptr<Bone> parentBone)
{
	ParentBone = parentBone;
}

void Bone::AddChildBone(std::shared_ptr<Bone> bone)
{
	ChildrenBoneList.emplace_back(bone);
}