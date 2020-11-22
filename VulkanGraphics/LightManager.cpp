#include "LightManager.h"
#include "OrthographicCamera.h"

LightManager::LightManager()
{
}

LightManager::LightManager(VulkanEngine& engine, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& layout, int renderBit, glm::vec3 Pos)
{
	light.dLight.direction = glm::vec3(0.5f, 1.0f, 0.3f);

	PointLightList.emplace_back(std::make_shared<PointLight>(PointLight(engine, textureManager, layout, renderBit, Pos)));
	PointLightList.emplace_back(std::make_shared<PointLight>(PointLight(engine, textureManager, layout, renderBit, Pos)));
	PointLightList.emplace_back(std::make_shared<PointLight>(PointLight(engine, textureManager, layout, renderBit, Pos)));
	PointLightList.emplace_back(std::make_shared<PointLight>(PointLight(engine, textureManager, layout, renderBit, Pos)));
}

LightManager::~LightManager()
{
}

void LightManager::Update(VulkanEngine& engine, std::shared_ptr<PerspectiveCamera>& camera)
{
	light.viewPos = static_cast<PerspectiveCamera*>(camera.get())->GetPosition();

	for (int x = 0; x < PointLightList.size(); x++)
	{
		PointLightList[x]->Update(engine, camera);
		light.pLight[x] = PointLightList[x]->pointLight;
	}
}

void LightManager::Destory(VulkanEngine& engine)
{
	for (auto pointLight : PointLightList)
	{
		pointLight->Destory(engine);
	}
}