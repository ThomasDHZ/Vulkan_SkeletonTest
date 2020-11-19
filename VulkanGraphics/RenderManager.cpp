#include "RenderManager.h"
#include "ImGui/imgui_impl_vulkan.h"

RenderManager::RenderManager()
{
}

RenderManager::RenderManager(VulkanEngine& engine, GLFWwindow* window)
{
    interfaceRenderPass = InterfaceRenderPass(engine, window);
    mainRenderPass = MainRenderPass(engine);
    sceneRenderPass = SceneRenderPass(engine);
    frameBufferRenderPass = FrameBufferRenderPass(engine);
    shadowRenderPass = ShadowRenderPass(engine);

    frameBuffer = FrameBufferMesh(engine, sceneRenderPass.ColorTexture, frameBufferRenderPass.frameBufferPipeline->ShaderPipelineDescriptorLayout);
}

RenderManager::~RenderManager()
{
}

void RenderManager::UpdateRenderManager(VulkanEngine& engine, GLFWwindow* window, std::vector<Model>& ModelList, SkyBoxMesh& skybox)
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
    sceneRenderPass.UpdateSwapChain(engine);
    frameBufferRenderPass.UpdateSwapChain(engine);
    shadowRenderPass.UpdateSwapChain(engine);
    interfaceRenderPass.UpdateSwapChain(engine);

    frameBuffer.UpdateSwapChain(engine, sceneRenderPass.ColorTexture, frameBufferRenderPass.frameBufferPipeline->ShaderPipelineDescriptorLayout);

    CMDBuffer(engine, ModelList, skybox);
}

void RenderManager::CMDBuffer(VulkanEngine& engine, std::vector<Model>& ModelList, SkyBoxMesh& skybox)
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
     //   MainRenderCMDBuffer(engine, mesh, skybox, i);
        SceneRenderCMDBuffer(engine, ModelList, skybox, i);
        FrameBufferRenderCMDBuffer(engine, i);
        ShadowRenderCMDBuffer(engine, ModelList, i);
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void RenderManager::UpdateCommandBuffer(VulkanEngine& engine, std::vector<Model>& ModelList, SkyBoxMesh& skybox)
{
    CMDBuffer(engine, ModelList, skybox);
}

