#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include "VulkanEngine.h"
#include "RenderedDepthTexture.h"
#include "FrameBufferRenderingPipeline.h"

class FrameBufferRenderPass
{
private:
	VkRenderPass RenderPass;

	void CreateRenderPass(VulkanEngine& engine);
	void CreateRendererFramebuffers(VulkanEngine& engine);

public:
	FrameBufferRenderPass();
	FrameBufferRenderPass(VulkanEngine& engine);
	~FrameBufferRenderPass();

	std::vector<VkFramebuffer> SwapChainFramebuffers;

	std::shared_ptr<FrameBufferRenderingPipeline> frameBufferPipeline;

	void CMDDraw(VulkanEngine& engine);
	void UpdateSwapChain(VulkanEngine& engine);
	void Destroy(VulkanEngine& engine);

	VkRenderPass GetRenderPass() { return RenderPass; }
};
