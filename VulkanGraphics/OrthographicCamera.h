#pragma once
#include "Camera.h"

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera();
	OrthographicCamera(float width, float height);
	OrthographicCamera(float width, float height, float zoom);
	OrthographicCamera(glm::vec2 viewScreenSize);
	OrthographicCamera(glm::vec2 viewScreenSize, float zoom);
	OrthographicCamera(glm::vec2 viewScreenSize, glm::vec2 position);
	OrthographicCamera(glm::vec2 viewScreenSize, glm::vec2 position, float zoom);
	OrthographicCamera(glm::vec2 viewScreenSize, glm::vec3 position);
	OrthographicCamera(glm::vec2 viewScreenSize, glm::vec3 position, float zoom);

	void Update(VulkanEngine& engine) override;
	void ProcessKeyboard(Camera_Movement direction, float deltaTime) override;
};