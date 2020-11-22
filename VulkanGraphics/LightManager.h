#pragma once
#include "PointLight.h"

class LightManager
{
private:

public:
	std::vector<std::shared_ptr<PointLight>> PointLightList;
	LightBufferObject light;

	LightManager();
	LightManager(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& layout, int renderBit, glm::vec3 Pos);
	~LightManager();

	void Update(VulkanEngine& engine, std::shared_ptr<PerspectiveCamera>& camera);
	void Destory(VulkanEngine& engine);
};
