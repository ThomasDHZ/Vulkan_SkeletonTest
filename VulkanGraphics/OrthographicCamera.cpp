#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera() : Camera()
{
}

OrthographicCamera::OrthographicCamera(float width, float height) : Camera()
{
	Width = width;
	Height = height;
	Aspect = Width / Height;
	Zoom = 1.0f;

	Position = glm::vec3(0.0f);
	ViewScreenSize = glm::vec2(width, height);
	ProjectionMatrix = glm::ortho(-Aspect * Zoom, Aspect * Zoom, -1.0f * Zoom, 1.0f * Zoom, -1.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

OrthographicCamera::OrthographicCamera(glm::vec2 viewScreenSize) : Camera()
{
	Width = viewScreenSize.x;
	Height = viewScreenSize.y;
	Aspect = Width / Height;
	Zoom = 1.0f;

	Position = glm::vec3(0.0f);
	ViewScreenSize = viewScreenSize;
	ProjectionMatrix = glm::ortho(-Aspect * Zoom, Aspect * Zoom, -1.0f * Zoom, 1.0f * Zoom, -1.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

OrthographicCamera::OrthographicCamera(float width, float height, float zoom) : Camera()
{
	Width = width;
	Height = height;
	Aspect = Width / Height;
	Zoom = zoom;

	Position = glm::vec3(0.0f);
	ViewScreenSize = glm::vec2(width, height);
	ProjectionMatrix = glm::ortho(-ViewScreenSize.x * Zoom, ViewScreenSize.x * Zoom, -ViewScreenSize.y * Zoom, ViewScreenSize.y * Zoom, -1.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

OrthographicCamera::OrthographicCamera(glm::vec2 viewScreenSize, float zoom) : Camera()
{
	Width = viewScreenSize.x;
	Height = viewScreenSize.y;
	Aspect = Width / Height;
	Zoom = zoom;

	Position = glm::vec3(0.0f);
	ViewScreenSize = viewScreenSize;
	ProjectionMatrix = glm::ortho(-ViewScreenSize.x * Zoom, ViewScreenSize.x * Zoom, -ViewScreenSize.y * Zoom, ViewScreenSize.y * Zoom, -1.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

OrthographicCamera::OrthographicCamera(glm::vec2 viewScreenSize, glm::vec2 position)
{
	Width = viewScreenSize.x;
	Height = viewScreenSize.y;
	Aspect = Width / Height;
	Zoom = 1.0f;
	Position = glm::vec3(position, 0.0f);

	ViewScreenSize = viewScreenSize;
	ProjectionMatrix = glm::ortho(-Aspect * Zoom, Aspect * Zoom, -1.0f * Zoom, 1.0f * Zoom, -1.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

OrthographicCamera::OrthographicCamera(glm::vec2 viewScreenSize, glm::vec2 position, float zoom)
{
	Width = viewScreenSize.x;
	Height = viewScreenSize.y;
	Aspect = Width / Height;
	Zoom = zoom;
	Position = glm::vec3(position, 0.0f);;

	ViewScreenSize = viewScreenSize;
	ProjectionMatrix = glm::ortho(-ViewScreenSize.x * Zoom, ViewScreenSize.x * Zoom, -ViewScreenSize.y * Zoom, ViewScreenSize.y * Zoom, -1.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

OrthographicCamera::OrthographicCamera(glm::vec2 viewScreenSize, glm::vec3 position)
{
	Width = viewScreenSize.x;
	Height = viewScreenSize.y;
	Aspect = Width / Height;
	Zoom = 1.0f;
	Position = position;

	ViewScreenSize = viewScreenSize;
	ProjectionMatrix = glm::ortho(-Aspect * Zoom, Aspect * Zoom, -1.0f * Zoom, 1.0f * Zoom, -1.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

OrthographicCamera::OrthographicCamera(glm::vec2 viewScreenSize, glm::vec3 position, float zoom)
{
	Width = viewScreenSize.x;
	Height = viewScreenSize.y;
	Aspect = Width / Height;
	Zoom = zoom;
	Position = position;

	ViewScreenSize = viewScreenSize;
	ProjectionMatrix = glm::ortho(-ViewScreenSize.x * Zoom, ViewScreenSize.x * Zoom, -ViewScreenSize.y * Zoom, ViewScreenSize.y * Zoom, -1.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void OrthographicCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Zoom -= 5.0f * velocity;
	if (direction == BACKWARD)
		Zoom += 5.0f * velocity;
	if (direction == UP)
		Position.y -= 1.0f * velocity;
	if (direction == DOWN)
		Position.y += 1.0f * velocity;
	if (direction == LEFT)
		Position.x += 1.0f * velocity;
	if (direction == RIGHT)
		Position.x -= 1.0f * velocity;
}

void OrthographicCamera::Update(VulkanEngine& engine)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));
	ViewMatrix = glm::inverse(transform);

	const auto 	Aspect = engine.SwapChain.GetSwapChainResolution().width / (float)engine.SwapChain.GetSwapChainResolution().height;
	ProjectionMatrix = glm::ortho(-Aspect * Zoom, Aspect * Zoom, -1.0f * Zoom, 1.0f * Zoom, -1.0f, 1.0f);
	ViewScreenSize = glm::vec2((Aspect * Zoom) * 2, (1.0f * Zoom) * 2);
}