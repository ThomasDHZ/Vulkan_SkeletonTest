#pragma once
#include "DebugLightMesh.h"

const std::vector<Vertex> LightVertices = {
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
	{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
};

const std::vector<uint16_t> LightIndices = {
	0, 1, 2, 2, 3, 0
};

class PointLight
{
private:
	std::shared_ptr<DebugLightMesh> DebugMesh;
public:
	PointLightStruct pointLight;

	PointLight();
	PointLight(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& layout, int renderBit, glm::vec3 position);
	~PointLight();

	void Update(VulkanEngine& engine, std::shared_ptr<PerspectiveCamera>& camera);
	void Destory(VulkanEngine& engine);
};
