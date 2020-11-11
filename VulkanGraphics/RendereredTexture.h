#pragma once
#include "Texture.h"

class RendereredTexture : public Texture
{
private:
    void CreateTextureImage(VulkanEngine& renderer);
    void CreateTextureView(VulkanEngine& renderer);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    RendereredTexture();
    RendereredTexture(VulkanEngine& renderer, unsigned int textureID);
    ~RendereredTexture();

    void RecreateRendererTexture(VulkanEngine& renderer);
};

