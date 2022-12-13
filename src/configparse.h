#ifndef CONFIGPARSE_H
#define CONFIGPARSE_H

#include "utils.h"
#include <string>

namespace prime
{
class configparse
{
public:
    configparse(std::string path, std::string config_name)
        : m_path{path}
        , m_config_name{config_name}
    {
        m_config_file = path + "/" + config_name;
    }
    ~configparse() = default;

    std::map<std::string, std::string> get_config_map()
    {
        std::ifstream f(m_config_file);

        if (!f) {
            return {};
        }

        std::string line;
        std::map<std::string, std::string> c_map;

        while (std::getline(f, line)) {
            if (!str_starts_with(line, "#")) {
                std::string key = "";
                auto key_pos = line.find('=');

                if (key_pos == std::string::npos) {
                    continue;
                    key = line.substr(0, key_pos);
                }
                key = line.substr(0, key_pos);

                auto val_pos = line.find('=');
                std::string value = "";
                if (val_pos == std::string::npos) {
                    continue;
                }
                value = line.substr(val_pos + 1);

                c_map.insert(std::pair<std::string, std::string>(key, value));
            }
        }

        return c_map;
    }

    void create_template_config()
    {
        if (file_exist(m_config_file)) {
            std::cerr << "error: " << m_config_file << " already exist." << std::endl;
            return;
        }

        std::filesystem::create_directories(m_path);
        std::ofstream file;
        std::cout << "creating template config in: " << m_config_file << std::endl;
        file.open(m_config_file);

        file << "# this is an example, you can add or remove env vars as you like using the "
                "environment_var=value syntax."
             << std::endl;
        file << "# that will be assigned before prime runs anything." << std::endl;
        file << "" << std::endl;
        file << "_EGL_VENDOR_LIBRARY_FILENAMES=/usr/share/glvnd/egl_vendor.d/10_nvidia.json"
             << std::endl;
        file << "DXVK_FILTER_DEVICE_NAME=NVIDIA" << std::endl;
        file << "VKD3D_FILTER_DEVICE_NAME=NVIDIA" << std::endl;
        file << "VK_ICD_FILENAMES=/usr/share/vulkan/icd.d/nvidia_icd.json" << std::endl;
        file << "__NV_PRIME_RENDER_OFFLOAD=1" << std::endl;
        file << "__NV_PRIME_RENDER_OFFLOAD_PROVIDER=NVIDIA-G0" << std::endl;
        file << "__VK_LAYER_NV_optimus = NVIDIA_only" << std::endl;
        file << "__GLX_VENDOR_LIBRARY_NAME=nvidia" << std::endl;
        file << "DRI_PRIME=1" << std::endl;
        file.close();
    }

private:
    std::string m_path;
    std::string m_config_name;
    std::string m_config_file;
};
}
#endif // CONFIGPARSE_H
