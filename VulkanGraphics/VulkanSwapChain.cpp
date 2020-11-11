#include "VulkanSwapChain.h"
#include <GLFW\glfw3.h>
#include <stdexcept>
#include <set>
#include <array>

VulkanSwapChain::VulkanSwapChain()
{
}

VulkanSwapChain::VulkanSwapChain(GLFWwindow* window, const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	SetUpSwapChain(window, device, physicalDevice, surface);
	SetUpSwapChainImageViews(device);
}

VulkanSwapChain::~VulkanSwapChain()
{
}

VkSurfaceFormatKHR VulkanSwapChain::FindSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::FindSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

void VulkanSwapChain::FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			GraphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, i, Surface, &presentSupport);

		if (presentSupport) {
			PresentFamily = i;
		}

		if (GraphicsFamily != -1 &&
			PresentFamily != -1)
		{
			break;
		}

		i++;
	}
}

void VulkanSwapChain::SetUpSwapChain(GLFWwindow* window, const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &SwapChainCapabilities);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &SurfaceFormatCount, nullptr);
	if (SurfaceFormatCount != 0)
	{
		CompatibleSwapChainFormatList.resize(SurfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &SurfaceFormatCount, CompatibleSwapChainFormatList.data());
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &PresentModeCount, nullptr);
	if (PresentModeCount != 0)
	{
		CompatiblePresentModesList.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &PresentModeCount, CompatiblePresentModesList.data());
	}

	FindQueueFamilies(physicalDevice, surface);
	SwapChainImageFormat = FindSwapSurfaceFormat(CompatibleSwapChainFormatList);
	SwapChainPresentMode = FindSwapPresentMode(CompatiblePresentModesList);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	SwapChainResolution = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

	SwapChainImageCount = SwapChainCapabilities.minImageCount + 1;
	if (SwapChainCapabilities.maxImageCount > 0 &&
		SwapChainImageCount > SwapChainCapabilities.maxImageCount)
	{
		SwapChainImageCount = SwapChainCapabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR SwapChainCreateInfo = {};
	SwapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapChainCreateInfo.surface = surface;
	SwapChainCreateInfo.minImageCount = SwapChainImageCount;
	SwapChainCreateInfo.imageFormat = SwapChainImageFormat.format;
	SwapChainCreateInfo.imageColorSpace = SwapChainImageFormat.colorSpace;
	SwapChainCreateInfo.imageExtent = SwapChainResolution;
	SwapChainCreateInfo.imageArrayLayers = 1;
	SwapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	SwapChainCreateInfo.preTransform = SwapChainCapabilities.currentTransform;
	SwapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	SwapChainCreateInfo.presentMode = SwapChainPresentMode;
	SwapChainCreateInfo.clipped = VK_TRUE;
	if (GraphicsFamily != PresentFamily)
	{
		uint32_t queueFamilyIndices[] = { GraphicsFamily, PresentFamily };

		SwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		SwapChainCreateInfo.queueFamilyIndexCount = 2;
		SwapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		SwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	if (vkCreateSwapchainKHR(device, &SwapChainCreateInfo, nullptr, &Swapchain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, Swapchain, &SwapChainImageCount, nullptr);
	SwapChainImages.resize(SwapChainImageCount);
	vkGetSwapchainImagesKHR(device, Swapchain, &SwapChainImageCount, SwapChainImages.data());
}

void VulkanSwapChain::SetUpSwapChainImageViews(const VkDevice& device)
{
	SwapChainImageViews.resize(SwapChainImageCount);
	for (uint32_t x = 0; x < SwapChainImageCount; x++)
	{
		VkImageViewCreateInfo SwapChainViewInfo = {};
		SwapChainViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		SwapChainViewInfo.image = SwapChainImages[x];
		SwapChainViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		SwapChainViewInfo.format = SwapChainImageFormat.format;
		SwapChainViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		SwapChainViewInfo.subresourceRange.baseMipLevel = 0;
		SwapChainViewInfo.subresourceRange.levelCount = 1;
		SwapChainViewInfo.subresourceRange.baseArrayLayer = 0;
		SwapChainViewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device, &SwapChainViewInfo, nullptr, &SwapChainImageViews[x]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create texture image view!");
		}
	}
}

void VulkanSwapChain::Destroy(VkDevice device)
{

	for (auto& imageView : SwapChainImageViews)
	{
		vkDestroyImageView(device, imageView, nullptr);
		imageView = VK_NULL_HANDLE;
	}

	vkDestroySwapchainKHR(device, Swapchain, nullptr);
	Swapchain = VK_NULL_HANDLE;
}

void VulkanSwapChain::UpdateSwapChain(GLFWwindow* window, const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	SetUpSwapChain(window, device, physicalDevice, surface);
	SetUpSwapChainImageViews(device);
}
