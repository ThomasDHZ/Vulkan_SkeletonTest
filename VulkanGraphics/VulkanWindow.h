#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanWindow
{
private:

	bool FramebufferResized;
	unsigned int Width;
	unsigned int Height;
	GLFWwindow* GLFWindow;
public:
	VulkanWindow();
	VulkanWindow(unsigned int width, unsigned int height, const char* WindowName);
	~VulkanWindow();

	void Update();
	void CleanUp();
	static void frameBufferResizeCallBack(GLFWwindow* Window, int width, int height);

	void SetFrameBufferResizedFlag(bool flag) { FramebufferResized = flag; }

	GLFWwindow* GetWindowPtr() { return GLFWindow; }
	unsigned int GetWindowWidth() { return Width; }
	unsigned int GetWindowHeight() { return Height; }
	bool GetFrameBufferResizedFlag() { return FramebufferResized; }

};


