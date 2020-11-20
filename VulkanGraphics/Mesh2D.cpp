#include "Mesh2D.h"
Mesh2D::Mesh2D() : Mesh()
{
}

Mesh2D::Mesh2D(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, VkDescriptorSetLayout& layout, int RenderFlags) : Mesh(engine, vertexdata, indicesdata, RenderFlags)
{
    CustomBuffer EmptyBuffer;
    EmptyBuffer.ByteSize = sizeof(Empty);

    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

Mesh2D::Mesh2D(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, VkDescriptorSetLayout& layout, int RenderFlags) : Mesh(engine, textureManager, vertexdata, indicesdata, textures, layout, RenderFlags)
{
    CustomBuffer EmptyBuffer;
    EmptyBuffer.ByteSize = sizeof(Empty);

    ExtendedMeshProperitesBuffer = EmptyBuffer;

    CreateMaterialProperties(textures);
    LoadTextures(engine, textureManager, textures);
    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

Mesh2D::Mesh2D(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, CustomBuffer customBuffer, VkDescriptorSetLayout& layout, int RenderFlags) : Mesh(engine, textureManager, vertexdata, indicesdata, textures, layout, RenderFlags)
{
    ExtendedMeshProperitesBuffer = customBuffer;

    LoadTextures(engine, textureManager, textures);
    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
    CreateMaterialProperties(textures);
}

Mesh2D::~Mesh2D()
{
}

void Mesh2D::ScreenResizeUpdate(VulkanEngine& engine, VkDescriptorSetLayout& layout)
{
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

void Mesh2D::CreateDescriptorPool(VulkanEngine& engine) 
{
    std::vector<VkDescriptorPoolSize>  DescriptorPoolList = {};
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));
    BaseMesh::CreateDescriptorPool(engine, DescriptorPoolList);
}

void Mesh2D::CreateDescriptorSets(VulkanEngine& engine, VkDescriptorSetLayout& layout)
{
    BaseMesh::CreateDescriptorSets(engine, layout);

    VkDescriptorImageInfo DiffuseMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, DiffuseTexture);
    VkDescriptorImageInfo SpecularMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, SpecularTexture);
    VkDescriptorImageInfo NormalMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, NormalTexture);
    VkDescriptorImageInfo AlphaMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, AlphaTexture);
    VkDescriptorImageInfo EmissionMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, EmissionTexture);

    for (size_t i = 0; i < engine.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkDescriptorBufferInfo PositionInfo = AddBufferDescriptorInfo(engine, uniformBuffer.GetUniformBuffer(i), sizeof(VertexMatrixObject));
        VkDescriptorBufferInfo LightInfo = AddBufferDescriptorInfo(engine, lightBuffer.GetUniformBuffer(i), sizeof(LightBufferObject));
        VkDescriptorBufferInfo meshPropertiesInfo = AddBufferDescriptorInfo(engine, meshPropertiesBuffer.GetUniformBuffer(i), sizeof(MeshProperties));
        VkDescriptorBufferInfo emptyPropertiesInfo = AddBufferDescriptorInfo(engine, ExtendedMeshProperitesBuffer.customBuffer.GetUniformBuffer(i), ExtendedMeshProperitesBuffer.ByteSize);

        std::vector<VkWriteDescriptorSet> DescriptorList;
        DescriptorList.emplace_back(AddDescriptorSetBufferInfo(engine, 0, DescriptorSets[i], PositionInfo));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 1, DescriptorSets[i], DiffuseMap));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 2, DescriptorSets[i], SpecularMap));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 3, DescriptorSets[i], NormalMap));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 4, DescriptorSets[i], AlphaMap));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 5, DescriptorSets[i], EmissionMap));
        DescriptorList.emplace_back(AddDescriptorSetBufferInfo(engine, 6, DescriptorSets[i], meshPropertiesInfo));
        DescriptorList.emplace_back(AddDescriptorSetBufferInfo(engine, 7, DescriptorSets[i], LightInfo));
        DescriptorList.emplace_back(AddDescriptorSetBufferInfo(engine, 8, DescriptorSets[i], emptyPropertiesInfo));
        BaseMesh::CreateDescriptorSetsData(engine, DescriptorList);
    }
}