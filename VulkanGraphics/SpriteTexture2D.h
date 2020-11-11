#pragma once
#include "Texture.h"

class SpriteTexture2D : public Texture
{
private:
    void CreateTextureView(VulkanEngine& renderer, VkFormat format);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    SpriteTexture2D();
    SpriteTexture2D(VulkanEngine& renderer, VkFormat format, std::string TextureLocation, unsigned int textureID);
    ~SpriteTexture2D();
};