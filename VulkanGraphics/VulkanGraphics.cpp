#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define STB_IMAGE_IMPLEMENTATION

#include "VulkanGraphics.h"

VulkanGraphics::VulkanGraphics()
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

    ModelList.emplace_back(Model(vulkanEngine, textureManager, "C:/Users/dhz/source/repos/Vulkan_SkeletonTest/Vulkan_SkeletonTest/VulkanGraphics/Models/TestAnimModel/model.dae", renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout, RenderDrawFlags::RenderNormally | RenderDrawFlags::RenderShadow));
    ModelList.emplace_back(Model(vulkanEngine, textureManager, "C:/Users/dhz/source/repos/Vulkan_SkeletonTest/Vulkan_SkeletonTest/VulkanGraphics/Models/TestAnimModel/model.dae", renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout, RenderDrawFlags::RenderNormally | RenderDrawFlags::RenderShadow));
    ModelList.emplace_back(Model(vulkanEngine, textureManager, "C:/Users/dhz/source/repos/Vulkan_SkeletonTest/Vulkan_SkeletonTest/VulkanGraphics/Models/TestAnimModel/model.dae", renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout, RenderDrawFlags::RenderNormally | RenderDrawFlags::RenderShadow));
    ModelList.emplace_back(Model(vulkanEngine, textureManager, "C:/Users/dhz/source/repos/Vulkan_SkeletonTest/Vulkan_SkeletonTest/VulkanGraphics/Models/TestAnimModel/model.dae", renderManager.mainRenderPass.forwardRendereringPipeline->ShaderPipelineDescriptorLayout, RenderDrawFlags::RenderNormally | RenderDrawFlags::RenderShadow));

    ModelList[0].ModelPosition = glm::vec3(1.0f, 5.0f, 0.0f);
    ModelList[0].ModelRotation = glm::vec3(0.0f, 0.0f, 90.0f);
    ModelList[1].ModelPosition = glm::vec3(2.0f, 4.0f, 0.0f);
    ModelList[2].ModelPosition = glm::vec3(3.0f, 3.0f, 0.0f);
    ModelList[3].ModelPosition = glm::vec3(7.0f, 3.0f, 0.0f);

    Skybox = SkyBoxMesh(vulkanEngine, textureManager, renderManager.mainRenderPass.skyBoxPipeline->ShaderPipelineDescriptorLayout, meshTextures);
    renderManager.CMDBuffer(vulkanEngine, ModelList, Skybox);
}

VulkanGraphics::~VulkanGraphics()
{
    renderManager.Destroy(vulkanEngine);
    for (auto model : ModelList)
    {
        model.Destroy(vulkanEngine);
    }
    Skybox.Destory(vulkanEngine);
    textureManager->UnloadAllTextures(vulkanEngine);
    vulkanEngine.Destory();
    window.CleanUp();
}

void VulkanGraphics::MainLoop()
{
    while (!glfwWindowShouldClose(window.GetWindowPtr())) 
    {
        glfwPollEvents();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::CheckboxFlags("Normally", &ModelList[0].MeshList[0]->RenderFlags, RenderDrawFlags::RenderNormally);
            ImGui::CheckboxFlags("WireFrame", &ModelList[0].MeshList[0]->RenderFlags, RenderDrawFlags::RenderWireFrame);
            ImGui::CheckboxFlags("Shadow", &ModelList[0].MeshList[0]->RenderFlags, RenderDrawFlags::RenderShadow);

            ImGui::CheckboxFlags("Normally2", &ModelList[1].MeshList[0]->RenderFlags, RenderDrawFlags::RenderNormally);
            ImGui::CheckboxFlags("WireFrame2", &ModelList[1].MeshList[0]->RenderFlags, RenderDrawFlags::RenderWireFrame);
            ImGui::CheckboxFlags("Shadow2", &ModelList[1].MeshList[0]->RenderFlags, RenderDrawFlags::RenderShadow);

            ImGui::CheckboxFlags("Normally3", &ModelList[2].MeshList[0]->RenderFlags, RenderDrawFlags::RenderNormally);
            ImGui::CheckboxFlags("WireFrame3", &ModelList[2].MeshList[0]->RenderFlags, RenderDrawFlags::RenderWireFrame);
            ImGui::CheckboxFlags("Shadow3", &ModelList[2].MeshList[0]->RenderFlags, RenderDrawFlags::RenderShadow);

            ImGui::CheckboxFlags("Normally4", &ModelList[3].MeshList[0]->RenderFlags, RenderDrawFlags::RenderNormally);
            ImGui::CheckboxFlags("WireFrame4", &ModelList[3].MeshList[0]->RenderFlags, RenderDrawFlags::RenderWireFrame);
            ImGui::CheckboxFlags("Shadow4", &ModelList[3].MeshList[0]->RenderFlags, RenderDrawFlags::RenderShadow);

            ImGui::Image(renderManager.sceneRenderPass.ColorTexture->ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
            ImGui::Image(renderManager.sceneRenderPass.BloomTexture->ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));/*
            ImGui::Image(renderManager.shadowRenderPass.DepthTexture->ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));*/
            textureManager->UpdateIMGUIVRAM();
        }
        ImGui::Render();

        renderManager.UpdateCommandBuffer(vulkanEngine, ModelList, Skybox);
        UpdateUniformBuffer(vulkanEngine.DrawFrame);
        renderManager.Draw(vulkanEngine, window.GetWindowPtr(), ModelList, Skybox);
        mouse.Update(window.GetWindowPtr(), camera);
        keyboard.Update(window.GetWindowPtr(), camera);
    }

    vkDeviceWaitIdle(vulkanEngine.Device);
}

void VulkanGraphics::UpdateUniformBuffer(uint32_t currentImage)
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