void RenderManager::Draw(VulkanEngine& engine, GLFWwindow* window, std::vector<Model>& ModelList, SkyBoxMesh& skybox)
{
    vkWaitForFences(engine.Device, 1, &engine.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    VkResult result = vkAcquireNextImageKHR(engine.Device, engine.SwapChain.GetSwapChain(), UINT64_MAX, engine.vulkanSemaphores[currentFrame].ImageAcquiredSemaphore, VK_NULL_HANDLE, &engine.DrawFrame);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        UpdateRenderManager(engine, window, ModelList, skybox);
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }


    if (engine.imagesInFlight[engine.DrawFrame] != VK_NULL_HANDLE) {
        vkWaitForFences(engine.Device, 1, &engine.imagesInFlight[engine.DrawFrame], VK_TRUE, UINT64_MAX);
    }
    engine.imagesInFlight[engine.DrawFrame] = engine.inFlightFences[currentFrame];

    interfaceRenderPass.Draw(engine);

    std::array<VkCommandBuffer, 2> submitCommandBuffers =
    { commandBuffers[engine.DrawFrame], interfaceRenderPass.ImGuiCommandBuffers[engine.DrawFrame] };

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
        UpdateRenderManager(engine, window, ModelList, skybox);
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void RenderManager::Destroy(VulkanEngine& engine)
{
    frameBuffer.Destory(engine);

	mainRenderPass.Destroy(engine);
    sceneRenderPass.Destroy(engine);
    shadowRenderPass.Destroy(engine);
    frameBufferRenderPass.Destroy(engine);
	interfaceRenderPass.Destroy(engine);
}

void RenderManager::MainRenderCMDBuffer(VulkanEngine& engine, std::vector<Model>& ModelList, SkyBoxMesh& skybox, int SwapBufferImageIndex)
{
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = mainRenderPass.GetRenderPass();
    renderPassInfo.framebuffer = mainRenderPass.SwapChainFramebuffers[SwapBufferImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = engine.SwapChain.GetSwapChainResolution();
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[SwapBufferImageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    skybox.Draw(commandBuffers[SwapBufferImageIndex], mainRenderPass.skyBoxPipeline, SwapBufferImageIndex);
    for (auto model : ModelList)
    {
        if (model.GetRenderFlags() & RenderDrawFlags::RenderWireFrame)
        {
            model.Draw(commandBuffers[SwapBufferImageIndex], mainRenderPass.wireFrameRendereringPipeline, SwapBufferImageIndex);
        }
        if (model.GetRenderFlags() & RenderDrawFlags::RenderNormally)
        {
            model.Draw(commandBuffers[SwapBufferImageIndex], mainRenderPass.forwardRendereringPipeline, SwapBufferImageIndex);
        }
    }
    vkCmdEndRenderPass(commandBuffers[SwapBufferImageIndex]);
}

void RenderManager::SceneRenderCMDBuffer(VulkanEngine& engine, std::vector<Model>& ModelList, SkyBoxMesh& skybox, int SwapBufferImageIndex)
{
    std::array<VkClearValue, 3> clearValues{};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[2].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = sceneRenderPass.GetRenderPass();
    renderPassInfo.framebuffer = sceneRenderPass.SwapChainFramebuffers[SwapBufferImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = engine.SwapChain.GetSwapChainResolution();
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[SwapBufferImageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    skybox.Draw(commandBuffers[SwapBufferImageIndex], sceneRenderPass.skyBoxPipeline, SwapBufferImageIndex);
    for (auto model : ModelList)
    {
        if (model.GetRenderFlags() & RenderDrawFlags::RenderWireFrame)
        {
            model.Draw(commandBuffers[SwapBufferImageIndex], mainRenderPass.wireFrameRendereringPipeline, SwapBufferImageIndex);
        }
        if (model.GetRenderFlags() & RenderDrawFlags::RenderNormally)
        {
            model.Draw(commandBuffers[SwapBufferImageIndex], sceneRenderPass.sceneRenderingPipeline, SwapBufferImageIndex);
        }
    }
    vkCmdEndRenderPass(commandBuffers[SwapBufferImageIndex]);
}

void RenderManager::FrameBufferRenderCMDBuffer(VulkanEngine& engine, int SwapBufferImageIndex)
{
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = frameBufferRenderPass.GetRenderPass();
    renderPassInfo.framebuffer = frameBufferRenderPass.SwapChainFramebuffers[SwapBufferImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = engine.SwapChain.GetSwapChainResolution();
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[SwapBufferImageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    frameBuffer.Draw(commandBuffers[SwapBufferImageIndex], frameBufferRenderPass.frameBufferPipeline, SwapBufferImageIndex);
    vkCmdEndRenderPass(commandBuffers[SwapBufferImageIndex]);
}

void RenderManager::ShadowRenderCMDBuffer(VulkanEngine& engine, std::vector<Model>& ModelList, int SwapBufferImageIndex)
{
    std::array<VkClearValue, 1> clearValues{};
    clearValues[0].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassInfo2{};
    renderPassInfo2.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo2.renderPass = shadowRenderPass.GetRenderPass();
    renderPassInfo2.framebuffer = shadowRenderPass.SwapChainFramebuffers[SwapBufferImageIndex];
    renderPassInfo2.renderArea.offset = { 0, 0 };
    renderPassInfo2.renderArea.extent = engine.SwapChain.GetSwapChainResolution();
    renderPassInfo2.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo2.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[SwapBufferImageIndex], &renderPassInfo2, VK_SUBPASS_CONTENTS_INLINE);
    for (auto model : ModelList)
    {
        if (model.GetRenderFlags() & RenderDrawFlags::RenderShadow)
        {
            model.Draw(commandBuffers[SwapBufferImageIndex], shadowRenderPass.shadowdRendereringPipeline, SwapBufferImageIndex);
        }
    }
    vkCmdEndRenderPass(commandBuffers[SwapBufferImageIndex]);
}
