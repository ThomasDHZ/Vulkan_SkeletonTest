#include "Mesh.h"
#include <chrono>


Mesh::Mesh() : BaseMesh()
{
}

Mesh::Mesh(VulkanEngine& renderer, const std::vector<Vertex>& vertexdata) : BaseMesh(renderer, vertexdata)
{
    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& renderer, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, VkDescriptorSetLayout desc, Texture& texture) : BaseMesh(renderer, vertexdata, indicesdata)
{
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, desc, texture);

    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& renderer, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata) : BaseMesh(renderer, vertexdata, indicesdata)
{
    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& renderer, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, CustomBuffer customBuffer) : BaseMesh(renderer, vertexdata, indicesdata)
{
    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

//Mesh::Mesh(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const MeshData& meshData) : BaseMesh(renderer, meshData)
//{
//    CustomBuffer EmptyBuffer;
//    EmptyBuffer.ByteSize = sizeof(Empty);
//
//    NodeId = meshData.NodeID;
//    MeshID = meshData.MeshID;
//    MeshName = meshData.NodeName;
//    properites = meshData.properties;
//    TransformMatrix = meshData.TransformMatrix;
//    ExtendedMeshProperitesBuffer = EmptyBuffer;
//
//    LoadTextures(renderer, textureManager, meshData.TextureList);
//    CreateUniformBuffers(renderer);
//    CreateDescriptorPool(renderer);
//    CreateDescriptorSets(renderer, textureManager);
//
//    for (int x = 0; x < 300; x++)
//    {
//        ubo.BoneTransform[x] = glm::mat4(1.0f);
//    }
//}

//Mesh::Mesh(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures) : BaseMesh(renderer, vertexdata, indicesdata)
//{
//    CustomBuffer EmptyBuffer;
//    EmptyBuffer.ByteSize = sizeof(Empty);
//
//    ExtendedMeshProperitesBuffer = EmptyBuffer;
//
//    CreateMaterialProperties(textures);
//    LoadTextures(renderer, textureManager, textures);
//    CreateUniformBuffers(renderer);
//    CreateDescriptorPool(renderer);
//    CreateDescriptorSets(renderer, textureManager);
//
//    for (int x = 0; x < 300; x++)
//    {
//        ubo.BoneTransform[x] = glm::mat4(1.0f);
//    }
//}

//Mesh::Mesh(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, CustomBuffer customBuffer) : BaseMesh(renderer, vertexdata, indicesdata)
//{
//    ExtendedMeshProperitesBuffer = customBuffer;
//
//    LoadTextures(renderer, textureManager, textures);
//    CreateUniformBuffers(renderer);
//    CreateDescriptorPool(renderer);
//    CreateDescriptorSets(renderer, textureManager);
//    CreateMaterialProperties(textures);
//
//    for (int x = 0; x < 300; x++)
//    {
//        ubo.BoneTransform[x] = glm::mat4(1.0f);
//    }
//}

Mesh::~Mesh()
{
}

void Mesh::SetTransformMatrix(glm::mat4 NewTranformMatrix)
{
    TransformMatrix = NewTranformMatrix;
}

void Mesh::CreateMaterialProperties(MeshTextures textures)
{
  /*  if (textures.DiffuseMap != DefaultTexture)
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
    properites.heightScale = 0.1f;*/
}

void Mesh::CreateUniformBuffers(VulkanEngine& renderer)
{
    uniformBuffer = VulkanUniformBuffer(renderer, sizeof(VertexMatrixObject));
    //lightBuffer = VulkanUniformBuffer(renderer, sizeof(LightBufferObject));
    //meshPropertiesBuffer = VulkanUniformBuffer(renderer, sizeof(MeshProperties));
    //ExtendedMeshProperitesBuffer.customBuffer = VulkanUniformBuffer(renderer, ExtendedMeshProperitesBuffer.ByteSize);
}

void Mesh::CreateDescriptorPool(VulkanEngine& renderer) {

    std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh::CreateDescriptorSets(VulkanEngine& renderer, VkDescriptorSetLayout desc, Texture textureManager)
{
    BaseMesh::CreateDescriptorSets(renderer, desc);

    VkDescriptorImageInfo DiffuseMap = {};
    DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DiffuseMap.imageView = textureManager.GetTextureView();
    DiffuseMap.sampler = textureManager.GetTextureSampler();

    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkDescriptorBufferInfo PositionInfo = {};
        PositionInfo.buffer = uniformBuffer.GetUniformBuffer(i);
        PositionInfo.offset = 0;
        PositionInfo.range = sizeof(VertexMatrixObject);

        std::vector<WriteDescriptorSetInfo> DescriptorList;

        WriteDescriptorSetInfo PositionDescriptor;
        PositionDescriptor.DstBinding = 0;
        PositionDescriptor.DstSet = DescriptorSets[i];
        PositionDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        PositionDescriptor.DescriptorBufferInfo = PositionInfo;
        DescriptorList.emplace_back(PositionDescriptor);

        WriteDescriptorSetInfo DiffuseMapDescriptor;
        DiffuseMapDescriptor.DstBinding = 1;
        DiffuseMapDescriptor.DstSet = DescriptorSets[i];
        DiffuseMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        DiffuseMapDescriptor.DescriptorImageInfo = DiffuseMap;
        DescriptorList.emplace_back(DiffuseMapDescriptor);

        BaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}

void Mesh::Update(VulkanEngine& renderer)
{
    BaseMesh::Update(renderer);
}

void Mesh::Update(VulkanEngine& renderer, std::shared_ptr<Camera> camera, LightBufferObject Lightbuffer, void* CustomBufferinfo)
{

    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    VertexMatrixObject ubo2{};
         ubo.model = glm::mat4(1.0f);
         ubo.model = glm::rotate(ubo2.model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
         ubo.view = camera->GetViewMatrix();
         ubo.proj = camera->GetProjectionMatrix();
         ubo.proj[1][1] *= -1;

   // properites.timer = glfwGetTime();
    UpdateUniformBuffer(renderer, ubo2, Lightbuffer, CustomBufferinfo);
}

void Mesh::Update(VulkanEngine& renderer, std::shared_ptr<Camera> camera, LightBufferObject Lightbuffer, const std::vector<std::shared_ptr<Bone>>& BoneList, void* CustomBufferinfo)
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    VertexMatrixObject ubo2{};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::rotate(ubo2.model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    ubo.view = camera->GetViewMatrix();
    ubo.proj = camera->GetProjectionMatrix();
    ubo.proj[1][1] *= -1;

    for (auto bone : BoneList)
    {
        ubo.BoneTransform[bone->BoneID] = bone->FinalTransformMatrix;
    }

    //properites.timer = glfwGetTime();
    UpdateUniformBuffer(renderer, ubo, Lightbuffer, CustomBufferinfo);
}

//void Mesh::ScreenResizeUpdate(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager)
//{
//    CreateDescriptorPool(renderer);
//    CreateDescriptorSets(renderer, textureManager);
//}

void Mesh::UpdateUniformBuffer(VulkanEngine& renderer, VertexMatrixObject ubo, void* CustomBufferinfo)
{
    uniformBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&ubo));
}

void Mesh::UpdateUniformBuffer(VulkanEngine& renderer, VertexMatrixObject ubo, LightBufferObject Lightbuffer, void* CustomBufferinfo)
{
    uniformBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&ubo));
  //  lightBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&Lightbuffer));
   // meshPropertiesBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&properites));
    //if (!CustomBufferinfo == NULL)
    //{
    //    ExtendedMeshProperitesBuffer.customBuffer.UpdateUniformBuffer(renderer, CustomBufferinfo);
    //}
    //else
    //{
    //    Empty empty = {};
    //    empty.empty = 1.0f;
    //    ExtendedMeshProperitesBuffer.customBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&empty));
    //}
}

void Mesh::Destory(VulkanEngine& renderer)
{
    uniformBuffer.Destroy(renderer);
    //lightBuffer.Destroy(renderer);
    //meshPropertiesBuffer.Destroy(renderer);
    //ExtendedMeshProperitesBuffer.customBuffer.Destroy(renderer);
    BaseMesh::Destory(renderer);
}