#pragma once
#include "Texture.h"

class RenderedHDRColorTexture : public Texture
{
private:
    void CreateTextureImage(VulkanEngine& renderer);
    void CreateTextureView(VulkanEngine& renderer);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    RenderedHDRColorTexture();
    RenderedHDRColorTexture(VulkanEngine& renderer);
    ~RenderedHDRColorTexture();

    void RecreateRendererTexture(VulkanEngine& renderer);
};