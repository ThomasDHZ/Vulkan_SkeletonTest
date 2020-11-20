#include "Frame2D.h"

Frame2D::Frame2D()
{
}

Frame2D::Frame2D(glm::vec2 uVOffset)
{
	UVOffset = uVOffset;
}

Frame2D::Frame2D(glm::vec2 uVOffset, float frameTime)
{
	FrameTime = frameTime;
	UVOffset = uVOffset;
}

Frame2D::Frame2D(glm::vec2 uVOffset, glm::vec2 uVScale, float frameTime)
{
	FrameTime = frameTime;
	UVOffset = uVOffset;
	UVScale = uVScale;
}

Frame2D::~Frame2D()
{
}