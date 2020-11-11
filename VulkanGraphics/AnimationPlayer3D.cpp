#include "AnimationPlayer3D.h"
#include <GLFW\glfw3.h>

AnimationPlayer3D::AnimationPlayer3D()
{
	AniTime = 0.0f;
	AnimationPlaySpeed = 0.0f;
	PlayAnimationFlag = false;
}

AnimationPlayer3D::AnimationPlayer3D(std::vector<std::shared_ptr<Bone>> skeleton, std::vector<NodeMap> nodeMapList, glm::mat4 globalInverseTransformMatrix, Animation3D StartingAnimation)
{
	Skeleton = skeleton;
	NodeMapList = nodeMapList;
	GlobalInverseTransformMatrix = globalInverseTransformMatrix;
	CurrentAnimation = StartingAnimation;
	AniTime = 0.0f;
	AnimationPlaySpeed = 1.0f;
	PlayAnimationFlag = true;
}

AnimationPlayer3D::~AnimationPlayer3D()
{
}

void AnimationPlayer3D::Update()
{
	if (PlayAnimationFlag)
	{
		UpdateSkeleton(0, glm::mat4(1.0f));
	}
}

void AnimationPlayer3D::SetAnimation(Animation3D animation)
{
	CurrentAnimation = animation;
}

void AnimationPlayer3D::SetPlayAnimationFlag(bool Flag)
{
	PlayAnimationFlag = Flag;
}

aiVector3D AnimationPlayer3D::InterpolatePosition(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID)
{
	if (CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition.size() == 1)
	{
		return CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[0].AnimationInfo;
	}

	int Frame = 0;
	for (int x = CurrentAnimation.BoneKeyFrameList[0].BonePosition.size() - 1; x > 0; x--)
	{
		if (AnimationTime >= CurrentAnimation.BoneKeyFrameList[0].BonePosition[x].Time)
		{
			Frame = x;
			break;
		}
	}

	int NextFrame = Frame + 1;
	if (NextFrame >= CurrentAnimation.BoneKeyFrameList[0].BonePosition.size() - 1)
	{
		NextFrame = 0;
	}

	float delta_time = (float)(CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[NextFrame].Time - CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[Frame].Time);
	float factor = (AnimationTime - (float)CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[Frame].Time) / delta_time;

	aiVector3D StartPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[Frame].AnimationInfo;
	aiVector3D EndPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[NextFrame].AnimationInfo;
	aiVector3D Diffrence = EndPos - StartPos;

	return StartPos + factor * Diffrence;
}

aiQuaternion AnimationPlayer3D::InterpolateRotation(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID)
{
	if (CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation.size() == 1)
	{
		return CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[0].AnimationInfo;
	}

	int Frame = 0;
	for (int x = CurrentAnimation.BoneKeyFrameList[0].BoneRotation.size() - 1; x > 0; x--)
	{
		if (AnimationTime >= CurrentAnimation.BoneKeyFrameList[0].BoneRotation[x].Time)
		{
			Frame = x;
			break;
		}
	}

	int NextFrame = Frame + 1;
	if (NextFrame >= CurrentAnimation.BoneKeyFrameList[0].BoneRotation.size() - 1)
	{
		NextFrame = 0;
	}

	float delta_time = (float)(CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[NextFrame].Time - CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[Frame].Time);
	float factor = (AnimationTime - (float)CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[Frame].Time) / delta_time;

	aiQuaternion StartPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[Frame].AnimationInfo;
	aiQuaternion EndPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[NextFrame].AnimationInfo;

	return nlerp(StartPos, EndPos, factor);
}

