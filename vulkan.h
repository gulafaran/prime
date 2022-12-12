#ifndef VULKAN_H
#define VULKAN_H

#include <vector>
#include <vulkan/vulkan.h>

class vulkan
{
public:
    vulkan()
        : m_instance{nullptr}
        , m_app_info{VK_STRUCTURE_TYPE_APPLICATION_INFO}
        , m_create_info{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO}
    {
        m_app_info.pApplicationName = "prime";
        m_app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        m_app_info.pEngineName = "prime";
        m_app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        m_app_info.apiVersion = VK_API_VERSION_1_0;

        m_create_info.pApplicationInfo = &m_app_info;
        m_create_info.enabledExtensionCount = 0;
        m_create_info.ppEnabledExtensionNames = nullptr;
    }

    ~vulkan()
    {
        if (m_instance) {
            vkDestroyInstance(m_instance, nullptr);
        }
    }

    bool create_instance()
    {
        return vkCreateInstance(&m_create_info, nullptr, &m_instance) == VK_SUCCESS;
    }

    std::vector<VkPhysicalDevice> get_devices()
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

    VkPhysicalDeviceProperties get_property(VkPhysicalDevice dev)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(dev, &properties);
        return properties;
    }

private:
    VkInstance m_instance;
    VkApplicationInfo m_app_info;
    VkInstanceCreateInfo m_create_info;
};

#endif // VULKAN_H
