#pragma once
#include "GraphicsPipeline.h"

class WireFrameRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	WireFrameRenderingPipeline();
	WireFrameRenderingPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~WireFrameRenderingPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};

