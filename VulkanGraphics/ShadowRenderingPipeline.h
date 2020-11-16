#pragma once
#include "GraphicsPipeline.h"

class ShadowRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	ShadowRenderingPipeline();
	ShadowRenderingPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~ShadowRenderingPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};

