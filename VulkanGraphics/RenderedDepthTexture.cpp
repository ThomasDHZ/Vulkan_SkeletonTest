#include "RenderedDepthTexture.h"
#include "ImGui/imgui_impl_vulkan.h"

RenderedDepthTexture::RenderedDepthTexture() : Texture()
{
}

RenderedDepthTexture::RenderedDepthTexture(VulkanEngine& renderer) : Texture(renderer, TextureType::vkRenderedTexture)
{
    CreateTextureImage(renderer);
    CreateTextureView(renderer);
    CreateTextureSampler(renderer);
    ImGui_ImplVulkan_AddTexture(ImGuiDescriptorSet, Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

RenderedDepthTexture::~RenderedDepthTexture()
{
}

void RenderedDepthTexture::CreateTextureImage(VulkanEngine& renderer)
{
    VkImageCreateInfo TextureInfo{};
    TextureInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    TextureInfo.imageType = VK_IMAGE_TYPE_2D;
    TextureInfo.extent.width = renderer.SwapChain.GetSwapChainResolution().width;
    TextureInfo.extent.height = renderer.SwapChain.GetSwapChainResolution().height;
    TextureInfo.extent.depth = 1;
    TextureInfo.mipLevels = 1;
    TextureInfo.arrayLayers = 1;
    TextureInfo.format = VK_FORMAT_D32_SFLOAT;
    TextureInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    TextureInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    TextureInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    TextureInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    TextureInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    Texture::CreateTextureImage(renderer, TextureInfo);
}

void RenderedDepthTexture::CreateTextureView(VulkanEngine& renderer)
{
    VkImageViewCreateInfo TextureImageViewInfo{};
    TextureImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    TextureImageViewInfo.image = Image;
    TextureImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    TextureImageViewInfo.format = VK_FORMAT_D32_SFLOAT;
    TextureImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    TextureImageViewInfo.subresourceRange.baseMipLevel = 0;
    TextureImageViewInfo.subresourceRange.levelCount = 1;
    TextureImageViewInfo.subresourceRange.baseArrayLayer = 0;
    TextureImageViewInfo.subresourceRange.layerCount = 1;

    Texture::CreateTextureView(renderer, TextureImageViewInfo);
}

void RenderedDepthTexture::CreateTextureSampler(VulkanEngine& renderer)
{
    VkSamplerCreateInfo TextureImageSamplerInfo = {};
    TextureImageSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    TextureImageSamplerInfo.magFilter = VK_FILTER_LINEAR;
    TextureImageSamplerInfo.minFilter = VK_FILTER_LINEAR;
    TextureImageSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    TextureImageSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    TextureImageSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    TextureImageSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    TextureImageSamplerInfo.mipLodBias = 0.0f;
    TextureImageSamplerInfo.maxAnisotropy = 1.0f;
    TextureImageSamplerInfo.minLod = 0.0f;
    TextureImageSamplerInfo.maxLod = 1.0f;
    TextureImageSamplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    Texture::CreateTextureSampler(renderer, TextureImageSamplerInfo);
}

void RenderedDepthTexture::RecreateRendererTexture(VulkanEngine& renderer)
{
    Texture::Delete(renderer);
    CreateTextureImage(renderer);
    CreateTextureView(renderer);
    CreateTextureSampler(renderer);
    ImGui_ImplVulkan_AddTexture(ImGuiDescriptorSet, Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}