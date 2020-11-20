#include "Object2D.h"

Object2D::Object2D()
{
}

Object2D::~Object2D()
{
}

void Object2D::AnimationHandler()
{
}

void Object2D::Update(VulkanEngine& engine, float dt, std::shared_ptr<Camera> camera, LightBufferObject Lightbuffer)
{
	AnimationHandler();
	ObjectMesh->Update(engine, camera, Lightbuffer);
	for (auto& collider : ObjectColliderList)
	{
		collider->Update(ObjectMesh->GetPosition3D());
	}
}

void Object2D::Collision(VulkanEngine& engine, std::vector<std::shared_ptr<Object2D>>& ObjectList)
{
}

void Object2D::Destory(VulkanEngine& engine)
{
	ObjectMesh->Destory(engine);
}

void Object2D::SetPosition2D(glm::vec2 Pos)
{
	ObjectMesh->SetPosition3D(glm::vec3(Pos, ObjectMesh->GetPosition3D().z));
}

void Object2D::SetPosition2D(float x, float y)
{
	ObjectMesh->SetPosition3D(glm::vec3(x, y, ObjectMesh->GetPosition3D().z));
}

void Object2D::SetPosition3D(glm::vec3 Pos)
{
	ObjectMesh->SetPosition3D(Pos);
}

void Object2D::SetPosition3D(float x, float y, float z)
{
	ObjectMesh->SetPosition3D(glm::vec3(x, y, z));
}