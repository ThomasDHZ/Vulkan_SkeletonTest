#pragma once
#include "Object2D.h"
#include "Mesh2D.h"
#include "Animation2D.h"
#include <map>
#include "BoxCollider.h"
#include "ColliderObject.h"


class Sprite : public Object2D
{
private:

protected:
	void SetUpSprite(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex> SpriteVertices, const MeshTextures& SpriteTextures, glm::vec2 StartPos, VkDescriptorSetLayout& layout);
	void SetUpSprite(VulkanEngine& engine, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex> SpriteVertices, const MeshTextures& SpriteTextures, glm::vec2 StartPos, CustomBuffer custom, VkDescriptorSetLayout& layout);

public:

	glm::ivec2 Velocity;
	Animation2D CurrentAni;

	Sprite();
	Sprite(VulkanEngine& engine, int renderBitFlags);
	~Sprite();

	bool OnGroundCheck(std::vector<std::shared_ptr<Object2D>>& ObjectList);
	void ApplyGravity(std::vector<std::shared_ptr<Object2D>>& ObjectList, float dt);
	void Move(std::vector<std::shared_ptr<Object2D>>& ObjectList, glm::vec3 MoveDirection);

	virtual void Update(VulkanEngine& engine, float dt, std::shared_ptr<Camera> camera, LightBufferObject light) override;
	virtual void Destory(VulkanEngine& engine) override;
};