#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Frame2D.h"

class Animation2D
{
private:
	unsigned int AnimationID;
	float lastframeTime;
	unsigned int CurrentFrame;
	float TimeBetweenFrames;
	std::vector<glm::vec2> UVOffsetList;
	std::vector<Frame2D> FrameList;

public:
	Animation2D();
	Animation2D(std::vector<Frame2D> frameList, float timeBetweenFrames, unsigned int animationID);
	void Update();

	unsigned int GetAnimationID() { return AnimationID; }
	Frame2D GetCurrentFrame() { return FrameList[CurrentFrame]; }
};