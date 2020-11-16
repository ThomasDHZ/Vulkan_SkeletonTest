#pragma once
#include "VulkanEngine.h"
#include "MainRenderPass.h"
#include "InterfaceRenderPass.h"
#include "Mesh.h"
#include "Model.h"
#include "SceneRenderPass.h"
#include "ShadowRenderPass.h"
#include "FrameBufferRenderPass.h"

class RenderManager
{
private:
	size_t currentFrame = 0;
	bool framebufferResized = false;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<RenderManager*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	void MainRenderCMDBuffer(VulkanEngine& engine, Model& model, int SwapBufferImageIndex);
	void SceneRenderCMDBuffer(VulkanEngine& engine, Model& model, int SwapBufferImageIndex);
	void ShadowRenderCMDBuffer(VulkanEngine& engine, Model& model, int SwapBufferImageIndex);

public:
	std::vector<VkCommandBuffer> commandBuffers;

	MainRenderPass mainRenderPass;
	SceneRenderPass sceneRenderPass;
	FrameBufferRenderPass frameBufferRenderPass;
	ShadowRenderPass shadowRenderPass;
	InterfaceRenderPass interfaceRenderPass;

	RenderManager();
	RenderManager(VulkanEngine& vEngine, GLFWwindow* window);
	~RenderManager();

	void CMDBuffer(VulkanEngine& engine, Model& mesh);
	void UpdateRenderManager(VulkanEngine& engine, GLFWwindow* window, Model& mesh);
	void Draw(VulkanEngine& engine, GLFWwindow* window, Model& mesh);
	void Destroy(VulkanEngine& engine);
};

