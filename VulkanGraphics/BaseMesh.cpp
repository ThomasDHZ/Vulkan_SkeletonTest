#include "BaseMesh.h"

BaseMesh::BaseMesh()
{
}

BaseMesh::BaseMesh(VulkanEngine& renderer, const MeshData& meshData)
{
    MeshVertex = VertexBuffer(renderer, meshData.VertexList);
    if (meshData.IndexList.size() != 0)
    {
        MeshIndices = IndicesBuffer(renderer, meshData.IndexList);
    }
}

BaseMesh::BaseMesh(VulkanEngine& renderer, const std::vector<Vertex>& Vertexdata, const std::vector<uint16_t>& Indicesdata)
{
    MeshVertex = VertexBuffer(renderer, Vertexdata);
    if (Indicesdata.size() != 0)
    {
        MeshIndices = IndicesBuffer(renderer, Indicesdata);
    }
}
BaseMesh::BaseMesh(VulkanEngine& renderer, const std::vector<Vertex>& Vertexdata)
{
    MeshVertex = VertexBuffer(renderer, Vertexdata);
}

BaseMesh::~BaseMesh()
{
}

//void BaseMesh::LoadTextures(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures)
//{
//    if (textures.RendererDiffuseMap)
//    {
//        DiffuseTexture = textures.RendererDiffuseMap;
//    }
//    else
//    {
//        DiffuseTexture = textureManager->LoadTexture(renderer, textures.DiffuseMap, VK_FORMAT_R8G8B8A8_SRGB);
//    }
//
//    if (textures.RendererSpecularMap)
//    {
//        SpecularTexture = textures.RendererSpecularMap;
//    }
//    else
//    {
//        SpecularTexture = textureManager->LoadTexture(renderer, textures.SpecularMap, VK_FORMAT_R8G8B8A8_UNORM);
//    }
//
//    if (textures.RendererNormalMap)
//    {
//        NormalTexture = textures.RendererNormalMap;
//    }
//    else
//    {
//        NormalTexture = textureManager->LoadTexture(renderer, textures.NormalMap, VK_FORMAT_R8G8B8A8_UNORM);
//    }
//
//    if (textures.RendererDepthMap)
//    {
//        DepthTexture = textures.RendererDepthMap;
//    }
//    else
//    {
//        DepthTexture = textureManager->LoadTexture(renderer, textures.DepthMap, VK_FORMAT_R8G8B8A8_UNORM);
//    }
//
//    if (textures.RendererAlphaMap)
//    {
//        AlphaTexture = textures.RendererAlphaMap;
//    }
//    else
//    {
//        AlphaTexture = textureManager->LoadTexture(renderer, textures.AlphaMap, VK_FORMAT_R8G8B8A8_UNORM);
//    }
//
//    if (textures.RendererEmissionMap)
//    {
//        EmissionTexture = textures.RendererEmissionMap;
//    }
//    else
//    {
//        EmissionTexture = textureManager->LoadTexture(renderer, textures.EmissionMap, VK_FORMAT_R8G8B8A8_UNORM);
//    }
//
//    if (textures.RendererReflectionMap)
//    {
//        ReflectionTexture = textures.RendererReflectionMap;
//    }
//    else
//    {
//        ReflectionTexture = textureManager->LoadTexture(renderer, textures.ReflectionMap, VK_FORMAT_R8G8B8A8_UNORM);
//    }
//
//    if (!textures.CubeMap[0].empty() &&
//        !textures.CubeMap[1].empty() &&
//        !textures.CubeMap[2].empty() &&
//        !textures.CubeMap[3].empty() &&
//        !textures.CubeMap[4].empty() &&
//        !textures.CubeMap[5].empty())
//    {
//        SkyBoxTexture = textureManager->LoadTexture(renderer, textures.CubeMap);
//    }
//    else if (textures.RendererReflectionMap)
//    {
//        SkyBoxTexture = textures.RendererCubeMap;
//    }
//}

