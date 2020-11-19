#include "SkyBoxMesh.h"

SkyBoxMesh::SkyBoxMesh() : Mesh()
{
}

SkyBoxMesh::SkyBoxMesh(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& layout, MeshTextures textures) : Mesh(engine, SkyBoxVertices, RenderDrawFlags::RenderNormally)
{
	LoadTextures(engine, textureManager, textures);
	CreateUniformBuffers(engine);
	CreateDescriptorPool(engine);
	CreateDescriptorSets(engine, layout);
}

SkyBoxMesh::~SkyBoxMesh()
{
}

void SkyBoxMesh::CreateUniformBuffers(VulkanEngine& engine)
{
	uniformBuffer = VulkanUniformBuffer(engine, sizeof(VertexMatrixObject));
}

void SkyBoxMesh::CreateDescriptorPool(VulkanEngine& engine)
{
	std::vector<VkDescriptorPoolSize>  DescriptorPoolList = {};
	DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));
	DescriptorPoolList.emplace_back(AddDsecriptorPoolBinding(engine, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER));
	BaseMesh::CreateDescriptorPool(engine, DescriptorPoolList);
}

void SkyBoxMesh::CreateDescriptorSets(VulkanEngine& engine, VkDescriptorSetLayout& layout)
{
	BaseMesh::CreateDescriptorSets(engine, layout);

	VkDescriptorImageInfo SkyBoxMap = AddImageDescriptorInfo(engine, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, SkyBoxTexture);

	for (size_t i = 0; i < engine.SwapChain.GetSwapChainImageCount(); i++)
	{
		VkDescriptorBufferInfo PositionInfo = AddBufferDescriptorInfo(engine, uniformBuffer.GetUniformBuffer(i), sizeof(VertexMatrixObject));

		std::vector<VkWriteDescriptorSet> DescriptorList;
		DescriptorList.emplace_back(AddDescriptorSetBufferInfo(engine, 0, DescriptorSets[i], PositionInfo));
		DescriptorList.emplace_back(AddDescriptorSetTextureInfo(engine, 1, DescriptorSets[i], SkyBoxMap));
		BaseMesh::CreateDescriptorSetsData(engine, DescriptorList);
	}
}

void SkyBoxMesh::UpdateUniformBuffer(VulkanEngine& engine, std::shared_ptr<Camera> camera)
{
	VertexMatrixObject ubo{};
	ubo.view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	ubo.proj = glm::perspective(glm::radians(camera->GetZoom()), engine.SwapChain.GetSwapChainResolution().width / (float)engine.SwapChain.GetSwapChainResolution().height, 0.1f, 100.0f);
	ubo.proj[1][1] *= -1;

	uniformBuffer.UpdateUniformBuffer(engine, static_cast<void*>(&ubo));
}

void SkyBoxMesh::Destory(VulkanEngine& engine)
{
	uniformBuffer.Destroy(engine);
	BaseMesh::Destory(engine);
}
