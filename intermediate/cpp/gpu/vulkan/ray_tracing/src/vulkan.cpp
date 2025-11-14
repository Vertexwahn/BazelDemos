#include "vulkan.h"
#include <iostream>
#include <set>
#include <fstream>
#include <algorithm>

Vulkan::Vulkan(VulkanSettings settings, Scene scene) :
        settings(settings), scene(scene), window(nullptr) {

    // Initialize the dynamic dispatcher
    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    aabbs.reserve(scene.sphereAmount);
    for (int i = 0; i < scene.sphereAmount; i++) {
        aabbs.push_back(getAABBFromSphere(scene.spheres[i].geometry));
    }

    createWindow();
    createInstance();
    createSurface();
    pickPhysicalDevice();
    findQueueFamilies();
    createLogicalDevice();

    createCommandPool();
    createSwapChain();
    createImages();

    createAABBBuffer();
    createBottomAccelerationStructure();
    createTopAccelerationStructure();

    createSphereBuffer();
    createRenderCallInfoBuffer();

    createDescriptorSetLayout();
    createDescriptorPool();
    createDescriptorSet();
    createPipelineLayout();
    createRTPipeline();

    createShaderBindingTable();
    createCommandBuffer();

    createFence();
    createSemaphore();
}

Vulkan::~Vulkan() {
    std::ranges::for_each(semaphores, [this](auto semaphore) {device.destroySemaphore(semaphore); });
    device.destroyFence(fence);

    device.destroyPipeline(rtPipeline);
    device.destroyPipelineLayout(rtPipelineLayout);
    device.destroyDescriptorSetLayout(rtDescriptorSetLayout);
    device.destroyDescriptorPool(rtDescriptorPool);

    destroyAccelerationStructure(topAccelerationStructure);
    destroyAccelerationStructure(bottomAccelerationStructure);

    destroyBuffer(sphereBuffer);
    destroyBuffer(aabbBuffer);
    destroyBuffer(shaderBindingTableBuffer);
    destroyBuffer(renderCallInfoBuffer);

    std::ranges::for_each(swapChainImageViews, [this](auto swapChainImageView) {device.destroyImageView(swapChainImageView); });
    device.destroySwapchainKHR(swapChain);
    device.destroyCommandPool(commandPool);

    destroyImage(renderTargetImage);
    destroyImage(summedPixelColorImage);

    device.destroy();
    instance.destroySurfaceKHR(surface);
    instance.destroy();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Vulkan::update() {
    glfwPollEvents();
}

void Vulkan::render(const RenderCallInfo &renderCallInfo) {
    updateRenderCallInfoBuffer(renderCallInfo);

    uint32_t swapChainImageIndex = 0;
    auto semaphore = *semaphores.begin();
    semaphores.pop_front();
    semaphores.push_back(semaphore);
    if (auto [result, index] = device.acquireNextImageKHR(swapChain, UINT64_MAX, semaphore);
        result == vk::Result::eSuccess || result == vk::Result::eSuboptimalKHR) {
            swapChainImageIndex = index;
    }
    else {
        throw std::runtime_error{ "failed to acquire next image" };
    }

    device.resetFences(fence);

    vk::SubmitInfo submitInfo = {
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffers[swapChainImageIndex]
    };

    computeQueue.submit(1, &submitInfo, fence);

    device.waitForFences(1, &fence, true, UINT64_MAX);
    device.resetFences(fence);

    vk::PresentInfoKHR presentInfo = {
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &semaphore,
            .swapchainCount = 1,
            .pSwapchains = &swapChain,
            .pImageIndices = &swapChainImageIndex
    };

    presentQueue.presentKHR(presentInfo);
    vk::SubmitInfo wait_to_present = {
    };
    presentQueue.submit(wait_to_present, fence);
    device.waitForFences(1, &fence, true, UINT64_MAX);
    device.resetFences(fence);
}

bool Vulkan::shouldExit() const {
    return glfwWindowShouldClose(window);
}

void Vulkan::createWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(settings.windowWidth, settings.windowHeight, "GPU Ray Tracing (Vulkan)",
                              nullptr, nullptr);
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                void* pUserData) {
    std::cout << "["
              << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) << " | "
              << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes )) << "]:\n"
              << "id      : " << pCallbackData->pMessageIdName << "\n"
              << "message : " << pCallbackData->pMessage << "\n"
              << std::endl;

    return false;
}

void Vulkan::createInstance() {
    vk::ApplicationInfo applicationInfo = {
            .pApplicationName = "Ray Tracing (Vulkan)",
            .applicationVersion = 1,
            .pEngineName = "Ray Tracing (Vulkan)",
            .engineVersion = 1,
            .apiVersion = VK_API_VERSION_1_3
    };

    std::vector<const char*> enabledExtensions;

    uint32_t windowExtensionCount;
    const char** windowExtensions = glfwGetRequiredInstanceExtensions(&windowExtensionCount);

    enabledExtensions.insert(enabledExtensions.end(), windowExtensions, windowExtensions + windowExtensionCount);
    enabledExtensions.insert(enabledExtensions.end(), requiredInstanceExtensions.begin(),
                             requiredInstanceExtensions.end());

    std::vector<const char*> enabledLayers =
            {"VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_monitor", "VK_LAYER_KHRONOS_synchronization2"};

#ifndef _DEBUG
    enabledLayers.clear();
#endif

    vk::DebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
    debugMessengerInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
    debugMessengerInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                                     vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                     vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral;
    debugMessengerInfo.pfnUserCallback = reinterpret_cast<vk::PFN_DebugUtilsMessengerCallbackEXT>(debugMessageFunc);
    debugMessengerInfo.pUserData = nullptr;

    vk::InstanceCreateInfo instanceCreateInfo = {
            .pNext = &debugMessengerInfo,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = static_cast<uint32_t>(enabledLayers.size()),
            .ppEnabledLayerNames = enabledLayers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size()),
            .ppEnabledExtensionNames = enabledExtensions.data(),
    };

