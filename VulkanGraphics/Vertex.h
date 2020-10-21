#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Normal = glm::vec3(0.0f);
	glm::vec2 TexureCoord = glm::vec2(0.0f);
	glm::vec3 Tangant = glm::vec3(0.0f);
	glm::vec3 BiTangant = glm::vec3(0.0f);
	glm::ivec4 BoneID = glm::ivec4(0);
	glm::vec4 BoneWeights = glm::vec4(0.0f);

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription BindingDescription = {};
		BindingDescription.binding = 0;
		BindingDescription.stride = sizeof(Vertex);
		BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return BindingDescription;
	}

	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> AttributeDescriptions = {};

		VkVertexInputAttributeDescription AttributeDescription;

		AttributeDescription.binding = 0;
		AttributeDescription.location = 0;
		AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, Position);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 1;
		AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, Normal);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 2;
		AttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, TexureCoord);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 3;
		AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, Tangant);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 4;
		AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, BiTangant);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 5;
		AttributeDescription.format = VK_FORMAT_R32G32B32A32_SINT;
		AttributeDescription.offset = offsetof(Vertex, BoneID);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 6;
		AttributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, BoneWeights);
		AttributeDescriptions.emplace_back(AttributeDescription);

		return AttributeDescriptions;
	}
};