#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
private:
    void CreateTextureView(VulkanEngine& renderer, VkFormat format);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    Texture2D();
    Texture2D(VulkanEngine& renderer, VkFormat format, std::string TextureLocation, unsigned int textureID);
    ~Texture2D();
};
