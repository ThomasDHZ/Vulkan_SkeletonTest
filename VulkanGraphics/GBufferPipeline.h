#pragma once
#include "GraphicsPipeline.h"

class GBufferPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	GBufferPipeline();
	GBufferPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~GBufferPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};

