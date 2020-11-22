#pragma once
#include "GraphicsPipeline.h"
class DebugLightRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	DebugLightRenderingPipeline();
	DebugLightRenderingPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~DebugLightRenderingPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};