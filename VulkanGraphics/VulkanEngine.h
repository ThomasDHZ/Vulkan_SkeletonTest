#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>
#include <GLFW\glfw3.h>
#include "VulkanDebugger.h"
#include "VulkanSwapChain.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
const int MAX_FRAMES_IN_FLIGHT = 2;

struct VulkanRendererSettings
{
	bool ShowMeshLines = false;
	bool ShowSkyBox = true;
	bool ShowDebugLightMesh = true;
	bool ShowDebugCollisionMesh = false;
	bool TwoDMode = false;

	bool operator!=(const VulkanRendererSettings& OtherSettings)
	{
		return (ShowMeshLines != OtherSettings.ShowMeshLines ||
			ShowSkyBox != OtherSettings.ShowSkyBox ||
			ShowDebugLightMesh != OtherSettings.ShowDebugLightMesh ||
			ShowDebugCollisionMesh != OtherSettings.ShowDebugCollisionMesh ||
			TwoDMode != OtherSettings.TwoDMode);
	}
};

struct VulkanDriver
{
	std::shared_ptr<VkInstance> Instance;
	std::shared_ptr<VkDevice> Device;
	std::shared_ptr<VkPhysicalDevice> PhysicalDevice;
	std::shared_ptr<VkSurfaceKHR> Surface;
	std::shared_ptr<VkQueue> GraphicsQueue;
	std::shared_ptr<VkQueue> PresentQueue;
	std::shared_ptr<VkCommandPool> RenderCommandPool;
	std::shared_ptr<VkSwapchainKHR> SwapChain;
	std::shared_ptr<std::vector<VkImage>> SwapChainImages;
	std::shared_ptr<std::vector<VkImageView>> SwapChainImageViews;
	std::shared_ptr<VkExtent2D> SwapChainResolution;
	VkSurfaceFormatKHR SwapChainImageFormat;
	uint32_t SwapChainMinImageCount;
	uint32_t SwapChainImageCount;
};

class VulkanEngine
{
private:
protected:

	struct VulkanSemaphores
	{
		VkSemaphore ImageAcquiredSemaphore;
		VkSemaphore RenderCompleteSemaphore;

		void Destory(VkDevice device)
		{
			vkDestroySemaphore(device, RenderCompleteSemaphore, nullptr);
			vkDestroySemaphore(device, ImageAcquiredSemaphore, nullptr);

			RenderCompleteSemaphore = VK_NULL_HANDLE;
			ImageAcquiredSemaphore = VK_NULL_HANDLE;
		}
	};
	VulkanDebugger VulkanDebug;

	std::vector<VkLayerProperties> VulkanLayers;

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();

	std::vector<const char*> getRequiredExtensions();
	bool isDeviceSuitable(VkPhysicalDevice GPUDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice GPUDevice);

	void FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface);

	void InitializeCommandPool();
	void InitializeSyncObjects();

	
public:

	int GraphicsFamily = -1;
	int PresentFamily = -1;

	std::vector<VulkanSemaphores> vulkanSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	VkInstance Instance = VK_NULL_HANDLE;
	VkDevice Device = VK_NULL_HANDLE;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR Surface = VK_NULL_HANDLE;
	VkQueue GraphicsQueue = VK_NULL_HANDLE;
	VkQueue PresentQueue = VK_NULL_HANDLE;
	VkCommandPool RenderCommandPool = VK_NULL_HANDLE;
	VulkanSwapChain SwapChain;

	VulkanEngine();
	VulkanEngine(GLFWwindow* window);
	~VulkanEngine();

	uint32_t DrawFrame = 0;
	VulkanRendererSettings Settings;
	void Destory();

	VkInstance GetVulkanInstance() { return Instance; }
	VkDevice GetVulkanDevice() { return Device; }
	VkPhysicalDevice GetVulkanPhysicalDevice() { return PhysicalDevice; }
	VkSurfaceKHR GetVulkanSurface() { return Surface; }
	VkQueue GetVulkanGraphicsQueue() { return GraphicsQueue; }
	VkQueue GetVulkanPresentQueue() { return PresentQueue; }
	VkCommandPool GetRenderCommandPool() { return RenderCommandPool; }
	VkSwapchainKHR GetSwapChain() { return SwapChain.GetSwapChain(); }
	std::vector<VkImage> GetSwapChainImages() { return SwapChain.GetSwapChainImages(); }
	std::vector<VkImageView> GetSwapChainImageViews() { return SwapChain.GetSwapChainImageViews(); }
	VkExtent2D GetSwapChainResolution() { return SwapChain.GetSwapChainResolution(); }
	VkSurfaceFormatKHR GetSwapChainImageFormat() { return SwapChain.GetSwapChainImageFormat(); }
	uint32_t GetSwapChainMinImageCount() { return SwapChain.GetSwapChainMinImageCount(); }
	uint32_t GetSwapChainImageCount() { return SwapChain.GetSwapChainImageCount(); }
};

