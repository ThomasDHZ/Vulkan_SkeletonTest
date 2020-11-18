#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include "VulkanEngine.h"
#include "RenderedDepthTexture.h"
#include "ForwardRenderingPipeline.h"

class TextureRenderer
{
private:
	VkRenderPass RenderPass;

	void CreateRenderPass(VulkanEngine& engine);
	void CreateRendererFramebuffers(VulkanEngine& engine);

public:
	TextureRenderer();
	TextureRenderer(VulkanEngine& engine);
	~TextureRenderer();

	std::vector<VkFramebuffer> SwapChainFramebuffers;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	std::shared_ptr<ForwardRenderingPipeline> forwardRendereringPipeline;

	void UpdateSwapChain(VulkanEngine& engine);
	void Destroy(VulkanEngine& engine);

	VkRenderPass GetRenderPass() { return RenderPass; }
};
