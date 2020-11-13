#pragma once
#include "Animation3D.h"

struct NodeMap
{
	std::string NodeString;
	aiMatrix4x4 NodeTransform;
	int ParentNodeID;
	int NodeID;
	std::vector<int> ChildNodeList;
	int MeshID;
};

class AnimationPlayer3D
{
private:

	Animation3D CurrentAnimation;
	std::vector<std::shared_ptr<Bone>> Skeleton;

	std::vector<NodeMap> NodeMapList;
	glm::mat4 GlobalInverseTransformMatrix;

	float AniTime;
	float AnimationPlaySpeed;
	bool PlayAnimationFlag;

	aiVector3D InterpolatePosition(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID);
	aiQuaternion InterpolateRotation(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID);
	aiVector3D InterpolateScaling(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID);

	void UpdateSkeleton(const int NodeID, const glm::mat4 ParentMatrix);

	glm::mat4 AssimpToGLMMatrixConverter(aiMatrix4x4 matrix);
	aiQuaternion nlerp(aiQuaternion quaternion1, aiQuaternion quaternion2, float blend);

public:
	AnimationPlayer3D();
	AnimationPlayer3D(std::vector<std::shared_ptr<Bone>> skeleton, std::vector<NodeMap> nodeMapList, glm::mat4 globalInverseTransformMatrix, Animation3D StartingAnimation);
	~AnimationPlayer3D();

	void Update();

	void SetAnimation(Animation3D animation);
	void SetPlayAnimationFlag(bool Flag);

	float GetAnimationTime() { return AniTime; }
	float GetAnimationPlaySpeed() { return AnimationPlaySpeed; }
	float GetAnimationLength() { return CurrentAnimation.AnimationTime; }
	bool GetPlayAnimationFlag() { return PlayAnimationFlag; }
	Animation3D GetCurrentAnimation() { return CurrentAnimation; }

	float* GetAnimationTimePtr() { return &AniTime; }
	float* GetAnimationLengthPtr() { return &CurrentAnimation.AnimationTime; }
	float* GetAnimationPlaySpeedPtr() { return &AnimationPlaySpeed; }
};

