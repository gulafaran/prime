#ifndef NVIDIA_H
#define NVIDIA_H

#include "utils.h"
#include <string>

namespace prime
{
class nvidia
{
public:
    nvidia()
        : nvidia_proc_path{"/proc/driver/nvidia/gpus/"}
        , nvidia_sys_path{"/sys/bus/pci/devices/"}
    {
        if (file_exist(nvidia_proc_path)) {
            nvidia_id = get_first_dir_entry(nvidia_proc_path);
        } else {
            nvidia_id = "";
        }
    }
    ~nvidia() = default;

    void print_proc_power()
    {
        if (!file_exist(nvidia_proc_path)) {
            return;
        }

        std::string file = nvidia_proc_path + nvidia_id + "/power";
        std::cout << "[NVIDIA Status]" << std::endl;
        print_file(file);
    }

    void print_sys_control()
    {
        if (!file_exist(nvidia_sys_path)) {
            return;
        }

        std::string file = nvidia_sys_path + nvidia_id + "/power/control";
        std::cout << "[NVIDIA Power Control State]" << std::endl;
        print_file(file);
    }

private:
    std::string nvidia_proc_path;
    std::string nvidia_sys_path;
    std::string nvidia_id;
};
}
#endif // NVIDIA_H
