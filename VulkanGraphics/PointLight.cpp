#include "PointLight.h"

PointLight::PointLight()
{
}

PointLight::PointLight(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& layout, int renderBit, glm::vec3 position)
{
	pointLight.position = glm::vec3(1.0f, 10.0f, 1.2f);
	pointLight.ambient = glm::vec3(0.8f, 0.8f, 0.86f);
	pointLight.InUseFlag = 1;
	DebugMesh = std::make_shared<DebugLightMesh>(DebugLightMesh(engine, textureManager, layout, renderBit));

}

PointLight::~PointLight()
{
}

void PointLight::Update(VulkanEngine& engine, std::shared_ptr<PerspectiveCamera>& camera)
{
	MeshColor color = {};
	color.Color = pointLight.diffuse;

	DebugMesh->MeshPosition = pointLight.position;
	DebugMesh->Update(engine, camera, color);
}

void PointLight::Destory(VulkanEngine& engine)
{
	DebugMesh->Destory(engine);
}