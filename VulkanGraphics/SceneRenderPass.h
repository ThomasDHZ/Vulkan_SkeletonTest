#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include "VulkanEngine.h"
#include "RenderedDepthTexture.h"
#include "SceneRenderingPipeline.h"
#include "RenderedColorTexture.h"
#include "SkyBoxPipeline.h"

class SceneRenderPass
{
private:
	VkRenderPass RenderPass;

	void CreateRenderPass(VulkanEngine& engine);
	void CreateRendererFramebuffers(VulkanEngine& engine);

public:
	SceneRenderPass();
	SceneRenderPass(VulkanEngine& engine);
	~SceneRenderPass();

	std::vector<VkFramebuffer> SwapChainFramebuffers;
	std::shared_ptr<RenderedColorTexture> ColorTexture;
	std::shared_ptr<RenderedColorTexture> BloomTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	std::shared_ptr<SceneRenderingPipeline> sceneRenderingPipeline;
	std::shared_ptr<SkyBoxPipeline> skyBoxPipeline;

	void UpdateSwapChain(VulkanEngine& engine);
	void Destroy(VulkanEngine& engine);

	VkRenderPass GetRenderPass() { return RenderPass; }
};