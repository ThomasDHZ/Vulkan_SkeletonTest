#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"
#include "BaseMesh.h"
#include "Camera.h"
#include "Mesh.h"

const std::vector<Vertex> CubeVertices =
{
	{{-0.5,-0.5,-0.5}, {0,-1,0}, {0,1}},
	{{0.5,-0.5,-0.5}, {0,-1,0}, {1,1}},
	{{0.5,-0.5,0.5}, {0,-1,0}, {1,0}},
	{{-0.5,-0.5,0.5}, {0,-1,0}, {0,0}},
	{{-0.5,0.5,-0.5}, {0,0,-1}, {0,0}},
	{{0.5,0.5,-0.5}, {0,0,-1}, {1,0}},
	{{0.5,-0.5,-0.5}, {0,0,-1}, {1,1}},
	{{-0.5,-0.5,-0.5}, {0,0,-1}, {0,1}},
	{{0.5,0.5,-0.5}, {1,0,0}, {0,0}},
	{{0.5,0.5,0.5}, {1,0,0}, {1,0}},
	{{0.5,-0.5,0.5}, {1,0,0}, {1,1}},
	{{0.5,-0.5,-0.5}, {1,0,0}, {0,1}},
	{{0.5,0.5,0.5}, {0,0,1}, {1,0}},
	{{-0.5,0.5,0.5}, {0,0,1}, {0,0}},
	{{-0.5,-0.5,0.5}, {0,0,1}, {0,1}},
	{{0.5,-0.5,0.5}, {0,0,1}, {1,1}},
	{{-0.5,0.5,0.5}, {-1,0,0}, {1,0}},
	{{-0.5,0.5,-0.5}, {-1,0,0}, {0,0}},
	{{-0.5,-0.5,-0.5}, {-1,0,0}, {0,1}},
	{{-0.5,-0.5,0.5}, {-1,0,0}, {1,1}},
	{{-0.5,0.5,-0.5}, {0,1,0}, {0,1}},
	{{-0.5,0.5,0.5}, {0,1,0}, {0,0}},
	{{0.5,0.5,0.5}, {0,1,0}, {1,0}},
	{{0.5,0.5,-0.5}, {0,1,0}, {1,1}},
};

const std::vector<uint16_t> CubeIndices = {
	0,1,2, 0,2,3,
	4,5,6, 4,6,7,
	8,9,10, 8,10,11,
	12,13,14, 12,14,15,
	16,17,18, 16,18,19,
	20,21,22, 20,22,23,
};

struct MeshColor
{
	alignas(16) glm::vec3 Color;
};

class DebugLightMesh : public Mesh
{
private:
	VulkanUniformBuffer meshColorBuffer;

	virtual void CreateUniformBuffers(VulkanEngine& engine) override;
	virtual void CreateDescriptorPool(VulkanEngine& engine) override;
	virtual void CreateDescriptorSets(VulkanEngine& engine, VkDescriptorSetLayout& layout) override;
	virtual void UpdateUniformBuffer(VulkanEngine& engine, VertexMatrixObject ubo, MeshColor meshColorBuffer);

public:

	DebugLightMesh();
	DebugLightMesh(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, VkDescriptorSetLayout& layout, int renderBit);
	~DebugLightMesh();

	void Update(VulkanEngine& engine, std::shared_ptr<PerspectiveCamera>& camera, MeshColor meshColorBuffer);
	virtual void Destory(VulkanEngine& engine) override;
};
