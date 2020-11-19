#include "VulkanGraphics.h"
int main() {
    VulkanGraphics app;

    try 
    {
        app.MainLoop();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

