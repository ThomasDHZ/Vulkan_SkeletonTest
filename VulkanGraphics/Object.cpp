#include "Object.h"

Object::Object()
{
}

Object::Object(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, MeshTextures textures, int cubemap, VkDescriptorSetLayout& descriptorSetLayout, int renderBit)
{
	//ObjectMesh = std::make_shared<Mesh>(Mesh(renderer, textureManager, vertexdata, indicesdata, textures, cubemap, descriptorSetLayout, renderBit));
//	renderer.AddDrawableMesh(ObjectMesh);
}

Object::~Object()
{
}