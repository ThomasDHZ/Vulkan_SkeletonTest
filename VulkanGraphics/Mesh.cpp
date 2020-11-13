#include "Mesh.h"


Mesh::Mesh() : BaseMesh()
{
}

Mesh::Mesh(VulkanEngine& engine, const std::vector<Vertex>& vertexdata) : BaseMesh(engine, vertexdata)
{
    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& engine, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata) : BaseMesh(engine, vertexdata, indicesdata)
{
    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& engine, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, CustomBuffer customBuffer) : BaseMesh(engine, vertexdata, indicesdata)
{
    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, const MeshData& meshData, VkDescriptorSetLayout layout) : BaseMesh(engine, meshData)
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
    CreateDescriptorSets(engine, textureManager, layout);

    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, VkDescriptorSetLayout layout) : BaseMesh(engine, vertexdata, indicesdata)
{
    CustomBuffer EmptyBuffer;
    EmptyBuffer.ByteSize = sizeof(Empty);

    ExtendedMeshProperitesBuffer = EmptyBuffer;

    CreateMaterialProperties(textures);
    LoadTextures(engine, textureManager, textures);
    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, textureManager, layout);

    for (int x = 0; x < 300; x++)
    {
        ubo.BoneTransform[x] = glm::mat4(1.0f);
    }
}

Mesh::Mesh(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, CustomBuffer customBuffer, VkDescriptorSetLayout layout) : BaseMesh(engine, vertexdata, indicesdata)
{
    ExtendedMeshProperitesBuffer = customBuffer;

    LoadTextures(engine, textureManager, textures);
    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, textureManager, layout);
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

    std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //DescriptorPoolInfo[6].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //DescriptorPoolInfo[7].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //DescriptorPoolInfo[8].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //DescriptorPoolInfo[9].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    //DescriptorPoolInfo[10].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    BaseMesh::CreateDescriptorPool(engine, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh::CreateDescriptorSets(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout layout)
{
    BaseMesh::CreateDescriptorSets(engine, layout);

    VkDescriptorImageInfo DiffuseMap = {};
    DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DiffuseMap.imageView = DiffuseTexture->GetTextureView();
    DiffuseMap.sampler = DiffuseTexture->GetTextureSampler();

    //VkDescriptorImageInfo SpecularMap = {};
    //SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //SpecularMap.imageView = SpecularTexture->GetTextureView();
    //SpecularMap.sampler = SpecularTexture->GetTextureSampler();

    //VkDescriptorImageInfo NormalMap = {};
    //NormalMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //NormalMap.imageView = NormalTexture->GetTextureView();
    //NormalMap.sampler = NormalTexture->GetTextureSampler();

    //VkDescriptorImageInfo DisplacementMap = {};
    //DisplacementMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //DisplacementMap.imageView = DepthTexture->GetTextureView();
    //DisplacementMap.sampler = DepthTexture->GetTextureSampler();

    //VkDescriptorImageInfo AlphaMap = {};
    //AlphaMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //AlphaMap.imageView = DiffuseTexture->GetTextureView();
    //AlphaMap.sampler = DiffuseTexture->GetTextureSampler();

    //VkDescriptorImageInfo EmissionMap = {};
    //EmissionMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //EmissionMap.imageView = EmissionTexture->GetTextureView();
    //EmissionMap.sampler = EmissionTexture->GetTextureSampler();

    //VkDescriptorImageInfo ReflectionMap = {};
    //ReflectionMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //ReflectionMap.imageView = ReflectionTexture->GetTextureView();
    //ReflectionMap.sampler = ReflectionTexture->GetTextureSampler();

    //VkDescriptorImageInfo SkyBoxMap = {};
    //SkyBoxMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //SkyBoxMap.imageView = SkyBoxTexture->GetTextureView();
    //SkyBoxMap.sampler = SkyBoxTexture->GetTextureSampler();

    for (size_t i = 0; i < engine.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkDescriptorBufferInfo PositionInfo = {};
        PositionInfo.buffer = uniformBuffer.GetUniformBuffer(i);
        PositionInfo.offset = 0;
        PositionInfo.range = sizeof(VertexMatrixObject);

        VkDescriptorBufferInfo LightInfo = {};
        LightInfo.buffer = lightBuffer.GetUniformBuffer(i);
        LightInfo.offset = 0;
        LightInfo.range = sizeof(LightBufferObject);

        VkDescriptorBufferInfo meshPropertiesInfo = {};
        meshPropertiesInfo.buffer = meshPropertiesBuffer.GetUniformBuffer(i);
        meshPropertiesInfo.offset = 0;
        meshPropertiesInfo.range = sizeof(MeshProperties);

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

        //WriteDescriptorSetInfo SpecularMapDescriptor;
        //SpecularMapDescriptor.DstBinding = 2;
        //SpecularMapDescriptor.DstSet = DescriptorSets[i];
        //SpecularMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //SpecularMapDescriptor.DescriptorImageInfo = SpecularMap;
        //DescriptorList.emplace_back(SpecularMapDescriptor);

        //WriteDescriptorSetInfo NormalMapDescriptor;
        //NormalMapDescriptor.DstBinding = 3;
        //NormalMapDescriptor.DstSet = DescriptorSets[i];
        //NormalMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //NormalMapDescriptor.DescriptorImageInfo = NormalMap;
        //DescriptorList.emplace_back(NormalMapDescriptor);

        //WriteDescriptorSetInfo DisplacementMapDescriptor;
        //DisplacementMapDescriptor.DstBinding = 4;
        //DisplacementMapDescriptor.DstSet = DescriptorSets[i];
        //DisplacementMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //DisplacementMapDescriptor.DescriptorImageInfo = DisplacementMap;
        //DescriptorList.emplace_back(DisplacementMapDescriptor);

        //WriteDescriptorSetInfo AlphaMapDescriptor;
        //AlphaMapDescriptor.DstBinding = 5;
        //AlphaMapDescriptor.DstSet = DescriptorSets[i];
        //AlphaMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //AlphaMapDescriptor.DescriptorImageInfo = AlphaMap;
        //DescriptorList.emplace_back(AlphaMapDescriptor);

        //WriteDescriptorSetInfo EmissionMapDescriptor;
        //EmissionMapDescriptor.DstBinding = 6;
        //EmissionMapDescriptor.DstSet = DescriptorSets[i];
        //EmissionMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //EmissionMapDescriptor.DescriptorImageInfo = EmissionMap;
        //DescriptorList.emplace_back(EmissionMapDescriptor);

        //WriteDescriptorSetInfo ReflectionMapDescriptor;
        //ReflectionMapDescriptor.DstBinding = 7;
        //ReflectionMapDescriptor.DstSet = DescriptorSets[i];
        //ReflectionMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //ReflectionMapDescriptor.DescriptorImageInfo = ReflectionMap;
        //DescriptorList.emplace_back(ReflectionMapDescriptor);

        //WriteDescriptorSetInfo SkyBoxDescriptor;
        //SkyBoxDescriptor.DstBinding = 8;
        //SkyBoxDescriptor.DstSet = DescriptorSets[i];
        //SkyBoxDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //SkyBoxDescriptor.DescriptorImageInfo = SkyBoxMap;
        //DescriptorList.emplace_back(SkyBoxDescriptor);

        //WriteDescriptorSetInfo  MeshPropertiesDescriptor;
        //MeshPropertiesDescriptor.DstBinding = 9;
        //MeshPropertiesDescriptor.DstSet = DescriptorSets[i];
        //MeshPropertiesDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        //MeshPropertiesDescriptor.DescriptorBufferInfo = meshPropertiesInfo;
        //DescriptorList.emplace_back(MeshPropertiesDescriptor);

        //WriteDescriptorSetInfo LightDescriptor;
        //LightDescriptor.DstBinding = 10;
        //LightDescriptor.DstSet = DescriptorSets[i];
        //LightDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        //LightDescriptor.DescriptorBufferInfo = LightInfo;
        //DescriptorList.emplace_back(LightDescriptor);

        BaseMesh::CreateDescriptorSetsData(engine, DescriptorList);
    }
}

