#pragma once
#include "Texture.h"

class RenderedDepthTexture : public Texture
{
private:
    void CreateTextureImage(VulkanEngine& renderer);
    void CreateTextureView(VulkanEngine& renderer);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    RenderedDepthTexture();
    RenderedDepthTexture(VulkanEngine& renderer);
    ~RenderedDepthTexture();

    void RecreateRendererTexture(VulkanEngine& renderer);
};


