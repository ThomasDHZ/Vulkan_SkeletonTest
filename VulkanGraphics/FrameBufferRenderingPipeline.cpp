#include "FrameBufferRenderingPipeline.h"
#include "Vertex.h"

FrameBufferRenderingPipeline::FrameBufferRenderingPipeline() : GraphicsPipeline()
{
}

FrameBufferRenderingPipeline::FrameBufferRenderingPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass) : GraphicsPipeline(renderer)
{
	CreateDescriptorSetLayout(renderer);
	CreateShaderPipeLine(renderer, renderPass);
}

FrameBufferRenderingPipeline::~FrameBufferRenderingPipeline()
{
}

void FrameBufferRenderingPipeline::CreateDescriptorSetLayout(VulkanEngine& renderer)
{
	std::array<DescriptorSetLayoutBindingInfo, 3> LayoutBindingInfo = {};

	LayoutBindingInfo[0].Binding = 0;
	LayoutBindingInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	LayoutBindingInfo[0].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	LayoutBindingInfo[1].Binding = 1;
	LayoutBindingInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	LayoutBindingInfo[1].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	LayoutBindingInfo[2].Binding = 2;
	LayoutBindingInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	LayoutBindingInfo[2].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	GraphicsPipeline::CreateDescriptorSetLayout(renderer, std::vector<DescriptorSetLayoutBindingInfo>(LayoutBindingInfo.begin(), LayoutBindingInfo.end()));
}

void FrameBufferRenderingPipeline::CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass)
{
    auto vertShaderCode = ReadShaderFile("shaders/FrameBufferShaderVert.spv");
    auto fragShaderCode = ReadShaderFile("shaders/FrameBufferShaderFrag.spv");

	VkShaderModule FrameBufferVertShaderModule = CreateShaderModule(renderer, vertShaderCode);
	VkShaderModule FrameBufferFragShaderModule = CreateShaderModule(renderer, fragShaderCode);

	VkPipelineShaderStageCreateInfo FrameBufferVertShaderStageInfo = {};
	FrameBufferVertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	FrameBufferVertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	FrameBufferVertShaderStageInfo.module = FrameBufferVertShaderModule;
	FrameBufferVertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo FrameBufferFragShaderStageInfo = {};
	FrameBufferFragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	FrameBufferFragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	FrameBufferFragShaderStageInfo.module = FrameBufferFragShaderModule;
	FrameBufferFragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo FrameBufferShaderStages[] = { FrameBufferVertShaderStageInfo, FrameBufferFragShaderStageInfo };

	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)renderer.SwapChain.GetSwapChainResolution().width;
	viewport.height = (float)renderer.SwapChain.GetSwapChainResolution().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = renderer.SwapChain.GetSwapChainResolution();

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

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo ColorBlending = {};
	ColorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	ColorBlending.logicOpEnable = VK_FALSE;
	ColorBlending.logicOp = VK_LOGIC_OP_COPY;
	ColorBlending.attachmentCount = 1;
	ColorBlending.pAttachments = &colorBlendAttachment;
	ColorBlending.blendConstants[0] = 0.0f;
	ColorBlending.blendConstants[1] = 0.0f;
	ColorBlending.blendConstants[2] = 0.0f;
	ColorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo FrameBufferPipelineLayoutInfo = {};
	FrameBufferPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	FrameBufferPipelineLayoutInfo.setLayoutCount = 1;
	FrameBufferPipelineLayoutInfo.pSetLayouts = &ShaderPipelineDescriptorLayout;

	GraphicsPipeline::CreatePipeLineLayout(renderer, FrameBufferPipelineLayoutInfo);

	VkGraphicsPipelineCreateInfo FrameBufferPipelineInfo = {};
	FrameBufferPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	FrameBufferPipelineInfo.stageCount = 2;
	FrameBufferPipelineInfo.pStages = FrameBufferShaderStages;
	FrameBufferPipelineInfo.pVertexInputState = &vertexInputInfo;
	FrameBufferPipelineInfo.pInputAssemblyState = &inputAssembly;
	FrameBufferPipelineInfo.pViewportState = &viewportState;
	FrameBufferPipelineInfo.pRasterizationState = &rasterizer;
	FrameBufferPipelineInfo.pMultisampleState = &multisampling;
	FrameBufferPipelineInfo.pDepthStencilState = &depthStencil;
	FrameBufferPipelineInfo.pColorBlendState = &ColorBlending;
	FrameBufferPipelineInfo.layout = ShaderPipelineLayout;
	FrameBufferPipelineInfo.renderPass = renderPass;
	FrameBufferPipelineInfo.subpass = 0;
	FrameBufferPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	GraphicsPipeline::CreatePipeLine(renderer, FrameBufferPipelineInfo);

	vkDestroyShaderModule(renderer.Device, FrameBufferFragShaderModule, nullptr);
	vkDestroyShaderModule(renderer.Device, FrameBufferVertShaderModule, nullptr);
}

void FrameBufferRenderingPipeline::UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass)
{
	vkDestroyPipeline(renderer.Device, ShaderPipeline, nullptr);
	vkDestroyPipelineLayout(renderer.Device, ShaderPipelineLayout, nullptr);

	ShaderPipeline = VK_NULL_HANDLE;
	ShaderPipelineLayout = VK_NULL_HANDLE;

	CreateShaderPipeLine(renderer, renderPass);
}
