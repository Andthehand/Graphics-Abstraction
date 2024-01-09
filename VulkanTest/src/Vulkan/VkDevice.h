#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <vector>
#include <stdexcept>
#include <iostream>
#include <optional>

#include "Vulkan/VkBuffer.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> GraphicsFamily;
    std::optional<uint32_t> PresentFamily;

    bool IsComplete() {
        return GraphicsFamily.has_value() && PresentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR Capabilities;
    std::vector<VkSurfaceFormatKHR> Formats;
    std::vector<VkPresentModeKHR> PresentModes;
};

class Device {
public:
    void Init(GLFWwindow* windowHandle);
    void Shutdown();

    void SwapBuffers();

    VkDevice GetDevice() { return m_Device; }
    VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
    VkSurfaceKHR GetSurface() { return m_Surface; }

    SwapChainSupportDetails& GetSwapChainSupport() { return m_SwapChainSupport; }
    QueueFamilyIndices& GetQueueFamilyIndices() { return m_QueueFamilyIndices; }

    VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
    VkQueue GetPresentQueue() { return m_PresentQueue; }

    uint32_t GetCurrentFrame() { return m_CurrentFrame; }

    static Device& Get() {
        static Device instance;
		return instance;
	}
public:
    static const int MAX_FRAMES_IN_FLIGHT = 2;
private:
    void CreateVkInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();

    std::vector<const char*> GetRequiredExtensions();

    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

    //Incase you need to check if the GPU is able to run the program
    bool IsDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
private:
    uint32_t m_CurrentFrame = 0;

    GLFWwindow* m_WindowHandle;
    VkSurfaceKHR m_Surface;

    SwapChainSupportDetails m_SwapChainSupport;
    QueueFamilyIndices m_QueueFamilyIndices;

    VkInstance m_Instance;
    VkDebugUtilsMessengerEXT m_DebugMessenger;

    VkPhysicalDevice m_PhysicalDevice;
    VkDevice m_Device;

    VkQueue m_GraphicsQueue, m_PresentQueue;

#ifdef DEBUG
    const bool m_EnableValidationLayers = true;
#else
    const bool m_EnableValidationLayers = false;
#endif

    const std::vector<const char*> m_ValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> m_DeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
};