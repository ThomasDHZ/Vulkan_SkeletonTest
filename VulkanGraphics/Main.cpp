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
    RenderManager renderManager;
    std::shared_ptr<TextureManager> textureManager;

    std::shared_ptr<PerspectiveCamera> camera;
    Keyboard keyboard;
    Mouse mouse;

    Model ModelInfo;

    VkDescriptorPool ImGuiDescriptorPool;
    VkCommandPool ImGuiCommandPool;


    void initVulkan() 
    {
        window = VulkanWindow(800, 600, "Vulkan Engine");
        vulkanEngine = VulkanEngine(window.GetWindowPtr());
        renderManager = RenderManager(vulkanEngine);
        textureManager = std::make_shared<TextureManager>(vulkanEngine);
        ModelInfo = Model(vulkanEngine, textureManager, "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/Models/TestAnimModel/model.dae", renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout);

       
        renderManager.CMDBuffer(vulkanEngine, ModelInfo);

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
        allocInfo2.commandBufferCount = (uint32_t)renderManager.ImGuiCommandBuffers.size();

        if (vkAllocateCommandBuffers(init_info.Device, &allocInfo2, renderManager.ImGuiCommandBuffers.data()) != VK_SUCCESS) {
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

        vkDestroyDescriptorPool(vulkanEngine.Device, ImGuiDescriptorPool, nullptr);
        vkFreeCommandBuffers(vulkanEngine.Device, ImGuiCommandPool, static_cast<uint32_t>(renderManager.ImGuiCommandBuffers.size()), renderManager.ImGuiCommandBuffers.data());
        vkDestroyCommandPool(vulkanEngine.Device, ImGuiCommandPool, nullptr);

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        renderManager.Destroy(vulkanEngine);

        vkDestroyRenderPass(vulkanEngine.Device, renderManager.mainRenderPass.GetRenderPass(), nullptr);

        ModelInfo.Destroy(vulkanEngine);
        textureManager->UnloadAllTextures(vulkanEngine);
        vulkanEngine.Destory();

        glfwDestroyWindow(window.GetWindowPtr());
        glfwTerminate();
    }

    void recreateSwapChain() {
        renderManager.UpdateRenderManager(vulkanEngine, window.GetWindowPtr(), ModelInfo);
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
        camera->Update(vulkanEngine);
        ModelInfo.Update(vulkanEngine, camera, light);
    }

    void drawFrame() 
    {
        updateUniformBuffer(vulkanEngine.DrawFrame);
        renderManager.Draw(vulkanEngine, window.GetWindowPtr(), ModelInfo);
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

