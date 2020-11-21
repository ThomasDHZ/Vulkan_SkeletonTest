#pragma once
#include "Texture.h"

class RenderedGBufferNormalTexture : public Texture
{
private:
    void CreateTextureImage(VulkanEngine& renderer);
    void CreateTextureView(VulkanEngine& renderer);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    RenderedGBufferNormalTexture();
    RenderedGBufferNormalTexture(VulkanEngine& renderer);
    ~RenderedGBufferNormalTexture();

    void RecreateRendererTexture(VulkanEngine& renderer);
};