#pragma once
#include "GraphicsPipeline.h"

class DefferedRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	DefferedRenderingPipeline();
	DefferedRenderingPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~DefferedRenderingPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};

