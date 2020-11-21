#pragma once
#include "Texture.h"

class RenderedGBufferPositionTexture : public Texture
{
private:
    void CreateTextureImage(VulkanEngine& renderer);
    void CreateTextureView(VulkanEngine& renderer);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    RenderedGBufferPositionTexture();
    RenderedGBufferPositionTexture(VulkanEngine& renderer);
    ~RenderedGBufferPositionTexture();

    void RecreateRendererTexture(VulkanEngine& renderer);
};