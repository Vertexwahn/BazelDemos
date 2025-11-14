#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VKFW_NO_STRUCT_CONSTRUCTORS

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <functional>
#include <list>
#include "vulkan_settings.h"
#include "scene.h"
#include "render_call_info.h"

struct VulkanImage {
    vk::Image image;
    vk::DeviceMemory memory;
    vk::ImageView imageView;
};

struct VulkanBuffer {
    vk::Buffer buffer;
    vk::DeviceMemory memory;
};

struct VulkanAccelerationStructure {
    vk::AccelerationStructureKHR accelerationStructure;
    VulkanBuffer structureBuffer;
    VulkanBuffer scratchBuffer;
    VulkanBuffer instancesBuffer;
};


class Vulkan {
public:
    Vulkan(VulkanSettings settings, Scene scene);

    ~Vulkan();

    void update();

    void render(const RenderCallInfo &renderCallInfo);

    [[nodiscard]] bool shouldExit() const;


private:
    VulkanSettings settings;
    Scene scene;
    std::vector<vk::AabbPositionsKHR> aabbs;

    const vk::Format swapChainImageFormat = vk::Format::eR8G8B8A8Unorm;
    const vk::Format summedPixelColorImageFormat = vk::Format::eR32G32B32A32Sfloat;
    const vk::ColorSpaceKHR colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
    const vk::PresentModeKHR presentMode = vk::PresentModeKHR::eImmediate;

    const std::vector<const char*> requiredInstanceExtensions = {
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    };

    const std::vector<const char*> requiredDeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
            VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
            VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
            VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
            VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
            VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
            VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
            VK_KHR_MAINTENANCE3_EXTENSION_NAME
    };


    GLFWwindow* window;
    vk::Instance instance;
    vk::SurfaceKHR surface;
    vk::PhysicalDevice physicalDevice;
    vk::Device device;

    uint32_t presentQueueFamily = 0, computeQueueFamily = 0;
    vk::Queue presentQueue, computeQueue;

    vk::CommandPool commandPool;

    vk::SwapchainKHR swapChain;
    vk::Extent2D swapChainExtent;
    std::vector<vk::Image> swapChainImages;
    std::vector<vk::ImageView> swapChainImageViews;

    vk::DescriptorSetLayout rtDescriptorSetLayout;
    vk::DescriptorPool rtDescriptorPool;
    vk::DescriptorSet rtDescriptorSet;
    vk::PipelineLayout rtPipelineLayout;
    vk::Pipeline rtPipeline;

    std::vector<vk::CommandBuffer> commandBuffers;

    vk::Fence fence;
    std::list<vk::Semaphore> semaphores;

    VulkanImage renderTargetImage;
    VulkanImage summedPixelColorImage;

    VulkanBuffer aabbBuffer;

    VulkanAccelerationStructure bottomAccelerationStructure;
    VulkanAccelerationStructure topAccelerationStructure;

    VulkanBuffer shaderBindingTableBuffer;
    vk::StridedDeviceAddressRegionKHR sbtRayGenAddressRegion, sbtHitAddressRegion, sbtMissAddressRegion;

    VulkanBuffer sphereBuffer;
    VulkanBuffer renderCallInfoBuffer;

    void createWindow();

    void createInstance();

    void createSurface();

    void pickPhysicalDevice();

    void findQueueFamilies();

    void createLogicalDevice();

    void createCommandPool();

    void createSwapChain();

    [[nodiscard]] vk::ImageView createImageView(const vk::Image &image, const vk::Format &format) const;

    void createDescriptorSetLayout();

    void createDescriptorPool();

    void createDescriptorSet();

    void createPipelineLayout();

    void createRTPipeline();

    [[nodiscard]] static std::vector<char> readBinaryFile(const std::string &path);

    void createCommandBuffer();

    void createFence();

    void createSemaphore();

    void createImages();

    [[nodiscard]] uint32_t findMemoryTypeIndex(const uint32_t &memoryTypeBits,
                                               const vk::MemoryPropertyFlags &properties);

    [[nodiscard]] vk::ImageMemoryBarrier getImagePipelineBarrier(
            const vk::AccessFlagBits &srcAccessFlags, const vk::AccessFlagBits &dstAccessFlags,
            const vk::ImageLayout &oldLayout, const vk::ImageLayout &newLayout, const vk::Image &image) const;

    [[nodiscard]] VulkanImage createImage(const vk::Format &format,
                                          const vk::Flags<vk::ImageUsageFlagBits> &usageFlagBits);

    void destroyImage(const VulkanImage &image) const;

    [[nodiscard]] VulkanBuffer createBuffer(const vk::DeviceSize &size, const vk::Flags<vk::BufferUsageFlagBits> &usage,
                                            const vk::Flags<vk::MemoryPropertyFlagBits> &memoryProperty);

    void destroyBuffer(const VulkanBuffer &buffer) const;

    void executeSingleTimeCommand(const std::function<void(const vk::CommandBuffer &singleTimeCommandBuffer)> &c);

    void createAABBBuffer();

    void createBottomAccelerationStructure();

    void createTopAccelerationStructure();

    void destroyAccelerationStructure(const VulkanAccelerationStructure &accelerationStructure);

    [[nodiscard]] vk::ShaderModule createShaderModule(const std::string &path) const;

    void createShaderBindingTable();

    [[nodiscard]] vk::PhysicalDeviceRayTracingPipelinePropertiesKHR getRayTracingProperties() const;

    void createSphereBuffer();

    [[nodiscard]] static vk::AabbPositionsKHR getAABBFromSphere(const glm::vec4 &geometry);

    void createRenderCallInfoBuffer();

    void updateRenderCallInfoBuffer(const RenderCallInfo &renderCallInfo);

};
