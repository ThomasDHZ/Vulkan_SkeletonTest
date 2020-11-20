#pragma once
#include "Mesh.h"
#include <map>
#include "Mesh.h"

class Mesh2D : public Mesh
{
private:
    void CreateDescriptorPool(VulkanEngine& engine) override;
    void CreateDescriptorSets(VulkanEngine& engine, VkDescriptorSetLayout& layout) override;

public:

    Mesh2D();
    Mesh2D(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, VkDescriptorSetLayout& layout, int RenderFlags);
    Mesh2D(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, VkDescriptorSetLayout& layout, int RenderFlags);
    Mesh2D(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, CustomBuffer customBuffer, VkDescriptorSetLayout& layout, int RenderFlags);
    ~Mesh2D();

    virtual void ScreenResizeUpdate(VulkanEngine& engine, VkDescriptorSetLayout& layout) override;
};