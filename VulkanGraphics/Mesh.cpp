#include "Mesh.h"


Mesh::Mesh() : BaseMesh()
{
}

Mesh::Mesh(VulkanEngine& engine, const std::vector<Vertex>& vertexdata, int renderFlags) : BaseMesh(engine, vertexdata, renderFlags)
{
    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& engine, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, int renderFlags) : BaseMesh(engine, vertexdata, indicesdata, renderFlags)
{
    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& engine, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, CustomBuffer customBuffer, int renderFlags) : BaseMesh(engine, vertexdata, indicesdata, renderFlags)
{
    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, const MeshData& meshData, VkDescriptorSetLayout layout, int renderFlags) : BaseMesh(engine, meshData, renderFlags)
{
    CustomBuffer EmptyBuffer;
    EmptyBuffer.ByteSize = sizeof(Empty);

    NodeId = meshData.NodeID;
    MeshID = meshData.MeshID;
    MeshName = meshData.NodeName;
    properites = meshData.properties;
    TransformMatrix = meshData.TransformMatrix;
    ExtendedMeshProperitesBuffer = EmptyBuffer;

    LoadTextures(engine, textureManager, meshData.TextureList);
    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);

    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, VkDescriptorSetLayout& layout, int renderFlags) : BaseMesh(engine, vertexdata, indicesdata, renderFlags)
{
    CustomBuffer EmptyBuffer;
    EmptyBuffer.ByteSize = sizeof(Empty);

    ExtendedMeshProperitesBuffer = EmptyBuffer;

    CreateMaterialProperties(textures);
    LoadTextures(engine, textureManager, textures);
    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);

    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, CustomBuffer customBuffer, VkDescriptorSetLayout& layout, int renderFlags) : BaseMesh(engine, vertexdata, indicesdata, renderFlags)
{
    ExtendedMeshProperitesBuffer = customBuffer;

    LoadTextures(engine, textureManager, textures);
    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
    CreateMaterialProperties(textures);

    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::~Mesh()
{
}

void Mesh::SetTransformMatrix(glm::mat4 NewTranformMatrix)
{
    TransformMatrix = NewTranformMatrix;
}

void Mesh::CreateMaterialProperties(MeshTextures textures)
{
    if (textures.DiffuseMap != DefaultTexture)
    {
        properites.UseDiffuseMapBit = 1;
    }

    if (textures.SpecularMap != DefaultTexture)
    {
        properites.UseSpecularMapBit = 1;
    }

    if (textures.NormalMap != DefaultTexture)
    {
        properites.UseNormalMapBit = 1;
    }

    if (textures.DepthMap != DefaultTexture)
    {
        properites.UseDepthMapBit = 1;
    }

    if (textures.AlphaMap != DefaultTexture)
    {
        properites.UseAlphaMapBit = 1;
    }

    if (textures.EmissionMap != DefaultTexture)
    {
        properites.UseEmissionMapBit = 1;
    }

    if (textures.ReflectionMap != DefaultTexture)
    {
        properites.UseReflectionMapBit = 1;
    }

    properites.material.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    properites.material.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    properites.material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    properites.material.shininess = 32;
    properites.material.reflectivness = 0;
    properites.minLayers = 8.0f;
    properites.maxLayers = 32.0f;
    properites.heightScale = 0.1f;
}

void Mesh::CreateUniformBuffers(VulkanEngine& engine)
{
    uniformBuffer = VulkanUniformBuffer(engine, sizeof(VertexMatrixObject));
    lightBuffer = VulkanUniformBuffer(engine, sizeof(LightBufferObject));
    meshPropertiesBuffer = VulkanUniformBuffer(engine, sizeof(MeshProperties));
    ExtendedMeshProperitesBuffer.customBuffer = VulkanUniformBuffer(engine, ExtendedMeshProperitesBuffer.ByteSize);
}

void Mesh::CreateDescriptorPool(VulkanEngine& engine) {

    std::vector<VkDescriptorPoolSize>  DescriptorPoolList = {};

    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));

    BaseMesh::CreateDescriptorPool(engine, DescriptorPoolList);
}

