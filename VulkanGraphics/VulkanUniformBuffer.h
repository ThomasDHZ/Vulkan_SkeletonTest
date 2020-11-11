#pragma once
#include <vulkan\vulkan_core.h>
#include "VulkanEngine.h"
#include "VulkanBufferManager.h"

class VulkanUniformBuffer
{
private:
	VkDeviceSize BufferSize;
	std::vector<VkBuffer> UniformBuffers;
	std::vector<VkDeviceMemory> UniformBuffersMemory;

	void CreateUniformBuffers(VulkanEngine& renderer);
public:
	VulkanUniformBuffer();
	VulkanUniformBuffer(VulkanEngine& renderer, VkDeviceSize bufferSize);
	~VulkanUniformBuffer();

	void UpdateUniformBuffer(VulkanEngine& renderer, void* UniformObjectData);
	void Destroy(VulkanEngine& renderer);
	VkBuffer GetUniformBuffer(int currentImage) { return UniformBuffers[currentImage]; };
};
