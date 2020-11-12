#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>
#include "vertex.h"
#include "PerspectiveCamera.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Model.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"
#include "InterfaceRenderPass.h"
#include "VulkanWindow.h"
#include "VulkanEngine.h"
#include "MainRenderPass.h"
#include "RenderManager.h"
#include "ForwardRenderingPipeline.h"
#include "Texuture2D.h"
#include "Mesh.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;



#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif



struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
};

std::vector<std::shared_ptr<Bone>> BoneList;
Animation3D animation;

class HelloTriangleApplication {
public:
    void run() 
    {
        initVulkan();
        camera = std::make_shared<PerspectiveCamera>(PerspectiveCamera(glm::vec2(vulkanEngine.SwapChain.GetSwapChainResolution().width / (float)vulkanEngine.SwapChain.GetSwapChainResolution().height), glm::vec3(0.0f)));
        mainLoop();
        cleanup();
    }

private:
    VulkanWindow window;
    VulkanEngine vulkanEngine;

    std::shared_ptr<PerspectiveCamera> camera;
    Keyboard keyboard;
    Mouse mouse;

    Model ModelInfo;
    Mesh mesh;


    Texture2D texture;

    size_t currentFrame = 0;

    RenderManager renderManager;
    ForwardRenderingPipeline pipeline;
  
    VkDescriptorPool ImGuiDescriptorPool;
    VkCommandPool ImGuiCommandPool;
    std::vector<VkCommandBuffer> ImGuiCommandBuffers;

    bool framebufferResized = false;

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    void initVulkan() 
    {
        window = VulkanWindow(800, 600, "Vulkan Engine");
        vulkanEngine = VulkanEngine(window.GetWindowPtr());
        renderManager = RenderManager(vulkanEngine);

      //  ModelInfo = Model("C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/Models/TestAnimModel/model.dae");
      //  vertices = ModelInfo.VertexList;
       // indices = ModelInfo.IndexList;
      //  BoneList = ModelInfo.BoneList;
        
        texture = Texture2D(vulkanEngine, VK_FORMAT_R8G8B8A8_UNORM, "C:/Users/dotha/source/repos/OpenGL_Skeleton_Test/OpenGL_Skeleton_Test/Model/TestAnimModel/diffuse.png", 0);
        mesh = Mesh(vulkanEngine, vertices, indices, renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout, texture);
        renderManager.CMDBuffer(vulkanEngine, vulkanEngine.GetRenderCommandPool(), mesh.MeshVertex.GetVertexBuffer(), mesh.MeshIndices.GetIndiceBuffer(), mesh.DescriptorSets, indices.size());

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = vulkanEngine.Instance;
        init_info.PhysicalDevice = vulkanEngine.PhysicalDevice;
        init_info.Device = vulkanEngine.Device;
        init_info.QueueFamily = 0;
        init_info.Queue = vulkanEngine.GraphicsQueue;
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.Allocator = nullptr;
        init_info.MinImageCount = vulkanEngine.SwapChain.GetSwapChainMinImageCount();
        init_info.ImageCount = vulkanEngine.SwapChain.GetSwapChainImageCount();

        ImGuiCommandBuffers.resize(init_info.ImageCount);

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

        ImGui_ImplGlfw_InitForVulkan(window.GetWindowPtr(), true);
        init_info.DescriptorPool = ImGuiDescriptorPool;
        init_info.CheckVkResultFn = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info, renderManager.interfaceRenderPass.GetRenderPass());