void BaseMesh::CreateDescriptorPool(VulkanEngine& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo)
{
    std::vector<VkDescriptorPoolSize> DescriptorPoolList = {};

    for (auto DescriptorPool : DescriptorPoolInfo)
    {
        VkDescriptorPoolSize DescriptorPoolBinding = {};
        DescriptorPoolBinding.type = DescriptorPool.DescriptorType;
        DescriptorPoolBinding.descriptorCount = static_cast<uint32_t>(renderer.SwapChain.GetSwapChainImageCount());
        DescriptorPoolList.emplace_back(DescriptorPoolBinding);
    }

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolList.size());
    poolInfo.pPoolSizes = DescriptorPoolList.data();
    poolInfo.maxSets = static_cast<uint32_t>(renderer.SwapChain.GetSwapChainImageCount());

    if (vkCreateDescriptorPool(renderer.Device, &poolInfo, nullptr, &DescriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void BaseMesh::CreateDescriptorSets(VulkanEngine& renderer, VkDescriptorSetLayout layout)
{
    std::vector<VkDescriptorSetLayout> layouts(renderer.SwapChain.GetSwapChainImageCount(), layout);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = DescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(renderer.SwapChain.GetSwapChainImageCount());
    allocInfo.pSetLayouts = layouts.data();

    DescriptorSets.resize(renderer.SwapChain.GetSwapChainImageCount());
    if (vkAllocateDescriptorSets(renderer.Device, &allocInfo, DescriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
}

void BaseMesh::CreateDescriptorSetsData(VulkanEngine& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList)
{
    std::vector<VkWriteDescriptorSet>  WriteDescriptorInfo = {};

    for (int x = 0; x < descriptorWritesList.size(); x++)
    {
        VkWriteDescriptorSet DescriptorSet = {};
        DescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        DescriptorSet.dstSet = descriptorWritesList[x].DstSet;
        DescriptorSet.dstBinding = descriptorWritesList[x].DstBinding;
        DescriptorSet.dstArrayElement = 0;
        DescriptorSet.descriptorType = descriptorWritesList[x].DescriptorType;
        DescriptorSet.descriptorCount = 1;
        if (descriptorWritesList[x].DescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
        {
            DescriptorSet.pBufferInfo = &descriptorWritesList[x].DescriptorBufferInfo;
        }
        else
        {
            DescriptorSet.pImageInfo = &descriptorWritesList[x].DescriptorImageInfo;
        }
        WriteDescriptorInfo.emplace_back(DescriptorSet);
    }

    vkUpdateDescriptorSets(renderer.Device, static_cast<uint32_t>(WriteDescriptorInfo.size()), WriteDescriptorInfo.data(), 0, nullptr);
}

void BaseMesh::SetPosition2D(glm::vec2 Pos)
{
    MeshPosition = glm::vec3(Pos, 0.0f);
}

void BaseMesh::SetPosition2D(float x, float y)
{
    MeshPosition = glm::vec3(x, y, 0.0f);
}

void BaseMesh::SetPosition3D(glm::vec3 Pos)
{
    MeshPosition = Pos;
}

void BaseMesh::SetPosition3D(float x, float y, float z)
{
    MeshPosition = glm::vec3(x, y, z);
}

void BaseMesh::SetRotation2D(glm::vec2 Rotation)
{
    MeshRotate = glm::vec3(Rotation, 0.0f);
}

void BaseMesh::SetRotation2D(float x, float y)
{
    MeshRotate = glm::vec3(x, y, 0.0f);
}

void BaseMesh::SetRotation3D(glm::vec3 Rotation)
{
    MeshRotate = Rotation;
}

void BaseMesh::SetRotation3D(float x, float y, float z)
{
    MeshRotate = glm::vec3(x, y, z);
}

void BaseMesh::SetScale2D(glm::vec2 Scale)
{
    MeshScale = glm::vec3(Scale, 1.0f);
}

void BaseMesh::SetScale2D(float x, float y)
{
    MeshScale = glm::vec3(x, y, 1.0f);
}

void BaseMesh::SetScale3D(glm::vec3 Scale)
{
    MeshScale = Scale;
}

void BaseMesh::SetScale3D(float x, float y, float z)
{
    MeshScale = glm::vec3(x, y, z);
}
//
//void BaseMesh::CreateDrawMessage(VulkanEngine& renderer, unsigned int RendererID, std::shared_ptr<GraphicsPipeline> pipeline)
//{
//    RendererDrawMessage DrawMessage = {};
//    DrawMessage.RendererID = RendererID;
//    DrawMessage.MeshVertex = MeshVertex;
//    DrawMessage.MeshIndices = MeshIndices;
//    DrawMessage.DescriptorSets = DescriptorSets;
//    DrawMessage.pipeline = pipeline;
//
//    std::shared_ptr<RendererDrawMessage> DrawMessagePtr = std::make_shared<RendererDrawMessage>(DrawMessage);
//    renderer.DrawMessageList.emplace_back(DrawMessagePtr);
//    DrawMessageList.emplace_back(DrawMessagePtr);
//}

void BaseMesh::Draw(VulkanEngine& renderer, std::shared_ptr<GraphicsPipeline> pipeline)
{
    //VkBuffer vertexBuffers[] = { MeshVertex.GetVertexBuffer() };
    //VkDeviceSize offsets[] = { 0 };

    //{
    //    vkCmdBindPipeline(renderer.RenderCommandBuffer[renderer.DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->ShaderPipeline);
    //    vkCmdBindVertexBuffers(renderer.RenderCommandBuffer[renderer.DrawFrame], 0, 1, vertexBuffers, offsets);
    //    vkCmdBindDescriptorSets(renderer.RenderCommandBuffer[renderer.DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->ShaderPipelineLayout, 0, 1, &DescriptorSets[renderer.DrawFrame], 0, nullptr);
    //    if (MeshIndices.GetIndiceBuffer() == 0)
    //    {
    //        vkCmdDraw(renderer.RenderCommandBuffer[renderer.DrawFrame], MeshVertex.GetVertexCount(), 1, 0, 0);
    //    }
    //    else
    //    {
    //        vkCmdBindIndexBuffer(renderer.RenderCommandBuffer[renderer.DrawFrame], MeshIndices.GetIndiceBuffer(), 0, VK_INDEX_TYPE_UINT16);
    //        vkCmdDrawIndexed(renderer.RenderCommandBuffer[renderer.DrawFrame], static_cast<uint32_t>(MeshIndices.GetIndiceCount()), 1, 0, 0, 0);
    //    }
    //}
}

void BaseMesh::Update(VulkanEngine& renderer)
{
}


void BaseMesh::Destory(VulkanEngine& renderer)
{
    MeshVertex.Destory(renderer);
    MeshVertex.Destory(renderer);
    MeshIndices.Destory(renderer);

    vkDestroyDescriptorPool(renderer.Device, DescriptorPool, nullptr);
    DescriptorPool = VK_NULL_HANDLE;

 /*   for (auto drawMessage : DrawMessageList)
    {
        renderer.RemoveDrawMessage(drawMessage);
    }*/

    MeshDeletedFlag = true;
}