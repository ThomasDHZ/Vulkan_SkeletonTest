#pragma once
#include "VulkanEngine.h"

enum RendererType
{
	RT_ForwardRenderer,
	RT_SceneRenderer,
	RT_TextureRenderer,
	RT_ShadowRenderer
};

struct DescriptorSetLayoutBindingInfo
{
	uint32_t Binding;
	VkDescriptorType DescriptorType;
	VkShaderStageFlags StageFlags;
};

class GraphicsPipeline
{
protected:
	std::vector<char> ReadShaderFile(const std::string& filename);
	VkShaderModule CreateShaderModule(VulkanEngine& renderer, const std::vector<char>& code);

	void CreateDescriptorSetLayout(VulkanEngine& renderer, std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo);
	void CreatePipeLineLayout(VulkanEngine& renderer, VkPipelineLayoutCreateInfo PipelineLayoutInfo);
	void CreatePipeLine(VulkanEngine& renderer, VkGraphicsPipelineCreateInfo PipeLineInfo);

public:
	VkPipelineLayout ShaderPipelineLayout;
	VkPipeline ShaderPipeline;
	VkDescriptorSetLayout ShaderPipelineDescriptorLayout;

	GraphicsPipeline();
	GraphicsPipeline(VulkanEngine& renderer);
	~GraphicsPipeline();

	void UpdateSwapChain(VulkanEngine& renderer);
	void Destroy(VulkanEngine& renderer);
};
