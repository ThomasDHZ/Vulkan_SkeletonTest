#include "InterfaceRenderPass.h"
#include <stdexcept>

InterfaceRenderPass::InterfaceRenderPass()
{
}

InterfaceRenderPass::InterfaceRenderPass(VulkanEngine& engine, GLFWwindow* window)
{
	CreateRenderPass(engine);
    CreateRendererFramebuffers(engine);

    ImGuiCommandBuffers.resize(engine.SwapChain.GetSwapChainImageCount());

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = engine.Instance;
    init_info.PhysicalDevice = engine.PhysicalDevice;
    init_info.Device = engine.Device;
    init_info.QueueFamily = 0;
    init_info.Queue = engine.GraphicsQueue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.Allocator = nullptr;
    init_info.MinImageCount = engine.SwapChain.GetSwapChainMinImageCount();
    init_info.ImageCount = engine.SwapChain.GetSwapChainImageCount();


    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = init_info.QueueFamily;

    if (vkCreateCommandPool(init_info.Device, &poolInfo, nullptr, &ImGuiCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics command pool!");
    }

    VkDescriptorPoolSize pool_sizes[] =
    {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;

    if (vkCreateDescriptorPool(init_info.Device, &pool_info, nullptr, &ImGuiDescriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

    VkCommandBufferAllocateInfo allocInfo2{};
    allocInfo2.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo2.commandPool = ImGuiCommandPool;
    allocInfo2.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo2.commandBufferCount = (uint32_t)ImGuiCommandBuffers.size();

    if (vkAllocateCommandBuffers(init_info.Device, &allocInfo2, ImGuiCommandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(window, true);
    init_info.DescriptorPool = ImGuiDescriptorPool;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, RenderPass);

    VkCommandBuffer command_buffer = VulkanBufferManager::beginSingleTimeCommands(engine.Device, ImGuiCommandPool);
    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
    VulkanBufferManager::endSingleTimeCommands(engine.Device, command_buffer, engine.GraphicsQueue, ImGuiCommandPool);
}

InterfaceRenderPass::~InterfaceRenderPass()
{
}

void InterfaceRenderPass::CreateRenderPass(VulkanEngine& engine)
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(engine.Device, &renderPassInfo, nullptr, &RenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void InterfaceRenderPass::CreateRendererFramebuffers(VulkanEngine& engine)
{
    SwapChainFramebuffers.resize(engine.SwapChain.GetSwapChainImageCount());

    for (size_t i = 0; i < engine.SwapChain.GetSwapChainImageCount(); i++) {
        std::array<VkImageView, 1> attachments = {
            engine.SwapChain.GetSwapChainImageViews()[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = RenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = engine.SwapChain.GetSwapChainResolution().width;
        framebufferInfo.height = engine.SwapChain.GetSwapChainResolution().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(engine.Device, &framebufferInfo, nullptr, &SwapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}


void InterfaceRenderPass::Draw(VulkanEngine& engine)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(ImGuiCommandBuffers[engine.DrawFrame], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = RenderPass;
    renderPassInfo.framebuffer = SwapChainFramebuffers[engine.DrawFrame];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = engine.SwapChain.GetSwapChainResolution();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();


    vkCmdBeginRenderPass(ImGuiCommandBuffers[engine.DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), ImGuiCommandBuffers[engine.DrawFrame]);
    vkCmdEndRenderPass(ImGuiCommandBuffers[engine.DrawFrame]);

    if (vkEndCommandBuffer(ImGuiCommandBuffers[engine.DrawFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void InterfaceRenderPass::UpdateSwapChain(VulkanEngine& engine)
{
    vkDestroyRenderPass(engine.Device, RenderPass, nullptr);
    RenderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(engine.Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }

    CreateRenderPass(engine);
    CreateRendererFramebuffers(engine);
}

void InterfaceRenderPass::Destroy(VulkanEngine& engine)
{
    vkDestroyRenderPass(engine.Device, RenderPass, nullptr);
    RenderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(engine.Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }

    vkDestroyDescriptorPool(engine.Device, ImGuiDescriptorPool, nullptr);
    vkFreeCommandBuffers(engine.Device, ImGuiCommandPool, static_cast<uint32_t>(ImGuiCommandBuffers.size()), ImGuiCommandBuffers.data());
    vkDestroyCommandPool(engine.Device, ImGuiCommandPool, nullptr);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}