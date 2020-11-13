#pragma once
#include <memory>
#include "Bone.h"

struct KeyFrameInfo
{
	float Time;
	aiVector3D AnimationInfo;
};
struct KeyFrameRotationInfo
{
	float Time;
	aiQuaternion AnimationInfo;
};
struct KeyFrame
{
	int BoneId;
	std::string BoneName;
	std::vector<KeyFrameInfo> BonePosition;
	std::vector<KeyFrameRotationInfo> BoneRotation;
	std::vector<KeyFrameInfo> BoneScale;
};

class Animation3D
{
private:


public:
	std::vector<KeyFrame> BoneKeyFrameList;
	float AnimationTime;
	float TicksPerSec;

	Animation3D();
	~Animation3D();

	void AddBoneKeyFrame(KeyFrame keyFrame);
};
