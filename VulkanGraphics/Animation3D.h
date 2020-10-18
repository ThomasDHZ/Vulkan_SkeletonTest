#pragma once
#include <memory>
#include "Bone.h"

struct KeyFrameInfo
{
	float Time;
	glm::vec3 AnimationInfo;
};
struct KeyFrame
{
	int BoneId;
	std::string BoneName;
	std::vector<KeyFrameInfo> BonePosition;
	std::vector<KeyFrameInfo> BoneRotation;
	std::vector<KeyFrameInfo> BoneScale;
};

class Animation3D
{
private:
	std::vector<KeyFrame> BoneKeyFrameList;

public:
	float AnimationTime;
	float TicksPerSec;

	Animation3D();
	~Animation3D();

	void AddBoneKeyFrame(KeyFrame keyFrame);
	void UpdateBones(std::vector<std::shared_ptr<Bone>> Skeleton, glm::mat4 GlobalInverseTransform);
};
