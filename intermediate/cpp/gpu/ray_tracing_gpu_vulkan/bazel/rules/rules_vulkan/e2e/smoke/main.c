#include <stdio.h>
#include <vulkan/vulkan.h>

int main() {
    uint32_t version = 0;
    VkResult res = vkEnumerateInstanceVersion(&version);

    if (res == VK_SUCCESS) {
        printf("Vulkan version: %u.%u.%u\n",
            VK_VERSION_MAJOR(version),
            VK_VERSION_MINOR(version),
            VK_VERSION_PATCH(version));
    } else {
        printf("Failed to query Vulkan version (error %d)\n", res);
    }

    return 0;
}