void Mesh::CreateDescriptorSets(VulkanEngine& engine, VkDescriptorSetLayout& layout)
{
    BaseMesh::CreateDescriptorSets(engine, layout);

    VkDescriptorImageInfo DiffuseMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, DiffuseTexture);
    VkDescriptorImageInfo SpecularMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, SpecularTexture);
    VkDescriptorImageInfo NormalMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, NormalTexture);
    VkDescriptorImageInfo DisplacementMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, DepthTexture);
    VkDescriptorImageInfo AlphaMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, AlphaTexture);
    VkDescriptorImageInfo EmissionMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, EmissionTexture);
    VkDescriptorImageInfo ReflectionMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, ReflectionTexture);
    VkDescriptorImageInfo SkyBoxMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, SkyBoxTexture);

    for (size_t i = 0; i < engine.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkDescriptorBufferInfo PositionInfo = AddBufferDescriptorInfo(engine, uniformBuffer.GetUniformBuffer(i), sizeof(VertexMatrixObject));
        VkDescriptorBufferInfo LightInfo = AddBufferDescriptorInfo(engine, lightBuffer.GetUniformBuffer(i), sizeof(LightBufferObject));
        VkDescriptorBufferInfo meshPropertiesInfo = AddBufferDescriptorInfo(engine, meshPropertiesBuffer.GetUniformBuffer(i), sizeof(MeshProperties));

        std::vector<VkWriteDescriptorSet> DescriptorList;
        DescriptorList.emplace_back(AddDescriptorSetBufferInfo(engine, 0, DescriptorSets[i], PositionInfo));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 1, DescriptorSets[i], DiffuseMap));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 2, DescriptorSets[i], SpecularMap));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 3, DescriptorSets[i], NormalMap));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 4, DescriptorSets[i], DisplacementMap));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 5, DescriptorSets[i], AlphaMap));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 6, DescriptorSets[i], EmissionMap));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 7, DescriptorSets[i], ReflectionMap));
        DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 8, DescriptorSets[i], SkyBoxMap));
        DescriptorList.emplace_back(AddDescriptorSetBufferInfo(engine, 9, DescriptorSets[i], meshPropertiesInfo));
        DescriptorList.emplace_back(AddDescriptorSetBufferInfo(engine, 10, DescriptorSets[i], LightInfo));
        BaseMesh::CreateDescriptorSetsData(engine, DescriptorList);
    }
}

void Mesh::Update(VulkanEngine& engine)
{
    BaseMesh::Update(engine);
}

void Mesh::Update(VulkanEngine& engine, std::shared_ptr<Camera> camera, LightBufferObject Lightbuffer, glm::mat4 ModelMatrix, void* CustomBufferinfo)
{
    ubo.model = TransformMatrix;
    ubo.model = glm::translate(ubo.model, MeshPosition);
    ubo.model = glm::rotate(ubo.model, glm::radians(MeshRotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    ubo.model = glm::rotate(ubo.model, glm::radians(MeshRotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.model = glm::rotate(ubo.model, glm::radians(MeshRotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.model = glm::scale(ubo.model, MeshScale);
    ubo.view = camera->GetViewMatrix();
    ubo.proj = camera->GetProjectionMatrix();
    ubo.proj[1][1] *= -1;

    ubo.model = ubo.model * ModelMatrix;

    properites.timer = glfwGetTime();
    UpdateUniformBuffer(engine, ubo, Lightbuffer, CustomBufferinfo);
}

void Mesh::Update(VulkanEngine& engine, std::shared_ptr<Camera> camera, LightBufferObject Lightbuffer, const std::vector<std::shared_ptr<Bone>>& BoneList, glm::mat4 ModelMatrix, void* CustomBufferinfo)
{
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::rotate(ubo.model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    ubo.view = camera->GetViewMatrix();
    ubo.proj = camera->GetProjectionMatrix();
    ubo.proj[1][1] *= -1;

    ubo.model = ubo.model * ModelMatrix;

    for (auto bone : BoneList)
    {
        ubo.BoneTransform[bone->BoneID] = bone->FinalTransformMatrix;
    }

    properites.timer = glfwGetTime();
    UpdateUniformBuffer(engine, ubo, Lightbuffer, CustomBufferinfo);
}

void Mesh::ScreenResizeUpdate(VulkanEngine& engine, VkDescriptorSetLayout& layout)
{
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

void Mesh::UpdateUniformBuffer(VulkanEngine& engine, VertexMatrixObject ubo, void* CustomBufferinfo)
{
    uniformBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&ubo));
}

void Mesh::UpdateUniformBuffer(VulkanEngine& engine, VertexMatrixObject ubo, LightBufferObject Lightbuffer, void* CustomBufferinfo)
{
    uniformBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&ubo));
    lightBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&Lightbuffer));
    meshPropertiesBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&properites));
    if (!CustomBufferinfo == NULL)
    {
        ExtendedMeshProperitesBuffer.customBuffer.UpdateUniformBuffer(engine, CustomBufferinfo);
    }
    else
    {
        Empty empty = {};
        empty.empty = 1.0f;
        ExtendedMeshProperitesBuffer.customBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&empty));
    }
}

void Mesh::Destory(VulkanEngine& engine)
{
    uniformBuffer.Destroy(engine);
    lightBuffer.Destroy(engine);
    meshPropertiesBuffer.Destroy(engine);
    ExtendedMeshProperitesBuffer.customBuffer.Destroy(engine);
    BaseMesh::Destory(engine);
}