#ifndef _DEBUG
    instanceCreateInfo.pNext = nullptr;
#endif

    instance = vk::createInstance(instanceCreateInfo);
    
    // Initialize the dispatcher with the instance
    VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
}

void Vulkan::createSurface() {
    glfwCreateWindowSurface(instance, window, nullptr, reinterpret_cast<VkSurfaceKHR*>(&surface));
}

void Vulkan::pickPhysicalDevice() {
    std::vector<vk::PhysicalDevice> allPhysicalDevices = instance.enumeratePhysicalDevices();

    if (allPhysicalDevices.empty()) {
        throw std::runtime_error("No GPU with Vulkan support found!");
    }

    std::vector<vk::PhysicalDevice> withRequiredExtensionsPhysicalDevices{};
    for (const vk::PhysicalDevice &d: allPhysicalDevices) {
        std::vector<vk::ExtensionProperties> availableExtensions = d.enumerateDeviceExtensionProperties();
        std::set<std::string> requiredExtensions(requiredDeviceExtensions.begin(), requiredDeviceExtensions.end());

        for (const vk::ExtensionProperties &extension: availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        if (requiredExtensions.empty()) {
            withRequiredExtensionsPhysicalDevices.push_back(d);
        }
    }

    for (const vk::PhysicalDevice& d : withRequiredExtensionsPhysicalDevices) {
        if (d.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
            physicalDevice = d;
            return;
        }
    }

    if (withRequiredExtensionsPhysicalDevices.size() > 0) {
        physicalDevice = withRequiredExtensionsPhysicalDevices[0];
        return;
    }

    throw std::runtime_error("No GPU supporting all required features found!");
}

void Vulkan::findQueueFamilies() {
    std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

    bool computeFamilyFound = false;
    bool presentFamilyFound = false;

    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        bool supportsGraphics = (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
                                == vk::QueueFlagBits::eGraphics;
        bool supportsCompute = (queueFamilies[i].queueFlags & vk::QueueFlagBits::eCompute)
                               == vk::QueueFlagBits::eCompute;
        bool supportsPresenting = physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface);

        if (supportsCompute && !supportsGraphics && !computeFamilyFound) {
            computeQueueFamily = i;
            computeFamilyFound = true;
            continue;
        }

        if (supportsPresenting && !presentFamilyFound) {
            presentQueueFamily = i;
            presentFamilyFound = true;
        }

        if (computeFamilyFound && presentFamilyFound)
            break;
    }
}

void Vulkan::createLogicalDevice() {
    float queuePriority = 1.0f;
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {
            {
                    .queueFamilyIndex = presentQueueFamily,
                    .queueCount = 1,
                    .pQueuePriorities = &queuePriority
            },
            {
                    .queueFamilyIndex = computeQueueFamily,
                    .queueCount = 1,
                    .pQueuePriorities = &queuePriority
            }
    };

    vk::PhysicalDeviceFeatures deviceFeatures = {};

    vk::PhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures = {
            .bufferDeviceAddress = true,
            .bufferDeviceAddressCaptureReplay = false,
            .bufferDeviceAddressMultiDevice = false
    };

    vk::PhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingPipelineFeatures = {
            .pNext = &bufferDeviceAddressFeatures,
            .rayTracingPipeline = true
    };

    vk::PhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures = {
            .pNext = &rayTracingPipelineFeatures,
            .accelerationStructure = true,
            .accelerationStructureCaptureReplay = true,
            .accelerationStructureIndirectBuild = false,
            .accelerationStructureHostCommands = false,
            .descriptorBindingAccelerationStructureUpdateAfterBind = false
    };

    vk::DeviceCreateInfo deviceCreateInfo = {
            .pNext = &accelerationStructureFeatures,
            .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
            .pQueueCreateInfos = queueCreateInfos.data(),
            .enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensions.size()),
            .ppEnabledExtensionNames = requiredDeviceExtensions.data(),
            .pEnabledFeatures = &deviceFeatures
    };

    device = physicalDevice.createDevice(deviceCreateInfo);

    // Initialize the dispatcher with the device
    VULKAN_HPP_DEFAULT_DISPATCHER.init(device);

    computeQueue = device.getQueue(computeQueueFamily, 0);
    presentQueue = device.getQueue(presentQueueFamily, 0);
}

void Vulkan::createCommandPool() {
    commandPool = device.createCommandPool({.queueFamilyIndex = computeQueueFamily});
}

void Vulkan::createSwapChain() {
    auto surface_capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
    swapChainExtent = surface_capabilities.currentExtent;
    vk::SwapchainCreateInfoKHR swapChainCreateInfo = {
            .surface = surface,
            .minImageCount = surface_capabilities.minImageCount,
            .imageFormat = swapChainImageFormat,
            .imageColorSpace = colorSpace,
            .imageExtent = swapChainExtent,
            .imageArrayLayers = 1,
            .imageUsage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
            .imageSharingMode = vk::SharingMode::eExclusive,
            .preTransform = surface_capabilities.currentTransform,
            .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
            .presentMode = presentMode,
            .clipped = true,
            .oldSwapchain = nullptr
    };

    swapChain = device.createSwapchainKHR(swapChainCreateInfo);

    // swap chain images
    swapChainImages = device.getSwapchainImagesKHR(swapChain);
    swapChainImageViews.resize(swapChainImages.size());
    std::ranges::transform(swapChainImages, swapChainImageViews.begin(),
        [this](auto swapChainImage) {
            return createImageView(swapChainImage, swapChainImageFormat);
        }
    );
}

