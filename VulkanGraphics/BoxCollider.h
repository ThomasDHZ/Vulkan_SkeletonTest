#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>

class BoxCollider
{
private:

public:
	float Left;
	float Right;
	float Top;
	float Bottom;

	BoxCollider();
	BoxCollider(float left, float right, float top, float bottom);
	~BoxCollider();

	bool CollidesWith(const BoxCollider& OtherBox) const;
	bool CollidesWith(const BoxCollider& OtherBox, const glm::vec3 NewPos) const;
};
