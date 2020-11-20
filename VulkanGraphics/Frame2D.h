#pragma once
#include <glm/glm.hpp>
class Frame2D
{
private:
	float FrameTime = 0;
	glm::vec2 UVOffset = glm::vec2(0.0f, 0.0f);
	glm::vec2 UVScale = glm::vec2(1.0f, 1.0f);
	bool ReflectFrame = 0;
public:
	Frame2D();
	Frame2D(glm::vec2 uVOffset);
	Frame2D(glm::vec2 uVOffset, float frameTime);
	Frame2D(glm::vec2 uVOffset, glm::vec2 uVScale, float frameTime);
	~Frame2D();

	const float GetFrameTime() { return FrameTime; }
	const glm::vec2 GetUVOffset() { return UVOffset; }
	const glm::vec2 GetUVScale() { return UVScale; }
};