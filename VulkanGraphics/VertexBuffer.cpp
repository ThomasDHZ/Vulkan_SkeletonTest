#include "VertexBuffer.h"
#include "VulkanBufferManager.h"

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::VertexBuffer(VulkanEngine& renderer, const std::vector<Vertex>& vertexdata)
{
    VertexCount = vertexdata.size();

    VkDeviceSize bufferSize = sizeof(vertexdata[0]) * VertexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanBufferManager::CreateBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(renderer.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertexdata.data(), (size_t)bufferSize);
    vkUnmapMemory(renderer.Device, stagingBufferMemory);

    VulkanBufferManager::CreateBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, VertexBufferMemory);

    VulkanBufferManager::CopyBuffer(renderer, stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(renderer.Device, stagingBuffer, nullptr);
    vkFreeMemory(renderer.Device, stagingBufferMemory, nullptr);
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::Destory(VulkanEngine& renderer)
{
    if (VertexCount != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(renderer.Device, vertexBuffer, nullptr);
        vkFreeMemory(renderer.Device, VertexBufferMemory, nullptr);

        vertexBuffer = VK_NULL_HANDLE;
        VertexBufferMemory = VK_NULL_HANDLE;
        VertexCount = 0;
    }
}
