#pragma once
#include "VulkanEngine.h"
#include "MainRenderPass.h"
#include "InterfaceRenderPass.h"
#include "BaseMesh.h"

class RenderManager
{
private:
	size_t currentFrame = 0;
public:
	std::vector<VkCommandBuffer> commandBuffers;
	MainRenderPass mainRenderPass;
	InterfaceRenderPass interfaceRenderPass;

	RenderManager();
	RenderManager(VulkanEngine& vEngine);
	~RenderManager();

	void CMDBuffer(VulkanEngine& engine, VkBuffer vertexBuffer, VkBuffer indexBuffer, std::vector<VkDescriptorSet> descriptorSets, uint32_t indices);
	void UpdateRenderManager(VulkanEngine& engine);
	void Draw(VulkanEngine& vEngine);
	void Destroy(VulkanEngine& vEngine);
};

