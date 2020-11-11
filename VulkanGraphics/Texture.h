#pragma once
#include <vulkan/vulkan.h>
#include "VulkanEngine.h"
#include "VulkanBufferManager.h"

enum TextureType
{
    vkTexture2D,
    vkTextureCube,
    vkRenderedTexture
};

class Texture
{
private:

protected:

    VkDeviceMemory Memory = VK_NULL_HANDLE;
    VkImage Image = VK_NULL_HANDLE;
    VkSampler Sampler = VK_NULL_HANDLE;

    void TransitionImageLayout(VulkanEngine& renderer, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VulkanEngine& renderer, VkBuffer buffer);

    virtual void LoadTexture(VulkanEngine& renderer, std::string TextureLocation, VkFormat format);
    virtual void CreateTextureImage(VulkanEngine& renderer, VkImageCreateInfo TextureInfo);
    virtual void CreateTextureView(VulkanEngine& renderer, VkImageViewCreateInfo TextureImageViewInfo);
    virtual void CreateTextureSampler(VulkanEngine& renderer, VkSamplerCreateInfo TextureImageSamplerInfo);

public:
    VkImageView View = VK_NULL_HANDLE;
    VkDescriptorSet ImGuiDescriptorSet = VK_NULL_HANDLE;

    unsigned int TextureID = 0;
    std::string FileName;
    TextureType TypeOfTexture;

    int Width;
    int Height;

    Texture();
    Texture(VulkanEngine& renderer, std::string TextureLocation, unsigned int textureID, TextureType textureType, VkFormat format);
    Texture(VulkanEngine& renderer, std::string TextureLocation, TextureType textureType, VkFormat format);
    Texture(VulkanEngine& renderer, unsigned int textureID, TextureType textureType);
    Texture(VulkanEngine& renderer, TextureType textureType);
    ~Texture();

    virtual void Delete(VulkanEngine& renderer);

    VkImageView GetTextureView() { return View; }
    VkSampler GetTextureSampler() { return Sampler; }
};