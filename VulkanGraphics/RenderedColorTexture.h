#pragma once
#include "Texture.h"

class RenderedColorTexture : public Texture
{
private:
    void CreateTextureImage(VulkanEngine& renderer);
    void CreateTextureView(VulkanEngine& renderer);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    RenderedColorTexture();
    RenderedColorTexture(VulkanEngine& renderer);
    ~RenderedColorTexture();

    void RecreateRendererTexture(VulkanEngine& renderer);
};