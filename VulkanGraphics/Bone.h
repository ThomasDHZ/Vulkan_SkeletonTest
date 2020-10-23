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
	glm::mat4 FinalTransformMatrix;

	Bone();
	Bone(const std::string& boneName, int boneId, glm::mat4 offsetMatrix);
	~Bone();

	int GetBoneID() { return BoneID; }
	const std::string& GetBoneName() { return BoneName; }
	const glm::mat4 GetOffsetMatrix() { return OffsetMatrix; }
	const glm::mat4 GetFinalBoneTransformMatrix() { return FinalTransformMatrix; }
};

