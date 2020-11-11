#include "VulkanUniformBuffer.h"

VulkanUniformBuffer::VulkanUniformBuffer()
{
}

VulkanUniformBuffer::VulkanUniformBuffer(VulkanEngine& renderer, VkDeviceSize bufferSize)
{
	BufferSize = bufferSize;
	CreateUniformBuffers(renderer);
}

VulkanUniformBuffer::~VulkanUniformBuffer()
{
}

void VulkanUniformBuffer::CreateUniformBuffers(VulkanEngine& renderer)
{
	UniformBuffers.resize(renderer.SwapChain.GetSwapChainImageCount());
	UniformBuffersMemory.resize(renderer.SwapChain.GetSwapChainImageCount());
	for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
	{
		VulkanBufferManager::CreateBuffer(renderer, BufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, UniformBuffers[i], UniformBuffersMemory[i]);
	}
}

void VulkanUniformBuffer::UpdateUniformBuffer(VulkanEngine& renderer, void* UniformObjectData)
{
	void* UniformData;
	vkMapMemory(renderer.Device, UniformBuffersMemory[renderer.DrawFrame], 0, BufferSize, 0, &UniformData);
	memcpy(UniformData, UniformObjectData, BufferSize);
	vkUnmapMemory(renderer.Device, UniformBuffersMemory[renderer.DrawFrame]);
}

void VulkanUniformBuffer::Destroy(VulkanEngine& renderer)
{
	for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
	{
		vkDestroyBuffer(renderer.Device, UniformBuffers[i], nullptr);
		vkFreeMemory(renderer.Device, UniformBuffersMemory[i], nullptr);

		UniformBuffers[i] = VK_NULL_HANDLE;
		UniformBuffersMemory[i] = VK_NULL_HANDLE;
	}
}