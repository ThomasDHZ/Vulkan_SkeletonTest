#pragma once
#include "Texture.h"

struct CubeMapLayout
{
	std::string Front;
	std::string Back;
	std::string Top;
	std::string Bottom;
	std::string Right;
	std::string Left;
};

class CubeMapTexture : public Texture
{
private:
	void LoadTexture(VulkanEngine& renderer, CubeMapLayout CubeMapFiles);
	void CreateTextureView(VulkanEngine& renderer);
	void CreateTextureSampler(VulkanEngine& renderer);
public:
	CubeMapTexture();
	CubeMapTexture(VulkanEngine& renderer, CubeMapLayout CubeMapFiles);
	CubeMapTexture(VulkanEngine& renderer, std::string CubeMapFiles[6]);
	~CubeMapTexture();
};