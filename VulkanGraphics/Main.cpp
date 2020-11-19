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
#include "SkyBoxMesh.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class HelloTriangleApplication {
public:
    void run() 
    {
        initVulkan();
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

    std::vector<Mesh> ModelList;
    SkyBoxMesh Skybox;

    void initVulkan() 
    {
        window = VulkanWindow(800, 600, "Vulkan Engine");
        vulkanEngine = VulkanEngine(window.GetWindowPtr());
        renderManager = RenderManager(vulkanEngine, window.GetWindowPtr());
        textureManager = std::make_shared<TextureManager>(vulkanEngine);
       
        MeshTextures meshTextures;
        meshTextures.DiffuseMap = DefaultTexture;
        meshTextures.SpecularMap = DefaultTexture;
        meshTextures.NormalMap = DefaultTexture;
        meshTextures.AlphaMap = DefaultTexture;
        meshTextures.DepthMap = DefaultTexture;
        meshTextures.EmissionMap = DefaultTexture;
        meshTextures.ReflectionMap = DefaultTexture;
        meshTextures.CubeMap[0] = "C:/Users/dhz/source/repos/Vulkan_SkeletonTest/Vulkan_SkeletonTest/VulkanGraphics/texture/skybox/left.jpg";
        meshTextures.CubeMap[1] = "C:/Users/dhz/source/repos/Vulkan_SkeletonTest/Vulkan_SkeletonTest/VulkanGraphics/texture/skybox/right.jpg";
        meshTextures.CubeMap[2] = "C:/Users/dhz/source/repos/Vulkan_SkeletonTest/Vulkan_SkeletonTest/VulkanGraphics/texture/skybox/top.jpg";
        meshTextures.CubeMap[3] = "C:/Users/dhz/source/repos/Vulkan_SkeletonTest/Vulkan_SkeletonTest/VulkanGraphics/texture/skybox/bottom.jpg";
        meshTextures.CubeMap[4] = "C:/Users/dhz/source/repos/Vulkan_SkeletonTest/Vulkan_SkeletonTest/VulkanGraphics/texture/skybox/back.jpg";
        meshTextures.CubeMap[5] = "C:/Users/dhz/source/repos/Vulkan_SkeletonTest/Vulkan_SkeletonTest/VulkanGraphics/texture/skybox/front.jpg";


        camera = std::make_shared<PerspectiveCamera>(PerspectiveCamera(glm::vec2(vulkanEngine.SwapChain.GetSwapChainResolution().width / (float)vulkanEngine.SwapChain.GetSwapChainResolution().height), glm::vec3(0.0f)));
        Model model = Model(vulkanEngine, textureManager, "C:/Users/dhz/source/repos/Vulkan_SkeletonTest/Vulkan_SkeletonTest/VulkanGraphics/Models/TestAnimModel/model.dae", renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout, RenderDrawFlags::RenderNormally | RenderDrawFlags::RenderShadow);
        
        ModelList.emplace_back(Mesh(vulkanEngine, textureManager, model.SubMeshList[0], renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout, RenderDrawFlags::RenderNormally | RenderDrawFlags::RenderShadow));
        ModelList.emplace_back(Mesh(vulkanEngine, textureManager, model.SubMeshList[0], renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout, RenderDrawFlags::RenderNormally | RenderDrawFlags::RenderShadow));
        ModelList.emplace_back(Mesh(vulkanEngine, textureManager, model.SubMeshList[0], renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout, RenderDrawFlags::RenderNormally | RenderDrawFlags::RenderShadow));
        ModelList.emplace_back(Mesh(vulkanEngine, textureManager, model.SubMeshList[0], renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout, RenderDrawFlags::RenderNormally | RenderDrawFlags::RenderShadow));

        ModelList[0].SetPosition3D(glm::vec3(1.0f, 5.0f, 0.0f));
        ModelList[1].SetPosition3D(glm::vec3(2.0f, 4.0f, 0.0f));
        ModelList[2].SetPosition3D(glm::vec3(3.0f, 3.0f, 0.0f));
        ModelList[2].SetPosition3D(glm::vec3(3.0f, 3.0f, 0.0f));

        Skybox = SkyBoxMesh(vulkanEngine, textureManager, renderManager.mainRenderPass.skyBoxPipeline->ShaderPipelineDescriptorLayout, meshTextures);
        renderManager.CMDBuffer(vulkanEngine, ModelList, Skybox);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window.GetWindowPtr())) {
            glfwPollEvents();

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::CheckboxFlags("Normally", &ModelList[0].RenderFlags, RenderDrawFlags::RenderNormally);
                ImGui::CheckboxFlags("WireFrame", &ModelList[0].RenderFlags, RenderDrawFlags::RenderWireFrame);
                ImGui::CheckboxFlags("Shadow", &ModelList[0].RenderFlags, RenderDrawFlags::RenderShadow);

                ImGui::CheckboxFlags("Normally2", &ModelList[1].RenderFlags, RenderDrawFlags::RenderNormally);
                ImGui::CheckboxFlags("WireFrame2", &ModelList[1].RenderFlags, RenderDrawFlags::RenderWireFrame);
                ImGui::CheckboxFlags("Shadow2", &ModelList[1].RenderFlags, RenderDrawFlags::RenderShadow);

                ImGui::CheckboxFlags("Normally3", &ModelList[2].RenderFlags, RenderDrawFlags::RenderNormally);
                ImGui::CheckboxFlags("WireFrame3", &ModelList[2].RenderFlags, RenderDrawFlags::RenderWireFrame);
                ImGui::CheckboxFlags("Shadow3", &ModelList[2].RenderFlags, RenderDrawFlags::RenderShadow);

                ImGui::CheckboxFlags("Normally4", &ModelList[3].RenderFlags, RenderDrawFlags::RenderNormally);
                ImGui::CheckboxFlags("WireFrame4", &ModelList[3].RenderFlags, RenderDrawFlags::RenderWireFrame);
                ImGui::CheckboxFlags("Shadow4", &ModelList[3].RenderFlags, RenderDrawFlags::RenderShadow);

                ImGui::Image(renderManager.sceneRenderPass.ColorTexture->ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
                ImGui::Image(renderManager.sceneRenderPass.BloomTexture->ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
                ImGui::Image(renderManager.shadowRenderPass.DepthTexture->ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
                textureManager->UpdateIMGUIVRAM();
            }
            ImGui::Render();

            renderManager.UpdateCommandBuffer(vulkanEngine, ModelList, Skybox);
            drawFrame();
            mouse.Update(window.GetWindowPtr(), camera);
            keyboard.Update(window.GetWindowPtr(), camera);
        }

        vkDeviceWaitIdle(vulkanEngine.Device);
    }


    void cleanup() 
    {
        renderManager.Destroy(vulkanEngine);
        for (auto model : ModelList)
        {
           // model.Destroy(vulkanEngine);
        }
        Skybox.Destory(vulkanEngine);
        textureManager->UnloadAllTextures(vulkanEngine);
        vulkanEngine.Destory();
        window.CleanUp();
    }

    void updateUniformBuffer(uint32_t currentImage) 
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        LightBufferObject light = {};
        camera->Update(vulkanEngine);
        for (auto& model : ModelList)
        {
            model.Update(vulkanEngine, camera, light);
        }
        Skybox.UpdateUniformBuffer(vulkanEngine, camera);
    }

    void drawFrame() 
    {
        updateUniformBuffer(vulkanEngine.DrawFrame);
        renderManager.Draw(vulkanEngine, window.GetWindowPtr(), ModelList, Skybox);
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