        VkCommandBuffer commandBuffer = beginSingleTimeCommands(); 
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        endSingleTimeCommands(commandBuffer);
    }

    static void check_vk_result(VkResult err)
    {
        if (err == 0) return;
        printf("VkResult %d\n", err);
        if (err < 0)
            abort();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window.GetWindowPtr())) {
            glfwPollEvents();

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
            ImGui::Render();

            drawFrame();
            mouse.Update(window.GetWindowPtr(), camera);
            keyboard.Update(window.GetWindowPtr(), camera);
        }

        vkDeviceWaitIdle(vulkanEngine.Device);
    }


    void cleanup() 
    {
        texture.Delete(vulkanEngine);

        vkDestroyDescriptorPool(vulkanEngine.Device, ImGuiDescriptorPool, nullptr);
        vkFreeCommandBuffers(vulkanEngine.Device, ImGuiCommandPool, static_cast<uint32_t>(ImGuiCommandBuffers.size()), ImGuiCommandBuffers.data());
        vkDestroyCommandPool(vulkanEngine.Device, ImGuiCommandPool, nullptr);

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        renderManager.Destroy(vulkanEngine);

        vkDestroyRenderPass(vulkanEngine.Device, renderManager.mainRenderPass.GetRenderPass(), nullptr);

        mesh.Destory(vulkanEngine);
        vulkanEngine.Destory();

        glfwDestroyWindow(window.GetWindowPtr());
        glfwTerminate();
    }

    void recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window.GetWindowPtr(), &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window.GetWindowPtr(), &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(vulkanEngine.Device);


        for (auto imageView : vulkanEngine.SwapChain.GetSwapChainImageViews()) {
            vkDestroyImageView(vulkanEngine.Device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(vulkanEngine.Device, vulkanEngine.SwapChain.GetSwapChain(), nullptr);


        vulkanEngine.SwapChain.UpdateSwapChain(window.GetWindowPtr(), vulkanEngine.Device, vulkanEngine.PhysicalDevice, vulkanEngine.Surface);

        renderManager.UpdateRenderManager(vulkanEngine);       
        renderManager.CMDBuffer(vulkanEngine, vulkanEngine.GetRenderCommandPool(), mesh.MeshVertex.GetVertexBuffer(), mesh.MeshIndices.GetIndiceBuffer(), mesh.DescriptorSets, indices.size());
    }

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(vulkanEngine.PhysicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throw std::runtime_error("failed to find supported format!");
    }

    VkFormat findDepthFormat() {
        return findSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

   
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(vulkanEngine.Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(vulkanEngine.Device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(vulkanEngine.Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(vulkanEngine.Device, buffer, bufferMemory, 0);
    }

    VkCommandBuffer beginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vulkanEngine.GetRenderCommandPool();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(vulkanEngine.Device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(vulkanEngine.GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(vulkanEngine.GraphicsQueue);

        vkFreeCommandBuffers(vulkanEngine.Device, vulkanEngine.GetRenderCommandPool(), 1, &commandBuffer);
    }

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        endSingleTimeCommands(commandBuffer);
    }

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(vulkanEngine.PhysicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }



    void updateUniformBuffer(uint32_t currentImage) 
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        LightBufferObject light = {};
        camera->Update();
        mesh.Update(vulkanEngine, camera, light);

        //VertexMatrixObject ubo{};
        //ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //ubo.view = camera->GetViewMatrix();
        //ubo.proj = camera->GetProjectionMatrix();
        //ubo.proj[1][1] *= -1;

        //void* data;
        //vkMapMemory(vulkanEngine.Device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
        //memcpy(data, &ubo, sizeof(ubo));
        //vkUnmapMemory(vulkanEngine.Device, uniformBuffersMemory[currentImage]);
        //static auto startTime = std::chrono::high_resolution_clock::now();

        //auto currentTime = std::chrono::high_resolution_clock::now();
        //float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        //camera->Update();
        //ModelInfo.Update();
    
        //UniformBufferObject ubo{};
        //ubo.model = glm::mat4(1.0f);
        //ubo.model = glm::rotate(ubo.model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //ubo.view = camera->GetViewMatrix();
        //ubo.proj = camera->GetProjectionMatrix();
        //ubo.proj[1][1] *= -1;

        //for (auto bone : BoneList)
        //{
        //    ubo.BoneTransform[bone->GetBoneID()] = bone->GetFinalBoneTransformMatrix();
        //}

        //void* data;
        //vkMapMemory(vulkanEngine.Device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
        //memcpy(data, &ubo, sizeof(ubo));
        //vkUnmapMemory(vulkanEngine.Device, uniformBuffersMemory[currentImage]);
    }

    void drawFrame() {
        vkWaitForFences(vulkanEngine.Device, 1, &vulkanEngine.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        VkResult result = vkAcquireNextImageKHR(vulkanEngine.Device, vulkanEngine.SwapChain.GetSwapChain(), UINT64_MAX, vulkanEngine.vulkanSemaphores[currentFrame].ImageAcquiredSemaphore, VK_NULL_HANDLE, &vulkanEngine.DrawFrame);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        updateUniformBuffer(vulkanEngine.DrawFrame);

        if (vulkanEngine.imagesInFlight[vulkanEngine.DrawFrame] != VK_NULL_HANDLE) {
            vkWaitForFences(vulkanEngine.Device, 1, &vulkanEngine.imagesInFlight[vulkanEngine.DrawFrame], VK_TRUE, UINT64_MAX);
        }
        vulkanEngine.imagesInFlight[vulkanEngine.DrawFrame] = vulkanEngine.inFlightFences[currentFrame];

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(ImGuiCommandBuffers[vulkanEngine.DrawFrame], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderManager.interfaceRenderPass.GetRenderPass();
        renderPassInfo.framebuffer = renderManager.interfaceRenderPass.SwapChainFramebuffers[vulkanEngine.DrawFrame];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = vulkanEngine.SwapChain.GetSwapChainResolution();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();


        vkCmdBeginRenderPass(ImGuiCommandBuffers[vulkanEngine.DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), ImGuiCommandBuffers[vulkanEngine.DrawFrame]);
        vkCmdEndRenderPass(ImGuiCommandBuffers[vulkanEngine.DrawFrame]);

        if (vkEndCommandBuffer(ImGuiCommandBuffers[vulkanEngine.DrawFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        std::array<VkCommandBuffer, 2> submitCommandBuffers =
        { renderManager.commandBuffers[vulkanEngine.DrawFrame], ImGuiCommandBuffers[vulkanEngine.DrawFrame] };

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { vulkanEngine.vulkanSemaphores[currentFrame].ImageAcquiredSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
        submitInfo.pCommandBuffers = submitCommandBuffers.data();

        VkSemaphore signalSemaphores[] = { vulkanEngine.vulkanSemaphores[currentFrame].RenderCompleteSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(vulkanEngine.Device, 1, &vulkanEngine.inFlightFences[currentFrame]);

        if (vkQueueSubmit(vulkanEngine.GraphicsQueue, 1, &submitInfo, vulkanEngine.inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { vulkanEngine.SwapChain.GetSwapChain() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &vulkanEngine.DrawFrame;

        result = vkQueuePresentKHR(vulkanEngine.PresentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
            framebufferResized = false;
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }


    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vulkanEngine.Surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

