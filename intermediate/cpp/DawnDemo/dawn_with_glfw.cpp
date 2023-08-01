#include <iostream>
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>

#include <dawn/dawn_proc.h>
#include <dawn/webgpu_cpp.h>
#include <dawn/native/NullBackend.h>

#ifdef DAWN_ENABLE_BACKEND_VULKAN
//#include <dawn/native/VulkanBackend.h>

#endif

class DawnRenderSystem {
public:
    DawnRenderSystem() {
        WGPUBackendType type = WGPUBackendType_Vulkan;
        dawn::native::Adapter adapter = request_adapter(type);
        wgpu::AdapterProperties properties;
        adapter.GetProperties(&properties);
        WGPUDevice device = adapter.CreateDevice();

        if(device == nullptr) {
            std::cout << "nn";
        }
    }

private:
    /**
     * Analogous to the browser's \c GPU.request_adapter().
     * \n
     * The returned \c Adapter is a wrapper around the underlying Dawn adapter (and
     * owned by the single Dawn instance).
     *
     * \todo we might be interested in whether the \c AdapterType is discrete or integrated for power-management reasons
     *
     * \param[in] type1st first choice of \e backend type (e.g. \c WGPUBackendType_D3D12)
     * \param[in] type2nd optional fallback \e backend type (or \c WGPUBackendType_Null to pick the first choice or nothing)
     * \return the best choice adapter or an empty adapter wrapper
     */
    static dawn::native::Adapter request_adapter(WGPUBackendType type1st, WGPUBackendType type2nd = WGPUBackendType_Null) {
        dawn::native::Instance instance;
        instance.DiscoverDefaultAdapters();
        wgpu::AdapterProperties properties;
        std::vector<dawn::native::Adapter> adapters = instance.GetAdapters();
        for (auto it = adapters.begin(); it != adapters.end(); ++it) {
            it->GetProperties(&properties);
            if (static_cast<WGPUBackendType>(properties.backendType) == type1st) {
                return *it;
            }
        }
        if (type2nd) {
            for (auto it = adapters.begin(); it != adapters.end(); ++it) {
                it->GetProperties(&properties);
                if (static_cast<WGPUBackendType>(properties.backendType) == type2nd) {
                    return *it;
                }
            }
        }
        return dawn::native::Adapter();
    }
};

int main (int, char**) {
    std::cout << "Hello, world!" << std::endl;

    DawnRenderSystem render_system;

/*
    // 1. We create a descriptor
    WGPUInstanceDescriptor desc = {};
    desc.nextInChain = nullptr;

    // 2. We create the instance using this descriptor
    WGPUInstance instance = wgpuCreateInstance(&desc);

    // 3. We can check whether there is actually an instance created
    if (!instance) {
        std::cerr << "Could not initialize WebGPU!" << std::endl;
        return 1;
    }

    // 4. Display the object (WGPUInstance is a simple pointer, it may be
    // copied around without worrying about its size).
    std::cout << "WGPU instance: " << instance << std::endl;

    glfwInit();

    if (!glfwInit()) {
        std::cerr << "Could not initialize GLFW!" << std::endl;
        return 1;
    }

    // Create the window
    GLFWwindow* window = glfwCreateWindow(640, 480, "Learn WebGPU", NULL, NULL);

    if (!window) {
        std::cerr << "Could not open window!" << std::endl;
        glfwTerminate();
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        // Check whether the user clicked on the close button (and any other
        // mouse/key event, which we don't use so far)
        glfwPollEvents();
    }

    // At the end of the program, destroy the window
    glfwDestroyWindow(window);

    glfwTerminate();
*/
    return 0;
}