vk::ImageView Vulkan::createImageView(const vk::Image &image, const vk::Format &format) const {
    return device.createImageView(
            {
                    .image = image,
                    .viewType = vk::ImageViewType::e2D,
                    .format = format,
                    .subresourceRange = {
                            .aspectMask = vk::ImageAspectFlagBits::eColor,
                            .baseMipLevel = 0,
                            .levelCount = 1,
                            .baseArrayLayer = 0,
                            .layerCount = 1
                    }
            });
}

void Vulkan::createDescriptorSetLayout() {
    std::vector<vk::DescriptorSetLayoutBinding> bindings = {
            {
                    .binding = 0,
                    .descriptorType = vk::DescriptorType::eStorageImage,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eRaygenKHR
            },
            {
                    .binding = 1,
                    .descriptorType = vk::DescriptorType::eAccelerationStructureKHR,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eRaygenKHR
            },
            {
                    .binding = 2,
                    .descriptorType = vk::DescriptorType::eUniformBuffer,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eIntersectionKHR |
                                  vk::ShaderStageFlagBits::eClosestHitKHR
            },
            {
                    .binding = 3,
                    .descriptorType = vk::DescriptorType::eStorageImage,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eRaygenKHR
            },
            {
                    .binding = 4,
                    .descriptorType = vk::DescriptorType::eUniformBuffer,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eRaygenKHR
            }
    };

    rtDescriptorSetLayout = device.createDescriptorSetLayout(
            {
                    .bindingCount = static_cast<uint32_t>(bindings.size()),
                    .pBindings = bindings.data()
            });
}

void Vulkan::createDescriptorPool() {
    std::vector<vk::DescriptorPoolSize> poolSizes = {
            {
                    .type = vk::DescriptorType::eStorageImage,
                    .descriptorCount = 2
            },
            {
                    .type = vk::DescriptorType::eAccelerationStructureKHR,
                    .descriptorCount = 1
            },
            {
                    .type = vk::DescriptorType::eUniformBuffer,
                    .descriptorCount = 2
            }
    };

    rtDescriptorPool = device.createDescriptorPool(
            {
                    .maxSets = 1,
                    .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
                    .pPoolSizes = poolSizes.data()
            });
}

void Vulkan::createDescriptorSet() {
    rtDescriptorSet = device.allocateDescriptorSets(
            {
                    .descriptorPool = rtDescriptorPool,
                    .descriptorSetCount = 1,
                    .pSetLayouts = &rtDescriptorSetLayout
            }).front();


    vk::DescriptorImageInfo renderTargetImageInfo = {
            .imageView = renderTargetImage.imageView,
            .imageLayout = vk::ImageLayout::eGeneral
    };

    vk::WriteDescriptorSetAccelerationStructureKHR accelerationStructureInfo = {
            .accelerationStructureCount = 1,
            .pAccelerationStructures = &topAccelerationStructure.accelerationStructure
    };

    vk::DescriptorBufferInfo sphereBufferInfo = {
            .buffer = sphereBuffer.buffer,
            .offset = 0,
            .range = sizeof(Sphere) * MAX_SPHERE_AMOUNT
    };

    vk::DescriptorImageInfo summedPixelColorImageInfo = {
            .imageView = summedPixelColorImage.imageView,
            .imageLayout = vk::ImageLayout::eGeneral
    };

    vk::DescriptorBufferInfo renderCallInfoBufferInfo = {
            .buffer = renderCallInfoBuffer.buffer,
            .offset = 0,
            .range = sizeof(RenderCallInfo)
    };

    std::vector<vk::WriteDescriptorSet> descriptorWrites = {
            {
                    .dstSet = rtDescriptorSet,
                    .dstBinding = 0,
                    .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = vk::DescriptorType::eStorageImage,
                    .pImageInfo = &renderTargetImageInfo
            },
            {
                    .pNext = &accelerationStructureInfo,
                    .dstSet = rtDescriptorSet,
                    .dstBinding = 1,
                    .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = vk::DescriptorType::eAccelerationStructureKHR
            },
            {
                    .dstSet = rtDescriptorSet,
                    .dstBinding = 2,
                    .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = vk::DescriptorType::eUniformBuffer,
                    .pBufferInfo = &sphereBufferInfo
            },
            {
                    .dstSet = rtDescriptorSet,
                    .dstBinding = 3,
                    .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = vk::DescriptorType::eStorageImage,
                    .pImageInfo = &summedPixelColorImageInfo
            },
            {
                    .dstSet = rtDescriptorSet,
                    .dstBinding = 4,
                    .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = vk::DescriptorType::eUniformBuffer,
                    .pBufferInfo = &renderCallInfoBufferInfo
            }
    };

    device.updateDescriptorSets(static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(),
                                0, nullptr);
}

void Vulkan::createPipelineLayout() {
    rtPipelineLayout = device.createPipelineLayout(
            {
                    .setLayoutCount = 1,
                    .pSetLayouts = &rtDescriptorSetLayout,
                    .pushConstantRangeCount = 0,
                    .pPushConstantRanges = nullptr
            });
}

