/*
 *  Copied an modified from
 *  https://github.com/ocornut/imgui/tree/master/examples/example_emscripten_wgpu
 *  under MIT License
 */

#include "GLFW/glfw3.h"
#include "emscripten.h"
#include "emscripten/html5.h"
#include "emscripten/html5_webgpu.h"
#include "webgpu/webgpu.h"
#include "webgpu/webgpu_cpp.h"

#include <iostream>
using namespace std;

// Global WebGPU required states
static WGPUDevice    wgpu_device = nullptr;
static WGPUSurface   wgpu_surface = nullptr;
static WGPUSwapChain wgpu_swap_chain = NULL;
static int           wgpu_swap_chain_width = 0;
static int           wgpu_swap_chain_height = 0;

// Forward declaration
static bool init_wgpu();
static void main_loop(void* window);
static void print_glfw_error(int error, const char* description);
static void print_wgpu_error(WGPUErrorType error_type, const char* message, void*);

int main(int argc, char **argv) {
    glfwSetErrorCallback(print_glfw_error);
    if (!glfwInit())
        return 1;

    // Make sure GLFW does not initialize any graphics context.
    // This needs to be done explicitly later
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+WebGPU example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    // Initialize the WebGPU environment
    if (!init_wgpu())
    {
        if (window)
            glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    glfwShowWindow(window);

    // This function will directly return and exit the main function.
    // Make sure that no required objects get cleaned up.
    // This way we can use the browsers 'requestAnimationFrame' to control the rendering.
    emscripten_set_main_loop_arg(main_loop, window, 0, false);

    cout << "Hello World!3" << endl;
}

static bool init_wgpu()
{
    wgpu_device = emscripten_webgpu_get_device();
    if (!wgpu_device)
        return false;

    wgpuDeviceSetUncapturedErrorCallback(wgpu_device, print_wgpu_error, NULL);

    // Use C++ wrapper due to misbehavior in Emscripten.
    // Some offset computation for wgpuInstanceCreateSurface in JavaScript
    // seem to be inline with struct alignments in the C++ structure
    wgpu::SurfaceDescriptorFromCanvasHTMLSelector html_surface_desc = {};
    html_surface_desc.selector = "#canvas";

    wgpu::SurfaceDescriptor surface_desc = {};
    surface_desc.nextInChain = &html_surface_desc;

    // Use 'null' instance
    wgpu::Instance instance = {};
    wgpu_surface = instance.CreateSurface(&surface_desc).Release();

    return true;
}

static void main_loop(void* window)
{
    glfwPollEvents();

    int width, height;
    glfwGetFramebufferSize((GLFWwindow*) window, &width, &height);

    // React to changes in screen size
    if (width != wgpu_swap_chain_width && height != wgpu_swap_chain_height)
    {
        //ImGui_ImplWGPU_InvalidateDeviceObjects();

        if (wgpu_swap_chain)
            wgpuSwapChainRelease(wgpu_swap_chain);

        wgpu_swap_chain_width = width;
        wgpu_swap_chain_height = height;

        WGPUSwapChainDescriptor swap_chain_desc = {};
        swap_chain_desc.usage = WGPUTextureUsage_RenderAttachment;
        swap_chain_desc.format = WGPUTextureFormat_RGBA8Unorm;
        swap_chain_desc.width = width;
        swap_chain_desc.height = height;
        swap_chain_desc.presentMode = WGPUPresentMode_Fifo;
        wgpu_swap_chain = wgpuDeviceCreateSwapChain(wgpu_device, wgpu_surface, &swap_chain_desc);

        //ImGui_ImplWGPU_CreateDeviceObjects();
    }

    WGPURenderPassColorAttachment color_attachments = {};
    color_attachments.loadOp = WGPULoadOp_Clear;
    color_attachments.storeOp = WGPUStoreOp_Store;
    //color_attachments.clearValue = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    color_attachments.clearValue = { 1.f, 0.f, 0.f, 1.f };
    color_attachments.view = wgpuSwapChainGetCurrentTextureView(wgpu_swap_chain);
    color_attachments.view = wgpuSwapChainGetCurrentTextureView(wgpu_swap_chain);
    WGPURenderPassDescriptor render_pass_desc = {};
    render_pass_desc.colorAttachmentCount = 1;
    render_pass_desc.colorAttachments = &color_attachments;
    render_pass_desc.depthStencilAttachment = NULL;

    WGPUCommandEncoderDescriptor enc_desc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(wgpu_device, &enc_desc);

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &render_pass_desc);
    //ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);
    wgpuRenderPassEncoderEnd(pass);

    WGPUCommandBufferDescriptor cmd_buffer_desc = {};
    WGPUCommandBuffer cmd_buffer = wgpuCommandEncoderFinish(encoder, &cmd_buffer_desc);
    WGPUQueue queue = wgpuDeviceGetQueue(wgpu_device);
    wgpuQueueSubmit(queue, 1, &cmd_buffer);
}

static void print_glfw_error(int error, const char* description) {
    printf("Glfw Error %d: %s\n", error, description);
}

static void print_wgpu_error(WGPUErrorType error_type, const char* message, void*)
{
    const char* error_type_lbl = "";
    switch (error_type)
    {
        case WGPUErrorType_Validation:  error_type_lbl = "Validation"; break;
        case WGPUErrorType_OutOfMemory: error_type_lbl = "Out of memory"; break;
        case WGPUErrorType_Unknown:     error_type_lbl = "Unknown"; break;
        case WGPUErrorType_DeviceLost:  error_type_lbl = "Device lost"; break;
        default:                        error_type_lbl = "Unknown";
    }
    printf("%s error: %s\n", error_type_lbl, message);
}
