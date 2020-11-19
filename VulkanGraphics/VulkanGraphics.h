#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

class VulkanGraphics
{

private:
    VulkanWindow window;
    VulkanEngine vulkanEngine;
    RenderManager renderManager;
    std::shared_ptr<TextureManager> textureManager;

    std::shared_ptr<PerspectiveCamera> camera;
    Keyboard keyboard;
    Mouse mouse;

    std::vector<Model> ModelList;
    SkyBoxMesh Skybox;

    void UpdateUniformBuffer(uint32_t currentImage);

public:
    VulkanGraphics();
    ~VulkanGraphics();

    void MainLoop();
};
