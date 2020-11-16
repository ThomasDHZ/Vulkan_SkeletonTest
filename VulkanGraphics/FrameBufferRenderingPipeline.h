#pragma once
#include "GraphicsPipeline.h"

class FrameBufferRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	FrameBufferRenderingPipeline();
	FrameBufferRenderingPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~FrameBufferRenderingPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};

