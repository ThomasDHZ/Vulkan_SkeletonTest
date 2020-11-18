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
        renderManager = RenderManager(vulkanEngine, window.GetWindowPtr());
        textureManager = std::make_shared<TextureManager>(vulkanEngine);
        ModelInfo = Model(vulkanEngine, textureManager, "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/Models/TestAnimModel/model.dae", renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout);

        renderManager.CMDBuffer(vulkanEngine, ModelInfo);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window.GetWindowPtr())) {
            glfwPollEvents();

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                //ImGui::Image(renderManager.sceneRenderPass.ColorTexture->ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
                //ImGui::Image(renderManager.sceneRenderPass.BloomTexture->ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
               // ImGui::Image(renderManager.shadowRenderPass.DepthTexture->ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
                textureManager->UpdateIMGUIVRAM();
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
        renderManager.Destroy(vulkanEngine);
        ModelInfo.Destroy(vulkanEngine);
        textureManager->UnloadAllTextures(vulkanEngine);
        vulkanEngine.Destory();
        window.CleanUp();
    }

    void recreateSwapChain() {
        renderManager.UpdateRenderManager(vulkanEngine, window.GetWindowPtr(), ModelInfo);
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