void Mesh::Update(VulkanEngine& engine)
{
    BaseMesh::Update(engine);
}

void Mesh::Update(VulkanEngine& engine, std::shared_ptr<Camera> camera, LightBufferObject Lightbuffer, void* CustomBufferinfo)
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

    properites.timer = glfwGetTime();
    UpdateUniformBuffer(engine, ubo, Lightbuffer, CustomBufferinfo);
}

void Mesh::Update(VulkanEngine& engine, std::shared_ptr<Camera> camera, LightBufferObject Lightbuffer, const std::vector<std::shared_ptr<Bone>>& BoneList, void* CustomBufferinfo)
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

    for (auto bone : BoneList)
    {
        ubo.BoneTransform[bone->BoneID] = bone->FinalTransformMatrix;
    }

    properites.timer = glfwGetTime();
    UpdateUniformBuffer(engine, ubo, Lightbuffer, CustomBufferinfo);
}

void Mesh::ScreenResizeUpdate(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, VkDescriptorSetLayout layout)
{
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, textureManager, layout);
}

void Mesh::UpdateUniformBuffer(VulkanEngine& engine, VertexMatrixObject ubo, void* CustomBufferinfo)
{
    uniformBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&ubo));
}

void Mesh::UpdateUniformBuffer(VulkanEngine& engine, VertexMatrixObject ubo, LightBufferObject Lightbuffer, void* CustomBufferinfo)
{
    uniformBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&ubo));
    //lightBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&Lightbuffer));
    //meshPropertiesBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&properites));
    //if (!CustomBufferinfo == NULL)
    //{
    //    ExtendedMeshProperitesBuffer.customBuffer.UpdateUniformBuffer(engine, CustomBufferinfo);
    //}
    //else
    //{
    //    Empty empty = {};
    //    empty.empty = 1.0f;
    //    ExtendedMeshProperitesBuffer.customBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&empty));
    //}
}

void Mesh::Destory(VulkanEngine& engine)
{
    uniformBuffer.Destroy(engine);
    lightBuffer.Destroy(engine);
    meshPropertiesBuffer.Destroy(engine);
    ExtendedMeshProperitesBuffer.customBuffer.Destroy(engine);
    BaseMesh::Destory(engine);
}