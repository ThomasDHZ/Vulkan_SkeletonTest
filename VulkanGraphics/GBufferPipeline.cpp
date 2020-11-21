#include "GBufferPipeline.h"
#include "Vertex.h"

GBufferPipeline::GBufferPipeline() : GraphicsPipeline()
{
}

GBufferPipeline::GBufferPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass) : GraphicsPipeline(renderer)
{
    CreateDescriptorSetLayout(renderer);
    CreateShaderPipeLine(renderer, renderPass);
}

GBufferPipeline::~GBufferPipeline()
{
}

void GBufferPipeline::CreateDescriptorSetLayout(VulkanEngine& renderer)
{
    std::array<DescriptorSetLayoutBindingInfo, 11> LayoutBindingInfo = {};

    LayoutBindingInfo[0].Binding = 0;
    LayoutBindingInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    LayoutBindingInfo[0].StageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    LayoutBindingInfo[1].Binding = 1;
    LayoutBindingInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    LayoutBindingInfo[1].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    LayoutBindingInfo[2].Binding = 2;
    LayoutBindingInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    LayoutBindingInfo[2].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    LayoutBindingInfo[3].Binding = 3;
    LayoutBindingInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    LayoutBindingInfo[3].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    LayoutBindingInfo[4].Binding = 4;
    LayoutBindingInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    LayoutBindingInfo[4].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    LayoutBindingInfo[5].Binding = 5;
    LayoutBindingInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    LayoutBindingInfo[5].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    LayoutBindingInfo[6].Binding = 6;
    LayoutBindingInfo[6].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    LayoutBindingInfo[6].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    LayoutBindingInfo[7].Binding = 7;
    LayoutBindingInfo[7].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    LayoutBindingInfo[7].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    LayoutBindingInfo[8].Binding = 8;
    LayoutBindingInfo[8].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    LayoutBindingInfo[8].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    LayoutBindingInfo[9].Binding = 9;
    LayoutBindingInfo[9].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    LayoutBindingInfo[9].StageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    LayoutBindingInfo[10].Binding = 10;
    LayoutBindingInfo[10].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    LayoutBindingInfo[10].StageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    GraphicsPipeline::CreateDescriptorSetLayout(renderer, std::vector<DescriptorSetLayoutBindingInfo>(LayoutBindingInfo.begin(), LayoutBindingInfo.end()));
}

void GBufferPipeline::CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass)
{
    auto vertShaderCode = ReadShaderFile("shaders/GBufferShaderVert.spv");
    auto fragShaderCode = ReadShaderFile("shaders/GBufferShaderFrag.spv");

    VkShaderModule vertShaderModule = CreateShaderModule(renderer, vertShaderCode);
    VkShaderModule fragShaderModule = CreateShaderModule(renderer, fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::GetBindingDescription();
    auto attributeDescriptions = Vertex::GetAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)renderer.SwapChain.GetSwapChainResolution().width;
    viewport.height = (float)renderer.SwapChain.GetSwapChainResolution().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = renderer.SwapChain.GetSwapChainResolution();

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    std::array<VkPipelineColorBlendAttachmentState, 4> ColorAttachment = {};
    ColorAttachment[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    ColorAttachment[0].blendEnable = VK_TRUE;
    ColorAttachment[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    ColorAttachment[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    ColorAttachment[0].colorBlendOp = VK_BLEND_OP_ADD;
    ColorAttachment[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    ColorAttachment[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    ColorAttachment[0].alphaBlendOp = VK_BLEND_OP_SUBTRACT;

    ColorAttachment[1].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    ColorAttachment[1].blendEnable = VK_TRUE;
    ColorAttachment[1].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    ColorAttachment[1].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    ColorAttachment[1].colorBlendOp = VK_BLEND_OP_ADD;
    ColorAttachment[1].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    ColorAttachment[1].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    ColorAttachment[1].alphaBlendOp = VK_BLEND_OP_SUBTRACT;

    ColorAttachment[2].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    ColorAttachment[2].blendEnable = VK_TRUE;
    ColorAttachment[2].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    ColorAttachment[2].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    ColorAttachment[2].colorBlendOp = VK_BLEND_OP_ADD;
    ColorAttachment[2].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    ColorAttachment[2].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    ColorAttachment[2].alphaBlendOp = VK_BLEND_OP_SUBTRACT;

    ColorAttachment[3].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    ColorAttachment[3].blendEnable = VK_TRUE;
    ColorAttachment[3].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    ColorAttachment[3].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    ColorAttachment[3].colorBlendOp = VK_BLEND_OP_ADD;
    ColorAttachment[3].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    ColorAttachment[3].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    ColorAttachment[3].alphaBlendOp = VK_BLEND_OP_SUBTRACT;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = static_cast<uint32_t>(ColorAttachment.size());
    colorBlending.pAttachments = ColorAttachment.data();
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &ShaderPipelineDescriptorLayout;

    if (vkCreatePipelineLayout(renderer.Device, &pipelineLayoutInfo, nullptr, &ShaderPipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = ShaderPipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(renderer.Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &ShaderPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(renderer.Device, fragShaderModule, nullptr);
    vkDestroyShaderModule(renderer.Device, vertShaderModule, nullptr);
}

void GBufferPipeline::UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass)
{
    vkDestroyPipeline(renderer.Device, ShaderPipeline, nullptr);
    vkDestroyPipelineLayout(renderer.Device, ShaderPipelineLayout, nullptr);

    ShaderPipeline = VK_NULL_HANDLE;
    ShaderPipelineLayout = VK_NULL_HANDLE;

    CreateShaderPipeLine(renderer, renderPass);
}
