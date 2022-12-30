#include "vulkan.h"
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace prime
{
vulkan::vulkan()
    : m_instance{nullptr}
    , m_app_info{VK_STRUCTURE_TYPE_APPLICATION_INFO}
    , m_create_info{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO}
{
    m_app_info.pApplicationName = "prime";
    m_app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    m_app_info.pEngineName = "prime";
    m_app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    m_app_info.apiVersion = VK_API_VERSION_1_0;
    char const* enabled_extensions[]
        = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME};

    m_create_info.pApplicationInfo = &m_app_info;
    m_create_info.enabledExtensionCount = 0;
    m_create_info.ppEnabledExtensionNames = nullptr;
}

vulkan::~vulkan()
{
    if (m_instance) {
        vkDestroyInstance(m_instance, nullptr);
    }
}

bool vulkan::create_instance()
{
    if (m_instance) {
        // dont create another instance if one already exist
        return true;
    }
    return vkCreateInstance(&m_create_info, nullptr, &m_instance) == VK_SUCCESS;
}

std::vector<VkPhysicalDevice> vulkan::get_devices()
{
    // Enumerate the physical devices (i.e. GPUs) available on the system
    // handle return result?
    uint32_t physicalDeviceCount = 0;
    if (vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, nullptr) != VK_SUCCESS) {
        return {};
    }

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    if (vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, physicalDevices.data())
        != VK_SUCCESS) {
        return {};
    }

    return physicalDevices;
}

VkPhysicalDeviceProperties vulkan::get_property(VkPhysicalDevice const& dev)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(dev, &properties);
    return properties;
}

std::string vulkan::get_version(uint32_t api_version)
{
    uint32_t major = VK_VERSION_MAJOR(api_version);
    uint32_t minor = VK_VERSION_MINOR(api_version);
    uint32_t patch = VK_VERSION_PATCH(api_version);
    std::string version
        = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);

    return version;
}
}
