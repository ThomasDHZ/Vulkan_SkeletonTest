//#pragma once
//#include "VulkanEngine.h"
//#include "RenderedDepthTexture.h"
//#include "RenderedGBufferNormalTexture.h"
//#include "RenderedGBufferAlbedoTexture.h"
//#include "RenderedGBufferPositionTexture.h"
//#include "DefferedRenderingPipeline.h"
//
//class DeferredRenderer
//{
//private:
//	VkRenderPass RenderPass;
//
//	void CreateRenderPass(VulkanEngine& engine);
//	void CreateRendererFramebuffers(VulkanEngine& engine);
//
//public:
//	DeferredRenderer();
//	DeferredRenderer(VulkanEngine& engine);
//	~DeferredRenderer();
//
//	std::vector<VkFramebuffer> SwapChainFramebuffers;
//	std::shared_ptr<RenderedGBufferPositionTexture> GPositionTexture;
//	std::shared_ptr<RenderedGBufferNormalTexture> GNormalTexture;
//	std::shared_ptr<RenderedGBufferAlbedoTexture> GAlbedoTexture;
//	std::shared_ptr<RenderedDepthTexture> GDepthTexture;
//
//	std::shared_ptr<DefferedRenderingPipeline> deferredRendereringPipeline;
//
//	void UpdateSwapChain(VulkanEngine& engine);
//	void Destroy(VulkanEngine& engine);
//
//	VkRenderPass GetRenderPass() { return RenderPass; }
//};
