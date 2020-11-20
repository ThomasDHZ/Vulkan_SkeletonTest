#include "Animation2D.h"
#include <chrono>

Animation2D::Animation2D()
{
	CurrentFrame = 0;
	TimeBetweenFrames = 0;
}

Animation2D::Animation2D(std::vector<Frame2D> frameList, float timeBetweenFrames, unsigned int animationID)
{
	AnimationID = animationID;
	CurrentFrame = 0;
	lastframeTime = 0;
	TimeBetweenFrames = timeBetweenFrames;
	FrameList = frameList;
}

void Animation2D::Update()
{
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	if ((time - lastframeTime) > TimeBetweenFrames)
	{
		if (CurrentFrame < FrameList.size() - 1)
		{
			CurrentFrame++;
		}
		else
		{
			CurrentFrame = 0;
		}
		lastframeTime = time;
	}
}
