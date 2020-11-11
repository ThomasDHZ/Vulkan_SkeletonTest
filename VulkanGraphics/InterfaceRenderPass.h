#pragma once
#include "VulkanEngine.h"
#include "RenderedDepthTexture.h"


class InterfaceRenderPass
{
	private:
		VkRenderPass RenderPass;
		

		void CreateRenderPass(VulkanEngine& engine);
		void CreateRendererFramebuffers(VulkanEngine& engine);

	public:
		InterfaceRenderPass();
		InterfaceRenderPass(VulkanEngine& engine);
		~InterfaceRenderPass();

		std::vector<VkFramebuffer> SwapChainFramebuffers;

		void UpdateSwapChain(VulkanEngine& engine);
		void Destroy(VulkanEngine& engine);

		VkRenderPass GetRenderPass() { return RenderPass; }
};

