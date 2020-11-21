#pragma once
#include "Texture.h"

class RenderedGBufferAlbedoTexture : public Texture
{
private:
    void CreateTextureImage(VulkanEngine& renderer);
    void CreateTextureView(VulkanEngine& renderer);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    RenderedGBufferAlbedoTexture();
    RenderedGBufferAlbedoTexture(VulkanEngine& renderer);
    ~RenderedGBufferAlbedoTexture();

    void RecreateRendererTexture(VulkanEngine& renderer);
};