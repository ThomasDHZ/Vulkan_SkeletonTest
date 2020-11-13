#include "Animation3D.h"

Animation3D::Animation3D()
{
}

Animation3D::~Animation3D()
{
}

void Animation3D::AddBoneKeyFrame(KeyFrame keyFrame)
{
	BoneKeyFrameList.emplace_back(keyFrame);
}
