#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include "VulkanEngine.h"
#include "RenderedDepthTexture.h"
#include "ShadowRenderingPipeline.h"

class ShadowRenderPass
{
private:
	VkRenderPass RenderPass;

	void CreateRenderPass(VulkanEngine& engine);
	void CreateRendererFramebuffers(VulkanEngine& engine);

public:
	ShadowRenderPass();
	ShadowRenderPass(VulkanEngine& engine);
	~ShadowRenderPass();

	std::vector<VkFramebuffer> SwapChainFramebuffers;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	std::shared_ptr<ShadowRenderingPipeline> shadowdRendereringPipeline;

	void UpdateSwapChain(VulkanEngine& engine);
	void Destroy(VulkanEngine& engine);

	VkRenderPass GetRenderPass() { return RenderPass; }
};
