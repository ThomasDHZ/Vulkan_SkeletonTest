#include "Bone.h"
#include <iostream>

Bone::Bone()
{
	OffsetMatrix = glm::mat4(1.0f);
	FinalTransformMatrix = glm::mat4(1.0f);
}

Bone::Bone(const std::string& boneName, int boneId, glm::mat4 offsetMatrix)
{
	BoneID = boneId;
	BoneName = boneName;
	OffsetMatrix = offsetMatrix; 
	FinalTransformMatrix = glm::mat4(1.0f);
}

Bone::~Bone()
{
}
