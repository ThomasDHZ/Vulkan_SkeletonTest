#include "SkyBoxPipeline.h"
#include <array>
#include "Mesh.h"
#include "SkyboxMesh.h"
#include "Vertex.h"

SkyBoxPipeline::SkyBoxPipeline() : GraphicsPipeline()
{
}

SkyBoxPipeline::SkyBoxPipeline(VulkanEngine& engine, const VkRenderPass& renderPass) : GraphicsPipeline(engine)
{
	CreateDescriptorSetLayout(engine);
	CreateShaderPipeLine(engine, renderPass);
}

SkyBoxPipeline::~SkyBoxPipeline()
{
}

void SkyBoxPipeline::CreateDescriptorSetLayout(VulkanEngine& engine)
{
	std::array<DescriptorSetLayoutBindingInfo, 2> LayoutBindingInfo = {};

	LayoutBindingInfo[0].Binding = 0;
	LayoutBindingInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	LayoutBindingInfo[0].StageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	LayoutBindingInfo[1].Binding = 1;
	LayoutBindingInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	LayoutBindingInfo[1].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	GraphicsPipeline::CreateDescriptorSetLayout(engine, std::vector<DescriptorSetLayoutBindingInfo>(LayoutBindingInfo.begin(), LayoutBindingInfo.end()));
}

void SkyBoxPipeline::CreateShaderPipeLine(VulkanEngine& engine, const VkRenderPass& renderPass)
{
	auto SkyBoxvertShaderCode = ReadShaderFile("shaders/SkyBoxShaderVert.spv");
	auto SkyBoxfragShaderCode = ReadShaderFile("shaders/SkyBoxShaderFrag.spv");

	VkShaderModule SkyBoxvertShaderModule = CreateShaderModule(engine, SkyBoxvertShaderCode);
	VkShaderModule SkyBoxfragShaderModule = CreateShaderModule(engine, SkyBoxfragShaderCode);

	VkPipelineShaderStageCreateInfo SkyBoxvertShaderStageInfo = {};
	SkyBoxvertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	SkyBoxvertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	SkyBoxvertShaderStageInfo.module = SkyBoxvertShaderModule;
	SkyBoxvertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo SkyBoxfragShaderStageInfo = {};
	SkyBoxfragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	SkyBoxfragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	SkyBoxfragShaderStageInfo.module = SkyBoxfragShaderModule;
	SkyBoxfragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo SkyBoxshaderStages[] = { SkyBoxvertShaderStageInfo, SkyBoxfragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo SkyBoxvertexInputInfo = {};
	SkyBoxvertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto SkyBoxbindingDescription = Vertex::GetBindingDescription();
	auto SkyBoxattributeDescriptions = Vertex::GetAttributeDescriptions();

	SkyBoxvertexInputInfo.vertexBindingDescriptionCount = 1;
	SkyBoxvertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(SkyBoxattributeDescriptions.size());
	SkyBoxvertexInputInfo.pVertexBindingDescriptions = &SkyBoxbindingDescription;
	SkyBoxvertexInputInfo.pVertexAttributeDescriptions = SkyBoxattributeDescriptions.data();

	VkPipelineDepthStencilStateCreateInfo SkyBoxdepthStencil = {};
	SkyBoxdepthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	SkyBoxdepthStencil.depthTestEnable = VK_TRUE;
	SkyBoxdepthStencil.depthWriteEnable = VK_TRUE;
	SkyBoxdepthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	SkyBoxdepthStencil.depthBoundsTestEnable = VK_FALSE;
	SkyBoxdepthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)engine.SwapChain.GetSwapChainResolution().width;
	viewport.height = (float)engine.SwapChain.GetSwapChainResolution().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = engine.SwapChain.GetSwapChainResolution();

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo SkyBoxpipelineLayoutInfo = {};
	SkyBoxpipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	SkyBoxpipelineLayoutInfo.setLayoutCount = 1;
	SkyBoxpipelineLayoutInfo.pSetLayouts = &ShaderPipelineDescriptorLayout;

	GraphicsPipeline::CreatePipeLineLayout(engine, SkyBoxpipelineLayoutInfo);

	VkGraphicsPipelineCreateInfo SkyBoxpipelineInfo = {};
	SkyBoxpipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	SkyBoxpipelineInfo.stageCount = 2;
	SkyBoxpipelineInfo.pStages = SkyBoxshaderStages;
	SkyBoxpipelineInfo.pVertexInputState = &SkyBoxvertexInputInfo;
	SkyBoxpipelineInfo.pInputAssemblyState = &inputAssembly;
	SkyBoxpipelineInfo.pViewportState = &viewportState;
	SkyBoxpipelineInfo.pRasterizationState = &rasterizer;
	SkyBoxpipelineInfo.pMultisampleState = &multisampling;
	SkyBoxpipelineInfo.pDepthStencilState = &SkyBoxdepthStencil;
	SkyBoxpipelineInfo.pColorBlendState = &colorBlending;
	SkyBoxpipelineInfo.layout = ShaderPipelineLayout;
	SkyBoxpipelineInfo.renderPass = renderPass;
	SkyBoxpipelineInfo.subpass = 0;
	SkyBoxpipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	GraphicsPipeline::CreatePipeLine(engine, SkyBoxpipelineInfo);

	vkDestroyShaderModule(engine.Device, SkyBoxfragShaderModule, nullptr);
	vkDestroyShaderModule(engine.Device, SkyBoxvertShaderModule, nullptr);
}

void SkyBoxPipeline::UpdateGraphicsPipeLine(VulkanEngine& engine, const VkRenderPass& renderPass)
{
	vkDestroyPipeline(engine.Device, ShaderPipeline, nullptr);
	vkDestroyPipelineLayout(engine.Device, ShaderPipelineLayout, nullptr);

	ShaderPipeline = VK_NULL_HANDLE;
	ShaderPipelineLayout = VK_NULL_HANDLE;

	CreateShaderPipeLine(engine, renderPass);
}