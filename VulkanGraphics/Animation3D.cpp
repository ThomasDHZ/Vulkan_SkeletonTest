#include "Animation3D.h"

Animation3D::Animation3D()
{
}

//Animation3D::Animation3D(const std::shared_ptr<std::vector<Bone>> skeleton)
//{
//	Skeleton = skeleton;
//}

Animation3D::~Animation3D()
{
}

void Animation3D::AddBoneKeyFrame(KeyFrame keyFrame)
{
	BoneKeyFrameList.emplace_back(keyFrame);
}

void Animation3D::UpdateBones(std::vector<std::shared_ptr<Bone>> Skeleton, glm::mat4 GlobalInverseTransform)
{

	for (auto bone : Skeleton)
	{
		bone->InvertBoneMatrix(GlobalInverseTransform);
	}
}
