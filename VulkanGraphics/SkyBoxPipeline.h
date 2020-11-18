#pragma once
#include "GraphicsPipeline.h"

class SkyBoxPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& engine);
	void CreateShaderPipeLine(VulkanEngine& engine, const VkRenderPass& renderPass);
public:
	SkyBoxPipeline();
	SkyBoxPipeline(VulkanEngine& engine, const VkRenderPass& renderPass);
	~SkyBoxPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& engine, const VkRenderPass& renderPass);
};