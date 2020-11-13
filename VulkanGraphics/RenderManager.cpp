#include "RenderManager.h"
#include "ImGui/imgui_impl_vulkan.h"

RenderManager::RenderManager()
{
}

RenderManager::RenderManager(VulkanEngine& engine)
{

	mainRenderPass = MainRenderPass(engine);
	interfaceRenderPass = InterfaceRenderPass(engine);

    ImGuiCommandBuffers.resize(engine.SwapChain.GetSwapChainImageCount());
}

RenderManager::~RenderManager()
{
}

void RenderManager::UpdateRenderManager(VulkanEngine& engine, GLFWwindow* window, Mesh& mesh)
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(engine.Device);

    for (auto imageView : engine.SwapChain.GetSwapChainImageViews()) {
        vkDestroyImageView(engine.Device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(engine.Device, engine.SwapChain.GetSwapChain(), nullptr);

    engine.SwapChain.UpdateSwapChain(window, engine.Device, engine.PhysicalDevice, engine.Surface);

    mainRenderPass.UpdateSwapChain(engine);
    interfaceRenderPass.UpdateSwapChain(engine);

   CMDBuffer(engine, mesh);
}

void RenderManager::CMDBuffer(VulkanEngine& engine, Mesh& mesh)
{
    commandBuffers.resize(mainRenderPass.SwapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = engine.GetRenderCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(engine.Device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = mainRenderPass.GetRenderPass();
        renderPassInfo.framebuffer = mainRenderPass.SwapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = engine.SwapChain.GetSwapChainResolution();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.0f, 0.0f, 0.2f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mainRenderPass.forwardRendereringPipeline->ShaderPipeline);

        VkBuffer vertexBuffers[] = { mesh.MeshVertex.GetVertexBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffers[i], mesh.MeshIndices.GetIndiceBuffer(), 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mainRenderPass.forwardRendereringPipeline->ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[i], 0, nullptr);

        vkCmdDrawIndexed(commandBuffers[i], mesh.MeshIndices.GetIndiceCount(), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void RenderManager::Draw(VulkanEngine& engine, GLFWwindow* window, Mesh& mesh)
{
    vkWaitForFences(engine.Device, 1, &engine.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    VkResult result = vkAcquireNextImageKHR(engine.Device, engine.SwapChain.GetSwapChain(), UINT64_MAX, engine.vulkanSemaphores[currentFrame].ImageAcquiredSemaphore, VK_NULL_HANDLE, &engine.DrawFrame);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        UpdateRenderManager(engine, window, mesh);
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }


    if (engine.imagesInFlight[engine.DrawFrame] != VK_NULL_HANDLE) {
        vkWaitForFences(engine.Device, 1, &engine.imagesInFlight[engine.DrawFrame], VK_TRUE, UINT64_MAX);
    }
    engine.imagesInFlight[engine.DrawFrame] = engine.inFlightFences[currentFrame];

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(ImGuiCommandBuffers[engine.DrawFrame], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = interfaceRenderPass.GetRenderPass();
    renderPassInfo.framebuffer = interfaceRenderPass.SwapChainFramebuffers[engine.DrawFrame];
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

    std::array<VkCommandBuffer, 2> submitCommandBuffers =
    { commandBuffers[engine.DrawFrame], ImGuiCommandBuffers[engine.DrawFrame] };

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { engine.vulkanSemaphores[currentFrame].ImageAcquiredSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
    submitInfo.pCommandBuffers = submitCommandBuffers.data();

    VkSemaphore signalSemaphores[] = { engine.vulkanSemaphores[currentFrame].RenderCompleteSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(engine.Device, 1, &engine.inFlightFences[currentFrame]);

    if (vkQueueSubmit(engine.GraphicsQueue, 1, &submitInfo, engine.inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { engine.SwapChain.GetSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &engine.DrawFrame;

    result = vkQueuePresentKHR(engine.PresentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        UpdateRenderManager(engine, window, mesh);
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void RenderManager::Destroy(VulkanEngine& engine)
{
	mainRenderPass.Destroy(engine);
	interfaceRenderPass.Destroy(engine);
}
