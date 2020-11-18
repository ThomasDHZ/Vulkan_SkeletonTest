#include "TestArea.h"
#include <VulkanGraphics.vcxproj>

int main() {
	VulkanGraphics app = VulkanGraphics(1920, 1080, "VulkanGraphics");

	try {
		app.MainLoop();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
