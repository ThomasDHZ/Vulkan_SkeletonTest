#include "Bone.h"
#include <iostream>

Bone::Bone()
{
	OffsetMatrix = glm::mat4(1.0f);
	BoneTransformMatrix = glm::mat4(1.0f);
}

Bone::Bone(const std::string& boneName, int boneId, glm::mat4 offsetMatrix)
{
	BoneID = boneId;
	BoneName = boneName;
	OffsetMatrix = offsetMatrix; 
	BoneTransformMatrix = glm::mat4(1.0f);
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



void Bone::InvertBoneMatrix(const glm::mat4& GlobalInverseTransformMatrix)
{

	glm::mat4 ParentMatrix = glm::mat4(1.0f);
	if (ParentBone.get() != nullptr)
	{
		ParentMatrix = ParentBone->GetBoneTransformMatrix();
	}
	glm::mat4 GlobalTransform = ParentMatrix * glm::mat4(1.0f);
	BoneTransformMatrix = GlobalInverseTransformMatrix * GlobalTransform * OffsetMatrix;
}
