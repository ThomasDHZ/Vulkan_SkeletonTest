#pragma once
#include "Object.h"
#include "BoxCollider.h"
#include "Mesh2D.h"
#include "ColliderObject.h"

struct ColisionGeo
{
	std::vector<glm::vec3> CollisionVertexs;
	std::vector<uint16_t> CollisionIndices;
};

enum ObjectFlags
{
	None = 0x00,
	Player = 0x01,
	Enemy = 0x02,
	Wall = 0x04,
	Collectible = 0x08,
	ApplyGravity = 0x10
};

class Object2D : public Object
{
private:
protected:
	static constexpr glm::vec3 Gravity = glm::vec3(0.0f, -1.50f, 0.0f);
	int RenderBitFlags;

	glm::vec2 UVOffset = glm::vec2(0.0f);

public:
	std::shared_ptr<Mesh2D> ObjectMesh;
	std::vector<std::shared_ptr<ColliderObject>> ObjectColliderList;

	Object2D();
	~Object2D();

	virtual void AnimationHandler();
	virtual void Update(VulkanEngine& engine, float dt, std::shared_ptr<Camera> camera, LightBufferObject Lightbuffer);
	virtual void Collision(VulkanEngine& engine, std::vector<std::shared_ptr<Object2D>>& ObjectList);
	virtual void Destory(VulkanEngine& engine);

	void SetPosition2D(glm::vec2 Pos);
	void SetPosition3D(glm::vec3 Pos);
	void SetPosition2D(float x, float y);
	void SetPosition3D(float x, float y, float z);

	int ObjectFlagBits;
	bool DestoryObjectFlag = false;

	glm::vec2 GetPosition2D() { return glm::vec2(ObjectMesh->GetPosition2D().x, ObjectMesh->GetPosition2D().y); }
	glm::vec3 GetPosition3D() { return ObjectMesh->GetPosition3D(); }
	float* GetUVOffsetPtr() { return &UVOffset.x; };
};
