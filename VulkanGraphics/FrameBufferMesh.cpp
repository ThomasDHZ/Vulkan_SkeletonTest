#include "FrameBufferMesh.h"

FrameBufferMesh::FrameBufferMesh() : Mesh()
{}

FrameBufferMesh::FrameBufferMesh(VulkanEngine& engine, std::shared_ptr<Texture> FrameBufferImage, VkDescriptorSetLayout layout) : Mesh(engine, FrameBufferVertices, FrameBufferIndices, RenderDrawFlags::RenderNormally)
{
    DiffuseTexture = FrameBufferImage;
    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

FrameBufferMesh::FrameBufferMesh(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> FrameBufferImage, VkDescriptorSetLayout layout) : Mesh(engine, FrameBufferVertices, FrameBufferIndices, RenderDrawFlags::RenderNormally)
{
    DiffuseTexture = FrameBufferImage;
    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

FrameBufferMesh::FrameBufferMesh(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> FrameBufferImage, VkDescriptorSetLayout layout, std::shared_ptr<Texture> BloomImage) : Mesh(engine, FrameBufferVertices, FrameBufferIndices, RenderDrawFlags::RenderNormally)
{
    DiffuseTexture = FrameBufferImage;
    EmissionTexture = BloomImage;
    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

FrameBufferMesh::FrameBufferMesh(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> FrameBufferImage, VkDescriptorSetLayout layout, std::shared_ptr<Texture> BloomImage, int effectRenderer, std::shared_ptr<GraphicsPipeline> shader) : Mesh(engine, FrameBufferVertices, FrameBufferIndices, RenderDrawFlags::RenderNormally)
{
    DiffuseTexture = FrameBufferImage;
    EmissionTexture = BloomImage;
    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

FrameBufferMesh::~FrameBufferMesh()
{
}

void FrameBufferMesh::CreateUniformBuffers(VulkanEngine& renderer)
{
    frameBufferSettings = VulkanUniformBuffer(renderer, sizeof(FrameBufferSettings));
}

void FrameBufferMesh::CreateDescriptorPool(VulkanEngine& engine) 
{
    std::vector<VkDescriptorPoolSize>  DescriptorPoolList = {};
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
   //DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    //DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));
    BaseMesh::CreateDescriptorPool(engine, DescriptorPoolList);
}

void FrameBufferMesh::CreateDescriptorSets(VulkanEngine& engine, VkDescriptorSetLayout& layout)
{
    BaseMesh::CreateDescriptorSets(engine, layout);

    VkDescriptorImageInfo DiffuseMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, DiffuseTexture);
   // VkDescriptorImageInfo BloomMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, EmissionTexture);

    for (size_t i = 0; i < engine.SwapChain.GetSwapChainImageCount(); i++)
    {
 //       VkDescriptorBufferInfo FramebufferSettingsBuffer = AddBufferDescriptorInfo(engine, uniformBuffer.GetUniformBuffer(i), sizeof(FrameBufferSettings));

        std::vector<VkWriteDescriptorSet> DescriptorList;
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 0, DescriptorSets[i], DiffuseMap));
        //DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 1, DescriptorSets[i], BloomMap));
        //DescriptorList.emplace_back(AddDescriptorSetBufferInfo(engine, 2, DescriptorSets[i], FramebufferSettingsBuffer));
        BaseMesh::CreateDescriptorSetsData(engine, DescriptorList);
    }
}

void FrameBufferMesh::Update(VulkanEngine& renderer)
{
    frameBufferSettings.UpdateUniformBuffer(renderer, static_cast<void*>(&settings));
}

void FrameBufferMesh::UpdateSwapChain(VulkanEngine& engine, std::shared_ptr<Texture> FrameBufferImage, VkDescriptorSetLayout layout)
{
    vkDestroyDescriptorPool(engine.Device, DescriptorPool, nullptr);
    DescriptorPool = VK_NULL_HANDLE;

    DiffuseTexture = FrameBufferImage;

    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

void FrameBufferMesh::UpdateSwapChain(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, VkDescriptorSetLayout& layout, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage)
{
    vkDestroyDescriptorPool(engine.Device, DescriptorPool, nullptr);
    DescriptorPool = VK_NULL_HANDLE;

    DiffuseTexture = FrameBufferImage;
    EmissionTexture = BloomImage;

    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

void FrameBufferMesh::UpdateSwapChain(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, VkDescriptorSetLayout& layout, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage, int effectRenderer, std::shared_ptr<GraphicsPipeline> shader)
{
    vkDestroyDescriptorPool(engine.Device, DescriptorPool, nullptr);
    DescriptorPool = VK_NULL_HANDLE;

    DiffuseTexture = FrameBufferImage;
    EmissionTexture = BloomImage;

    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

void FrameBufferMesh::Destory(VulkanEngine& engine)
{
    frameBufferSettings.Destroy(engine);
    BaseMesh::Destory(engine);
}