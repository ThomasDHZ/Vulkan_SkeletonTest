#pragma once
#include "VulkanEngine.h"
#include "RenderedDepthTexture.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"

static void check_vk_result(VkResult err)
{
	if (err == 0) return;
	printf("VkResult %d\n", err);
	if (err < 0)
		abort();
}

class InterfaceRenderPass
{
	private:
		VkRenderPass RenderPass;
		VkDescriptorPool ImGuiDescriptorPool;
		VkCommandPool ImGuiCommandPool;

		void CreateRenderPass(VulkanEngine& engine);
		void CreateRendererFramebuffers(VulkanEngine& engine);

	public:
		InterfaceRenderPass();
		InterfaceRenderPass(VulkanEngine& engine, GLFWwindow* window);
		~InterfaceRenderPass();

		std::vector<VkCommandBuffer> ImGuiCommandBuffers;
		std::vector<VkFramebuffer> SwapChainFramebuffers;

		void Draw(VulkanEngine& engine);
		void UpdateSwapChain(VulkanEngine& engine);
		void Destroy(VulkanEngine& engine);

		VkRenderPass GetRenderPass() { return RenderPass; }
};

