#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include "VulkanEngine.h"
#include "RenderedDepthTexture.h"
#include "ForwardRenderingPipeline.h"
#include "WireFrameRenderingPipeline.h"
#include "SkyBoxPipeline.h"
#include "DebugLightRenderingPipeline.h"

class MainRenderPass
{
private:
	VkRenderPass RenderPass;

	void CreateRenderPass(VulkanEngine& engine);
	void CreateRendererFramebuffers(VulkanEngine& engine);

public:
	MainRenderPass();
	MainRenderPass(VulkanEngine& engine);
	~MainRenderPass();

	std::vector<VkFramebuffer> SwapChainFramebuffers;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	std::shared_ptr<ForwardRenderingPipeline> forwardRendereringPipeline;
	std::shared_ptr<SkyBoxPipeline> skyBoxPipeline;
	std::shared_ptr<WireFrameRenderingPipeline> wireFrameRendereringPipeline;
	std::shared_ptr<DebugLightRenderingPipeline> debugLightRenderingPipeline;

	void UpdateSwapChain(VulkanEngine& engine);
	void Destroy(VulkanEngine& engine);

	VkRenderPass GetRenderPass() { return RenderPass; }
};