void Vulkan::createRTPipeline() {
    vk::ShaderModule raygenModule = createShaderModule("shader_rgen.out");
    vk::ShaderModule intModule = createShaderModule("shader_rint.out");
    vk::ShaderModule chitModule = createShaderModule("shader_rchit.out");
    vk::ShaderModule missModule = createShaderModule("shader_rmiss.out");

    std::vector<vk::PipelineShaderStageCreateInfo> stages = {
            {
                    .stage = vk::ShaderStageFlagBits::eRaygenKHR,
                    .module = raygenModule,
                    .pName = "main"
            },
            {
                    .stage = vk::ShaderStageFlagBits::eIntersectionKHR,
                    .module = intModule,
                    .pName = "main"
            },
            {
                    .stage = vk::ShaderStageFlagBits::eMissKHR,
                    .module = missModule,
                    .pName = "main"
            },
            {
                    .stage = vk::ShaderStageFlagBits::eClosestHitKHR,
                    .module = chitModule,
                    .pName = "main"
            }
    };

    std::vector<vk::RayTracingShaderGroupCreateInfoKHR> groups = {
            {
                    .type = vk::RayTracingShaderGroupTypeKHR::eGeneral,
                    .generalShader = 0,
                    .closestHitShader = VK_SHADER_UNUSED_KHR,
                    .anyHitShader = VK_SHADER_UNUSED_KHR,
                    .intersectionShader = VK_SHADER_UNUSED_KHR
            },
            {
                    .type = vk::RayTracingShaderGroupTypeKHR::eGeneral,
                    .generalShader = 2,
                    .closestHitShader = VK_SHADER_UNUSED_KHR,
                    .anyHitShader = VK_SHADER_UNUSED_KHR,
                    .intersectionShader = VK_SHADER_UNUSED_KHR
            },
            {
                    .type = vk::RayTracingShaderGroupTypeKHR::eProceduralHitGroup,
                    .generalShader = VK_SHADER_UNUSED_KHR,
                    .closestHitShader = 3,
                    .anyHitShader = VK_SHADER_UNUSED_KHR,
                    .intersectionShader = 1
            }
    };

    vk::PipelineLibraryCreateInfoKHR libraryCreateInfo = {.libraryCount = 0};

    vk::RayTracingPipelineCreateInfoKHR pipelineCreateInfo = {
            .stageCount = static_cast<uint32_t>(stages.size()),
            .pStages = stages.data(),
            .groupCount = static_cast<uint32_t>(groups.size()),
            .pGroups = groups.data(),
            .maxPipelineRayRecursionDepth = getRayTracingProperties().maxRayRecursionDepth,
            .pLibraryInfo = &libraryCreateInfo,
            .pLibraryInterface = nullptr,
            .layout = rtPipelineLayout,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = 0
    };

    rtPipeline = device.createRayTracingPipelineKHR(nullptr, nullptr, pipelineCreateInfo,
                                                    nullptr).value;

    device.destroyShaderModule(raygenModule);
    device.destroyShaderModule(chitModule);
    device.destroyShaderModule(missModule);
    device.destroyShaderModule(intModule);
}

std::vector<char> Vulkan::readBinaryFile(const std::string &path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("[Error] Failed to open file at '" + path + "'!");

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

void Vulkan::createCommandBuffer() {
    commandBuffers.resize(swapChainImages.size());
    for (int swapChainImageIndex = 0; swapChainImageIndex < swapChainImages.size(); swapChainImageIndex++) {
        auto& commandBuffer = commandBuffers[swapChainImageIndex];
        auto& swapChainImage = swapChainImages[swapChainImageIndex];
        commandBuffer = device.allocateCommandBuffers(
            {
                    .commandPool = commandPool,
                    .level = vk::CommandBufferLevel::ePrimary,
                    .commandBufferCount = 1
            }).front();

        vk::CommandBufferBeginInfo beginInfo = {};
        commandBuffer.begin(&beginInfo);

        // RENDER TARGET IMAGE & SUMMED PIXEL COLOR IMAGE: UNDEFINED -> GENERAL
        vk::ImageMemoryBarrier imageBarriersToGeneral[2] = {
                getImagePipelineBarrier(
                        vk::AccessFlagBits::eNoneKHR, vk::AccessFlagBits::eShaderWrite,
                        vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral, renderTargetImage.image),
                getImagePipelineBarrier(
                        vk::AccessFlagBits::eNoneKHR, vk::AccessFlagBits::eShaderWrite,
                        vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral, summedPixelColorImage.image)
        };

        commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eRayTracingShaderKHR,
            vk::PipelineStageFlagBits::eRayTracingShaderKHR,
            vk::DependencyFlagBits::eByRegion, 0, nullptr,
            0, nullptr, 2, imageBarriersToGeneral);


        // RAY TRACING
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eRayTracingKHR, rtPipeline);

        std::vector<vk::DescriptorSet> descriptorSets = { rtDescriptorSet };
        commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eRayTracingKHR, rtPipelineLayout,
            0, descriptorSets, nullptr);

        commandBuffer.traceRaysKHR(sbtRayGenAddressRegion, sbtMissAddressRegion, sbtHitAddressRegion, {},
            settings.windowWidth, settings.windowHeight, 1);


        // RENDER TARGET IMAGE: GENERAL -> TRANSFER SRC & SWAP CHAIN IMAGE: UNDEFINED -> TRANSFER DST
        vk::ImageMemoryBarrier imageBarriersToTransfer[2] = {
                getImagePipelineBarrier(
                        vk::AccessFlagBits::eShaderWrite, vk::AccessFlagBits::eTransferRead,
                        vk::ImageLayout::eGeneral, vk::ImageLayout::eTransferSrcOptimal, renderTargetImage.image),
                getImagePipelineBarrier(
                        vk::AccessFlagBits::eNoneKHR, vk::AccessFlagBits::eTransferWrite,
                        vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, swapChainImage)
        };

        commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eRayTracingShaderKHR, vk::PipelineStageFlagBits::eTransfer,
            vk::DependencyFlagBits::eByRegion, 0, nullptr,
            0, nullptr, 2, imageBarriersToTransfer);


        // COPY RENDER TARGET IMAGE TO SWAP CHAIN IMAGE
        vk::ImageSubresourceLayers subresourceLayers = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .mipLevel = 0,
                .baseArrayLayer = 0,
                .layerCount = 1
        };

        vk::ImageCopy imageCopy = {
                .srcSubresource = subresourceLayers,
                .srcOffset = {0, 0, 0},
                .dstSubresource = subresourceLayers,
                .dstOffset = {0, 0, 0},
                .extent = {
                        .width = swapChainExtent.width,
                        .height = swapChainExtent.height,
                        .depth = 1
                }
        };

        commandBuffer.copyImage(renderTargetImage.image, vk::ImageLayout::eTransferSrcOptimal, swapChainImage,
            vk::ImageLayout::eTransferDstOptimal, 1, &imageCopy);


        // SWAP CHAIN IMAGE: TRANSFER DST -> PRESENT
        vk::ImageMemoryBarrier barrierSwapChainToPresent = getImagePipelineBarrier(
            vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eMemoryRead,
            vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::ePresentSrcKHR, swapChainImage);

        commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer,
            vk::DependencyFlagBits::eByRegion, 0, nullptr,
            0, nullptr, 1, &barrierSwapChainToPresent);

        commandBuffer.end();
    }
}

