#pragma once
#pragma once
#include "Mesh.h"

const std::vector<Vertex> FrameBufferVertices =
{
    {{1.0f, 1.0f, 0.0f},   {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 0.0f},  {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-1.0f, 1.0f, 0.0f},  {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
};

const std::vector<uint16_t> FrameBufferIndices = {
    0, 1, 3, 1, 2, 3
};

struct FrameBufferSettings
{
    alignas(4) float Gamma = 2.2f;
    alignas(4) float HDRValue = 1.0f;
};

class FrameBufferMesh : public Mesh
{
private:

    VulkanUniformBuffer frameBufferSettings;

    void CreateUniformBuffers(VulkanEngine& renderer) override;
    void CreateDescriptorPool(VulkanEngine& renderer) override;
    void CreateDescriptorSets(VulkanEngine& engine, VkDescriptorSetLayout& layout) override;

public:

    FrameBufferMesh();
    FrameBufferMesh(VulkanEngine& renderer, std::shared_ptr<Texture> FrameBufferImage, VkDescriptorSetLayout layout);
    FrameBufferMesh(VulkanEngine& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> FrameBufferImage, VkDescriptorSetLayout layout);
    FrameBufferMesh(VulkanEngine& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> FrameBufferImage, VkDescriptorSetLayout layout, std::shared_ptr<Texture> BloomImage);
    FrameBufferMesh(VulkanEngine& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> FrameBufferImage, VkDescriptorSetLayout layout, std::shared_ptr<Texture> BloomImage, int effectRenderer, std::shared_ptr<GraphicsPipeline> shader);
    ~FrameBufferMesh();

    FrameBufferSettings settings;

    void Update(VulkanEngine& renderer);
    void ScreenResizeUpdate(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, VkDescriptorSetLayout& layout, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage);
    void ScreenResizeUpdate(VulkanEngine& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& layout, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage, int effectRenderer, std::shared_ptr<GraphicsPipeline> shader);
    void Destory(VulkanEngine& renderer) override;
};