aiVector3D AnimationPlayer3D::InterpolateScaling(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID)
{
	if (CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale.size() == 1)
	{
		return CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[0].AnimationInfo;
	}

	int Frame = 0;
	for (int x = CurrentAnimation.BoneKeyFrameList[0].BoneScale.size() - 1; x > 0; x--)
	{
		if (AnimationTime >= CurrentAnimation.BoneKeyFrameList[0].BoneScale[x].Time)
		{
			Frame = x;
			break;
		}
	}

	int NextFrame = Frame + 1;
	if (NextFrame >= CurrentAnimation.BoneKeyFrameList[0].BoneScale.size() - 1)
	{
		NextFrame = 0;
	}

	float delta_time = (float)(CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[NextFrame].Time - CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[Frame].Time);
	float factor = (AnimationTime - (float)CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[Frame].Time) / delta_time;

	aiVector3D StartPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[Frame].AnimationInfo;
	aiVector3D EndPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[NextFrame].AnimationInfo;
	aiVector3D Diffrence = EndPos - StartPos;

	return StartPos + factor * Diffrence;
}

void AnimationPlayer3D::UpdateSkeleton(const int NodeID = 0, const glm::mat4 ParentMatrix = glm::mat4(1.0f))
{
	glm::mat4 glmTransform = AssimpToGLMMatrixConverter(NodeMapList[NodeID].NodeTransform);

	auto Time = (float)glfwGetTime() * AnimationPlaySpeed * CurrentAnimation.TicksPerSec;
	AniTime = fmod(Time, CurrentAnimation.AnimationTime);

	for (auto bone : Skeleton)
	{
		if (NodeMapList[NodeID].NodeString == bone->BoneName)
		{
			aiMatrix4x4 ScaleMatrix;
			aiMatrix4x4 TranslateMatrix;

			aiVector3D scaling_vector = InterpolateScaling(bone, AniTime, NodeID);
			aiMatrix4x4::Scaling(scaling_vector, ScaleMatrix);

			aiQuaternion rotate_quat = InterpolateRotation(bone, AniTime, NodeID);
			aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

			aiVector3D translate_vector = InterpolatePosition(bone, AniTime, NodeID);
			aiMatrix4x4::Translation(translate_vector, TranslateMatrix);

			glmTransform = AssimpToGLMMatrixConverter(TranslateMatrix * rotate_matr * ScaleMatrix);
		}
	}

	glm::mat4 GlobalTransform = ParentMatrix * glmTransform;

	for (auto bone : Skeleton)
	{
		if (NodeMapList[NodeID].NodeString == bone->BoneName)
		{
			bone->FinalTransformMatrix = GlobalInverseTransformMatrix * GlobalTransform * bone->OffsetMatrix;
		}
	}

	for (int x = 0; x < NodeMapList[NodeID].ChildNodeList.size(); x++)
	{
		UpdateSkeleton(NodeMapList[NodeID].ChildNodeList[x], GlobalTransform);
	}
}

glm::mat4 AnimationPlayer3D::AssimpToGLMMatrixConverter(aiMatrix4x4 matrix)
{
	glm::mat4 GLMMatrix;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			GLMMatrix[x][y] = matrix[y][x];
		}
	}
	return GLMMatrix;
}

aiQuaternion AnimationPlayer3D::nlerp(aiQuaternion quaternion1, aiQuaternion quaternion2, float blend)
{
	quaternion1.Normalize();
	quaternion2.Normalize();

	aiQuaternion result;
	float dot_product = quaternion1.x * quaternion2.x + quaternion1.y * quaternion2.y + quaternion1.z * quaternion2.z + quaternion1.w * quaternion2.w;
	float one_minus_blend = 1.0f - blend;

	if (dot_product < 0.0f)
	{
		result.x = quaternion1.x * one_minus_blend + blend * -quaternion2.x;
		result.y = quaternion1.y * one_minus_blend + blend * -quaternion2.y;
		result.z = quaternion1.z * one_minus_blend + blend * -quaternion2.z;
		result.w = quaternion1.w * one_minus_blend + blend * -quaternion2.w;
	}
	else
	{
		result.x = quaternion1.x * one_minus_blend + blend * quaternion2.x;
		result.y = quaternion1.y * one_minus_blend + blend * quaternion2.y;
		result.z = quaternion1.z * one_minus_blend + blend * quaternion2.z;
		result.w = quaternion1.w * one_minus_blend + blend * quaternion2.w;
	}

	return result.Normalize();
}