void Vulkan::createFence() {
    fence = device.createFence({});
}

void Vulkan::createSemaphore() {
    semaphores.resize(swapChainImages.size());
    std::ranges::for_each(semaphores, [this](auto& semaphore) {
        semaphore = device.createSemaphore({});
        });
}

uint32_t Vulkan::findMemoryTypeIndex(const uint32_t &memoryTypeBits, const vk::MemoryPropertyFlags &properties) {
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((memoryTypeBits & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Unable to find suitable memory type!");
}

vk::ImageMemoryBarrier Vulkan::getImagePipelineBarrier(
        const vk::AccessFlagBits &srcAccessFlags, const vk::AccessFlagBits &dstAccessFlags,
        const vk::ImageLayout &oldLayout, const vk::ImageLayout &newLayout,
        const vk::Image &image) const {

    return {
            .srcAccessMask = srcAccessFlags,
            .dstAccessMask = dstAccessFlags,
            .oldLayout = oldLayout,
            .newLayout = newLayout,
            .srcQueueFamilyIndex = computeQueueFamily,
            .dstQueueFamilyIndex = computeQueueFamily,
            .image = image,
            .subresourceRange = {
                    .aspectMask = vk::ImageAspectFlagBits::eColor,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1
            },
    };
}

VulkanImage Vulkan::createImage(const vk::Format &format, const vk::Flags<vk::ImageUsageFlagBits> &usageFlagBits) {
    vk::ImageCreateInfo imageCreateInfo = {
            .imageType = vk::ImageType::e2D,
            .format = format,
            .extent = {.width = settings.windowWidth, .height = settings.windowHeight, .depth = 1},
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = vk::SampleCountFlagBits::e1,
            .tiling = vk::ImageTiling::eOptimal,
            .usage = usageFlagBits,
            .sharingMode = vk::SharingMode::eExclusive,
            .initialLayout = vk::ImageLayout::eUndefined
    };

    vk::Image image = device.createImage(imageCreateInfo);

    vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(image);

    vk::MemoryAllocateInfo allocateInfo = {
            .allocationSize = memoryRequirements.size,
            .memoryTypeIndex = findMemoryTypeIndex(memoryRequirements.memoryTypeBits,
                                                   vk::MemoryPropertyFlagBits::eDeviceLocal)
    };

    vk::DeviceMemory memory = device.allocateMemory(allocateInfo);

    device.bindImageMemory(image, memory, 0);

    return {
            .image = image,
            .memory = memory,
            .imageView = createImageView(image, format)
    };
}

void Vulkan::createImages() {
    renderTargetImage = createImage(swapChainImageFormat,
                                    vk::ImageUsageFlagBits::eStorage | vk::ImageUsageFlagBits::eTransferSrc);

    summedPixelColorImage = createImage(summedPixelColorImageFormat, vk::ImageUsageFlagBits::eStorage);
}

void Vulkan::destroyImage(const VulkanImage &image) const {
    device.destroyImageView(image.imageView);
    device.destroyImage(image.image);
    device.freeMemory(image.memory);
}

VulkanBuffer Vulkan::createBuffer(const vk::DeviceSize &size, const vk::Flags<vk::BufferUsageFlagBits> &usage,
                                  const vk::Flags<vk::MemoryPropertyFlagBits> &memoryProperty) {
    vk::BufferCreateInfo bufferCreateInfo = {
            .size = size,
            .usage = usage,
            .sharingMode = vk::SharingMode::eExclusive
    };

    vk::Buffer buffer = device.createBuffer(bufferCreateInfo);

    vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements(buffer);

    vk::MemoryAllocateFlagsInfo allocateFlagsInfo = {
            .flags = vk::MemoryAllocateFlagBits::eDeviceAddress
    };

    vk::MemoryAllocateInfo allocateInfo = {
            .pNext = &allocateFlagsInfo,
            .allocationSize = memoryRequirements.size,
            .memoryTypeIndex = findMemoryTypeIndex(memoryRequirements.memoryTypeBits, memoryProperty)
    };

    vk::DeviceMemory memory = device.allocateMemory(allocateInfo);

    device.bindBufferMemory(buffer, memory, 0);

    return {
            .buffer = buffer,
            .memory = memory,
    };
}

void Vulkan::destroyBuffer(const VulkanBuffer &buffer) const {
    device.destroyBuffer(buffer.buffer);
    device.freeMemory(buffer.memory);
}

void Vulkan::executeSingleTimeCommand(const std::function<void(const vk::CommandBuffer &singleTimeCommandBuffer)> &c) {
    vk::CommandBuffer singleTimeCommandBuffer = device.allocateCommandBuffers(
            {
                    .commandPool = commandPool,
                    .level = vk::CommandBufferLevel::ePrimary,
                    .commandBufferCount = 1
            }).front();

    vk::CommandBufferBeginInfo beginInfo = {
            .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
    };

    singleTimeCommandBuffer.begin(&beginInfo);

    c(singleTimeCommandBuffer);

    singleTimeCommandBuffer.end();


    vk::SubmitInfo submitInfo = {
            .commandBufferCount = 1,
            .pCommandBuffers = &singleTimeCommandBuffer
    };

    vk::Fence f = device.createFence({});
    computeQueue.submit(1, &submitInfo, f);
    device.waitForFences(1, &f, true, UINT64_MAX);

    device.destroyFence(f);
    device.freeCommandBuffers(commandPool, singleTimeCommandBuffer);
}

void Vulkan::createAABBBuffer() {
    const vk::DeviceSize bufferSize = sizeof(vk::AabbPositionsKHR) * aabbs.size();

    aabbBuffer = createBuffer(bufferSize,
                              vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR |
                              vk::BufferUsageFlagBits::eShaderDeviceAddress,
                              vk::MemoryPropertyFlagBits::eHostVisible |
                              vk::MemoryPropertyFlagBits::eHostCoherent |
                              vk::MemoryPropertyFlagBits::eDeviceLocal);

    void* data = device.mapMemory(aabbBuffer.memory, 0, bufferSize);
    memcpy(data, aabbs.data(), bufferSize);
    device.unmapMemory(aabbBuffer.memory);
}

void Vulkan::createBottomAccelerationStructure() {
    // ACCELERATION STRUCTURE META INFO
    vk::AccelerationStructureGeometryKHR geometry = {
            .geometryType = vk::GeometryTypeKHR::eAabbs,
            .flags = vk::GeometryFlagBitsKHR::eOpaque
    };

    geometry.geometry.aabbs.sType = vk::StructureType::eAccelerationStructureGeometryAabbsDataKHR;
    geometry.geometry.aabbs.stride = sizeof(vk::AabbPositionsKHR);
    geometry.geometry.aabbs.data.deviceAddress = device.getBufferAddress({.buffer = aabbBuffer.buffer});


    vk::AccelerationStructureBuildGeometryInfoKHR buildInfo = {
            .type = vk::AccelerationStructureTypeKHR::eBottomLevel,
            .flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace,
            .mode = vk::BuildAccelerationStructureModeKHR::eBuild,
            .srcAccelerationStructure = nullptr,
            .dstAccelerationStructure = nullptr,
            .geometryCount = 1,
            .pGeometries = &geometry,
            .scratchData = {}
    };


    // CALCULATE REQUIRED SIZE FOR THE ACCELERATION STRUCTURE
    std::vector<uint32_t> maxPrimitiveCounts = {static_cast<uint32_t>(aabbs.size())};

    vk::AccelerationStructureBuildSizesInfoKHR buildSizesInfo = device.getAccelerationStructureBuildSizesKHR(
            vk::AccelerationStructureBuildTypeKHR::eDevice, buildInfo, maxPrimitiveCounts);


    // ALLOCATE BUFFERS FOR ACCELERATION STRUCTURE
    bottomAccelerationStructure.structureBuffer = createBuffer(buildSizesInfo.accelerationStructureSize,
                                                               vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR |
        vk::BufferUsageFlagBits::eShaderDeviceAddress,
                                                               vk::MemoryPropertyFlagBits::eDeviceLocal);

    bottomAccelerationStructure.scratchBuffer = createBuffer(buildSizesInfo.buildScratchSize,
                                                             vk::BufferUsageFlagBits::eStorageBuffer |
                                                             vk::BufferUsageFlagBits::eShaderDeviceAddress,
                                                             vk::MemoryPropertyFlagBits::eDeviceLocal);

    // CREATE THE ACCELERATION STRUCTURE
    vk::AccelerationStructureCreateInfoKHR createInfo = {
            .buffer = bottomAccelerationStructure.structureBuffer.buffer,
            .offset = 0,
            .size = buildSizesInfo.accelerationStructureSize,
            .type = vk::AccelerationStructureTypeKHR::eBottomLevel
    };

    bottomAccelerationStructure.accelerationStructure =
            device.createAccelerationStructureKHR(createInfo, nullptr);


    // FILL IN THE REMAINING META INFO
    buildInfo.dstAccelerationStructure = bottomAccelerationStructure.accelerationStructure;
    buildInfo.scratchData.deviceAddress =
            device.getBufferAddress({.buffer = bottomAccelerationStructure.scratchBuffer.buffer});


    // BUILD THE ACCELERATION STRUCTURE
    vk::AccelerationStructureBuildRangeInfoKHR buildRangeInfo = {
            .primitiveCount = static_cast<uint32_t>(aabbs.size()),
            .primitiveOffset = 0,
            .firstVertex = 0,
            .transformOffset = 0
    };

    const vk::AccelerationStructureBuildRangeInfoKHR* pBuildRangeInfos[] = {&buildRangeInfo};

    executeSingleTimeCommand([&](const vk::CommandBuffer &singleTimeCommandBuffer) {
        singleTimeCommandBuffer.buildAccelerationStructuresKHR(1, &buildInfo, pBuildRangeInfos);
    });
}

void Vulkan::createTopAccelerationStructure() {
    // ACCELERATION STRUCTURE META INFO
    vk::AccelerationStructureGeometryKHR geometry = {
            .geometryType = vk::GeometryTypeKHR::eInstances,
            .flags = vk::GeometryFlagBitsKHR::eOpaque
    };

    geometry.geometry.instances.sType = vk::StructureType::eAccelerationStructureGeometryInstancesDataKHR;
    geometry.geometry.instances.arrayOfPointers = false;


    vk::AccelerationStructureBuildGeometryInfoKHR buildInfo = {
            .type = vk::AccelerationStructureTypeKHR::eTopLevel,
            .flags = vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace,
            .mode = vk::BuildAccelerationStructureModeKHR::eBuild,
            .srcAccelerationStructure = nullptr,
            .dstAccelerationStructure = nullptr,
            .geometryCount = 1,
            .pGeometries = &geometry,
            .scratchData = {}
    };


    // CALCULATE REQUIRED SIZE FOR THE ACCELERATION STRUCTURE
    vk::AccelerationStructureBuildSizesInfoKHR buildSizesInfo = device.getAccelerationStructureBuildSizesKHR(
            vk::AccelerationStructureBuildTypeKHR::eDevice, buildInfo, {1});


    // ALLOCATE BUFFERS FOR ACCELERATION STRUCTURE
    topAccelerationStructure.structureBuffer = createBuffer(buildSizesInfo.accelerationStructureSize,
                                                            vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR,
                                                            vk::MemoryPropertyFlagBits::eDeviceLocal);

    topAccelerationStructure.scratchBuffer = createBuffer(buildSizesInfo.buildScratchSize,
                                                          vk::BufferUsageFlagBits::eStorageBuffer |
                                                          vk::BufferUsageFlagBits::eShaderDeviceAddress,
                                                          vk::MemoryPropertyFlagBits::eDeviceLocal);

    // CREATE THE ACCELERATION STRUCTURE
    vk::AccelerationStructureCreateInfoKHR createInfo = {
            .buffer = topAccelerationStructure.structureBuffer.buffer,
            .offset = 0,
            .size = buildSizesInfo.accelerationStructureSize,
            .type = vk::AccelerationStructureTypeKHR::eTopLevel
    };

    topAccelerationStructure.accelerationStructure =
            device.createAccelerationStructureKHR(createInfo, nullptr);


    // CREATE INSTANCE INFO & WRITE IN NEW BUFFER
    std::array<std::array<float, 4>, 3> matrix = {
            {
                    {1.0f, 0.0f, 0.0f, 0.0f},
                    {0.0f, 1.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 1.0f, 0.0f}
            }};

    vk::AccelerationStructureInstanceKHR accelerationStructureInstance = {
            .transform = {.matrix = matrix},
            .instanceCustomIndex = 0,
            .mask = 0xFF,
            .instanceShaderBindingTableRecordOffset = 0,
            .accelerationStructureReference = device.getAccelerationStructureAddressKHR(
                    {.accelerationStructure = bottomAccelerationStructure.accelerationStructure}),
    };

    topAccelerationStructure.instancesBuffer = createBuffer(
            sizeof(vk::AccelerationStructureInstanceKHR),
            vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR |
            vk::BufferUsageFlagBits::eShaderDeviceAddress,
            vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostCoherent |
            vk::MemoryPropertyFlagBits::eHostVisible);

    void* pInstancesBuffer = device.mapMemory(topAccelerationStructure.instancesBuffer.memory, 0,
                                              sizeof(vk::AccelerationStructureInstanceKHR));
    memcpy(pInstancesBuffer, &accelerationStructureInstance, sizeof(vk::AccelerationStructureInstanceKHR));
    device.unmapMemory(topAccelerationStructure.instancesBuffer.memory);


    // FILL IN THE REMAINING META INFO
    buildInfo.dstAccelerationStructure = topAccelerationStructure.accelerationStructure;
    buildInfo.scratchData.deviceAddress = device.getBufferAddress(
            {.buffer = topAccelerationStructure.scratchBuffer.buffer});

    geometry.geometry.instances.data.deviceAddress = device.getBufferAddress(
            {.buffer = topAccelerationStructure.instancesBuffer.buffer});


    // BUILD THE ACCELERATION STRUCTURE
    vk::AccelerationStructureBuildRangeInfoKHR buildRangeInfo = {
            .primitiveCount = 1,
            .primitiveOffset = 0,
            .firstVertex = 0,
            .transformOffset = 0
    };

    const vk::AccelerationStructureBuildRangeInfoKHR* pBuildRangeInfos[] = {&buildRangeInfo};

    executeSingleTimeCommand([&](const vk::CommandBuffer &singleTimeCommandBuffer) {
        singleTimeCommandBuffer.buildAccelerationStructuresKHR(1, &buildInfo, pBuildRangeInfos);
    });
}

void Vulkan::destroyAccelerationStructure(const VulkanAccelerationStructure &accelerationStructure) {
    device.destroyAccelerationStructureKHR(accelerationStructure.accelerationStructure, nullptr);
    destroyBuffer(accelerationStructure.structureBuffer);
    destroyBuffer(accelerationStructure.scratchBuffer);
    destroyBuffer(accelerationStructure.instancesBuffer);
}

vk::ShaderModule Vulkan::createShaderModule(const std::string &path) const {
    std::vector<char> shaderCode = readBinaryFile(path);

    vk::ShaderModuleCreateInfo shaderModuleCreateInfo = {
            .codeSize = shaderCode.size(),
            .pCode = reinterpret_cast<const uint32_t*>(shaderCode.data())
    };

    return device.createShaderModule(shaderModuleCreateInfo);
}

void Vulkan::createShaderBindingTable() {
    vk::PhysicalDeviceRayTracingPipelinePropertiesKHR rayTracingProperties = getRayTracingProperties();
    uint32_t baseAlignment = rayTracingProperties.shaderGroupBaseAlignment;
    uint32_t handleSize = rayTracingProperties.shaderGroupHandleSize;


    const uint32_t shaderGroupCount = 3;
    vk::DeviceSize sbtBufferSize = baseAlignment * shaderGroupCount;

    shaderBindingTableBuffer = createBuffer(sbtBufferSize,
                                            vk::BufferUsageFlagBits::eShaderBindingTableKHR |
                                            vk::BufferUsageFlagBits::eShaderDeviceAddress,
                                            vk::MemoryPropertyFlagBits::eHostVisible |
                                            vk::MemoryPropertyFlagBits::eHostCoherent |
                                            vk::MemoryPropertyFlagBits::eDeviceLocal);


    std::vector<uint8_t> handles = device.getRayTracingShaderGroupHandlesKHR<uint8_t>(
            rtPipeline, 0, shaderGroupCount, shaderGroupCount * handleSize);

    vk::DeviceAddress sbtAddress = device.getBufferAddress({.buffer = shaderBindingTableBuffer.buffer});

    vk::StridedDeviceAddressRegionKHR addressRegion = {
            .stride = baseAlignment,
            .size = handleSize
    };

    sbtRayGenAddressRegion = addressRegion;
    sbtRayGenAddressRegion.size = baseAlignment;
    sbtRayGenAddressRegion.deviceAddress = sbtAddress;

    sbtMissAddressRegion = addressRegion;
    sbtMissAddressRegion.deviceAddress = sbtAddress + baseAlignment;

    sbtHitAddressRegion = addressRegion;
    sbtHitAddressRegion.deviceAddress = sbtAddress + baseAlignment * 2;

    uint8_t* sbtBufferData = static_cast<uint8_t*>(device.mapMemory(shaderBindingTableBuffer.memory, 0, sbtBufferSize));

    memcpy(sbtBufferData, handles.data(), handleSize);
    memcpy(sbtBufferData + baseAlignment, handles.data() + handleSize, handleSize);
    memcpy(sbtBufferData + baseAlignment * 2, handles.data() + handleSize * 2, handleSize);

    device.unmapMemory(shaderBindingTableBuffer.memory);
}

vk::PhysicalDeviceRayTracingPipelinePropertiesKHR Vulkan::getRayTracingProperties() const {
    vk::PhysicalDeviceRayTracingPipelinePropertiesKHR rayTracingPipelinePropertiesKhr = {};

    vk::PhysicalDeviceProperties2 physicalDeviceProperties2 = {
            .pNext = &rayTracingPipelinePropertiesKhr
    };

    physicalDevice.getProperties2(&physicalDeviceProperties2);

    return rayTracingPipelinePropertiesKhr;
}

void Vulkan::createSphereBuffer() {
    const vk::DeviceSize bufferSize = sizeof(Sphere) * MAX_SPHERE_AMOUNT;

    sphereBuffer = createBuffer(bufferSize,
                                vk::BufferUsageFlagBits::eUniformBuffer,
                                vk::MemoryPropertyFlagBits::eHostVisible |
                                vk::MemoryPropertyFlagBits::eHostCoherent |
                                vk::MemoryPropertyFlagBits::eDeviceLocal);

    void* data = device.mapMemory(sphereBuffer.memory, 0, bufferSize);
    memcpy(data, scene.spheres, sizeof(Sphere) * scene.sphereAmount);
    device.unmapMemory(sphereBuffer.memory);
}

vk::AabbPositionsKHR Vulkan::getAABBFromSphere(const glm::vec4 &geometry) {
    return {
            .minX = geometry.x - geometry.w,
            .minY = geometry.y - geometry.w,
            .minZ = geometry.z - geometry.w,
            .maxX = geometry.x + geometry.w,
            .maxY = geometry.y + geometry.w,
            .maxZ = geometry.z + geometry.w
    };
}

void Vulkan::createRenderCallInfoBuffer() {
    renderCallInfoBuffer = createBuffer(sizeof(RenderCallInfo), vk::BufferUsageFlagBits::eUniformBuffer,
                                        vk::MemoryPropertyFlagBits::eHostVisible |
                                        vk::MemoryPropertyFlagBits::eHostCoherent |
                                        vk::MemoryPropertyFlagBits::eDeviceLocal);
}

void Vulkan::updateRenderCallInfoBuffer(const RenderCallInfo &renderCallInfo) {
    void* data = device.mapMemory(renderCallInfoBuffer.memory, 0, sizeof(RenderCallInfo));
    memcpy(data, &renderCallInfo, sizeof(RenderCallInfo));
    device.unmapMemory(renderCallInfoBuffer.memory);
}
