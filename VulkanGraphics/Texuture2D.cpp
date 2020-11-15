#include "Texuture2D.h"
#include "ImGui/imgui_impl_vulkan.h"

Texture2D::Texture2D() : Texture()
{
}

Texture2D::Texture2D(VulkanEngine& renderer, VkFormat format, std::string TextureLocation, unsigned int textureID) : Texture(renderer, TextureLocation, textureID, TextureType::vkTexture2D, format)
{
	CreateTextureView(renderer, format);
	CreateTextureSampler(renderer);
	ImGui_ImplVulkan_AddTexture(ImGuiDescriptorSet, Sampler, View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

Texture2D::~Texture2D()
{
}

void Texture2D::CreateTextureView(VulkanEngine& renderer, VkFormat format)
{
	VkImageViewCreateInfo TextureImageViewInfo = {};
	TextureImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	TextureImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	TextureImageViewInfo.format = format;
	TextureImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	TextureImageViewInfo.subresourceRange.baseMipLevel = 0;
	TextureImageViewInfo.subresourceRange.levelCount = 1;
	TextureImageViewInfo.subresourceRange.baseArrayLayer = 0;
	TextureImageViewInfo.subresourceRange.layerCount = 1;
	TextureImageViewInfo.image = Image;

	Texture::CreateTextureView(renderer, TextureImageViewInfo);
}

void Texture2D::CreateTextureSampler(VulkanEngine& renderer)
{
	VkSamplerCreateInfo TextureImageSamplerInfo = {};
	TextureImageSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	TextureImageSamplerInfo.magFilter = VK_FILTER_NEAREST;
	TextureImageSamplerInfo.minFilter = VK_FILTER_NEAREST;
	TextureImageSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	TextureImageSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	TextureImageSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	TextureImageSamplerInfo.anisotropyEnable = VK_FALSE;
	TextureImageSamplerInfo.maxAnisotropy = 1.0f;
	TextureImageSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	TextureImageSamplerInfo.unnormalizedCoordinates = VK_FALSE;
	TextureImageSamplerInfo.compareEnable = VK_FALSE;
	TextureImageSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	TextureImageSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	Texture::CreateTextureSampler(renderer, TextureImageSamplerInfo);
}