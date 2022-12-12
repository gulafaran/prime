#ifndef ENVVAR_H
#define ENVVAR_H

#include <iostream>
#include <ostream>
#include <string>

namespace env
{
extern std::string get_env_var(std::string const& env)
{
    const char* e = getenv(env.c_str());
    if (e == NULL) {
        return "";
    }
    return e;
}

extern bool set_env_var(std::string const& env, std::string const& value, int overwrite)
{
    int ret = setenv(env.c_str(), value.c_str(), overwrite);
    if (ret == -1) {
        return false;
    }
    return true;
}

extern void print_env_var(std::string const& env)
{
    const char* e = getenv(env.c_str());
    if (e == NULL) {
        std::cout << env << " is not set." << std::endl;
        return;
    }

    std::cout << env << " = " << e << std::endl;
}

extern void set_env_vars()
{
    env::set_env_var(
        "_EGL_VENDOR_LIBRARY_FILENAMES", "/usr/share/glvnd/egl_vendor.d/10_nvidia.json", 1);
    env::set_env_var("DXVK_FILTER_DEVICE_NAME", "NVIDIA", 1);
    env::set_env_var("VKD3D_FILTER_DEVICE_NAME", "NVIDIA", 1);
    env::set_env_var("VK_ICD_FILENAMES", "/usr/share/vulkan/icd.d/nvidia_icd.json", 1);
    env::set_env_var("__NV_PRIME_RENDER_OFFLOAD", "1", 1);
    env::set_env_var("__NV_PRIME_RENDER_OFFLOAD_PROVIDER", "NVIDIA-G0", 1);
    env::set_env_var("__VK_LAYER_NV_optimus", "NVIDIA_only", 1);
    env::set_env_var("__GLX_VENDOR_LIBRARY_NAME", "nvidia", 1);
    env::set_env_var("DRI_PRIME", "1", 1);
}

extern void print_env_vars()
{
    env::print_env_var("_EGL_VENDOR_LIBRARY_FILENAMES");
    env::print_env_var("DXVK_FILTER_DEVICE_NAME");
    env::print_env_var("VKD3D_FILTER_DEVICE_NAME");
    env::print_env_var("VK_ICD_FILENAMES");
    env::print_env_var("__NV_PRIME_RENDER_OFFLOAD");
    env::print_env_var("__NV_PRIME_RENDER_OFFLOAD_PROVIDER");
    env::print_env_var("__VK_LAYER_NV_optimus");
    env::print_env_var("__GLX_VENDOR_LIBRARY_NAME");
    env::print_env_var("DRI_PRIME");
}

}

#endif // ENVVAR_H
