#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "VulkanEngine.h"

class IndicesBuffer
{
private:
	unsigned int IndiceCount = 0;
	VkBuffer indiceBuffer = VK_NULL_HANDLE;
	VkDeviceMemory IndiceBufferMemory = VK_NULL_HANDLE;

public:

	IndicesBuffer();
	IndicesBuffer(VulkanEngine& renderer, const std::vector<uint16_t>& indicesdata);
	~IndicesBuffer();

	void Destory(VulkanEngine& renderer);

	unsigned int GetIndiceCount() { return IndiceCount; }
	VkBuffer GetIndiceBuffer() { return indiceBuffer; }
};
