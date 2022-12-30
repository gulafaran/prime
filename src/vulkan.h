#ifndef VULKAN_H
#define VULKAN_H

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace prime
{
class vulkan
{
public:
    vulkan();
    ~vulkan();

    bool create_instance();
    std::vector<VkPhysicalDevice> get_devices();
    VkPhysicalDeviceProperties get_property(VkPhysicalDevice const& dev);
    std::string get_version(uint32_t api_version);

private:
    VkInstance m_instance;
    VkApplicationInfo m_app_info;
    VkInstanceCreateInfo m_create_info;
};
}

#endif // VULKAN_H
