#include "DebugLightMesh.h"

DebugLightMesh::DebugLightMesh() : Mesh()
{
}

DebugLightMesh::DebugLightMesh(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, VkDescriptorSetLayout& layout, int renderBit) : Mesh(engine, CubeVertices, CubeIndices, renderBit)
{

    CreateUniformBuffers(engine);
    CreateDescriptorPool(engine);
    CreateDescriptorSets(engine, layout);
}

DebugLightMesh::~DebugLightMesh()
{
}

void DebugLightMesh::CreateUniformBuffers(VulkanEngine& engine)
{
    uniformBuffer = VulkanUniformBuffer(engine, sizeof(VertexMatrixObject));
    meshColorBuffer = VulkanUniformBuffer(engine, sizeof(MeshColor));
}

void DebugLightMesh::CreateDescriptorPool(VulkanEngine& engine) 
{
    std::vector<VkDescriptorPoolSize>  DescriptorPoolList = {};

    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));
    DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));

    BaseMesh::CreateDescriptorPool(engine, DescriptorPoolList);
}

void DebugLightMesh::CreateDescriptorSets(VulkanEngine& engine, VkDescriptorSetLayout& layout)
{
    BaseMesh::CreateDescriptorSets(engine, layout);

    for (size_t i = 0; i < engine.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkDescriptorBufferInfo PositionInfo = AddBufferDescriptorInfo(engine, uniformBuffer.GetUniformBuffer(i), sizeof(VertexMatrixObject));
        VkDescriptorBufferInfo LightInfo = AddBufferDescriptorInfo(engine, meshColorBuffer.GetUniformBuffer(i), sizeof(MeshColor));

        std::vector<VkWriteDescriptorSet> DescriptorList;
        DescriptorList.emplace_back(AddDescriptorSetBufferInfo(engine, 0, DescriptorSets[i], PositionInfo));
        DescriptorList.emplace_back(AddDescriptorSetBufferInfo(engine, 1, DescriptorSets[i], LightInfo));
        BaseMesh::CreateDescriptorSetsData(engine, DescriptorList);
    }
}

void DebugLightMesh::Update(VulkanEngine& engine, std::shared_ptr<PerspectiveCamera>& camera, MeshColor MeshColorBuffer)
{
    VertexMatrixObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(ubo.model, MeshPosition);
    ubo.model = glm::scale(ubo.model, MeshScale);
    ubo.view = camera->GetViewMatrix();
    ubo.proj = camera->GetProjectionMatrix();
    ubo.proj[1][1] *= -1;

    UpdateUniformBuffer(engine, ubo, MeshColorBuffer);
    Mesh::UpdateUniformBuffer(engine, ubo);
}

void DebugLightMesh::UpdateUniformBuffer(VulkanEngine& engine, VertexMatrixObject ubo, MeshColor MeshColorBuffer)
{
    meshColorBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&MeshColorBuffer));
    Mesh::UpdateUniformBuffer(engine, ubo);
}

void DebugLightMesh::Destory(VulkanEngine& engine)
{
    uniformBuffer.Destroy(engine);
    meshColorBuffer.Destroy(engine);
    BaseMesh::Destory(engine);
}