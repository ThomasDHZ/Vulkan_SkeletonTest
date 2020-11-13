#pragma once
#include "VulkanEngine.h"
#include "MainRenderPass.h"
#include "InterfaceRenderPass.h"
#include "Mesh.h"

class RenderManager
{
private:
	size_t currentFrame = 0;
	bool framebufferResized = false;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<RenderManager*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}
public:
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkCommandBuffer> ImGuiCommandBuffers;

	MainRenderPass mainRenderPass;
	InterfaceRenderPass interfaceRenderPass;

	RenderManager();
	RenderManager(VulkanEngine& vEngine);
	~RenderManager();

	void CMDBuffer(VulkanEngine& engine, VkCommandPool commnadPool, VkBuffer vertexBuffer, VkBuffer indexBuffer, std::vector<VkDescriptorSet> descriptorSets, uint32_t indices);
	void UpdateRenderManager(VulkanEngine& engine, GLFWwindow* window, Mesh& mesh);
	void Draw(VulkanEngine& engine, GLFWwindow* window, Mesh& mesh);
	void Destroy(VulkanEngine& engine);
};

