#include <iostream>
#include <map>
#include <string.h>
#include <string>
#include <vulkan/vulkan.h>

#include "argvparse.h"
#include "configparse.h"
#include "nvidia.h"
#include "opengl.h"
#include "vulkan.h"

void print_help()
{
    std::cout << "usage: prime [options]" << std::endl;
    std::cout << "options:" << std::endl;
    std::cout << "  -h, --help        print this help message." << std::endl;
    std::cout << "  -e, --print-env   print env vars that will be used." << std::endl;
    std::cout << "  -p, --power       print state and power management info about the cards."
              << std::endl;
    std::cout << "  -d, --devices     check devices that will be used in opengl/vulkan."
              << std::endl;
    std::cout << "  -c, --create      create local config template that will be used when running prime."
              << std::endl
              << std::endl;
    std::cout << "usage: prime <command>" << std::endl;
    std::cout << "example:" << std::endl;
    std::cout
        << "  prime glxgears    will run glxgears with set environment variables to run on dgpu."
        << std::endl;
}

void check_devices()
{
    auto v = prime::vulkan();
    if (v.create_instance()) {
        auto const& vk_devices = v.get_devices();

        if (!vk_devices.empty()) {
            auto first_dev = vk_devices.front();
            auto prop = v.get_property(first_dev);
            auto name = std::string(prop.deviceName);
            std::cout << "vulkan first device: " << name << std::endl;
        } else {
            std::cerr << "error: cant find any vulkan devices." << std::endl;
        }
    } else {
        std::cerr << "error: cant create a vulkan instance." << std::endl;
    }

    auto gl = prime::opengl();
    auto const& gl_devices = gl.get_opengl_devices();

    if (!gl_devices.empty()) {
        auto first_dev = gl_devices.front();
        std::cout << "opengl first device: " << first_dev.get_renderer() << std::endl;
    } else {
        std::cerr << "error: cant find any opengl devices" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::cerr << "no argument specified (use -h for help)" << std::endl;
        exit(1);
    }

    auto config = prime::configparse(prime::get_xdg_config() + "/prime", "prime.cfg");
    auto cfg_map = config.get_config_map();

    for (auto const& [key, val] : cfg_map) {
        prime::set_env_var(key, val, 1);
    }

    if (argc >= 2 && !prime::str_starts_with(std::string(argv[1]), "-")) {
        prime::run_cmd(argc, argv);
    }

    auto argv_parse = prime::argvparse(argc, argv);

    if (argv_parse.find_argument("--help") || argv_parse.find_argument("-h")) {
        print_help();
    } else if (argv_parse.find_argument("--print-env") || argv_parse.find_argument("-e")) {
        if(cfg_map.empty()) {
            std::cerr << "error: no environment variables will be used." << std::endl;
        }
        for (auto const& [key, val] : cfg_map) {
            prime::print_env_var(key);
        }
    } else if (argv_parse.find_argument("--power") || argv_parse.find_argument("-p")) {
        auto nv = prime::nvidia();
        nv.print_proc_power();
        nv.print_sys_control();
    } else if (argv_parse.find_argument("--devices") || argv_parse.find_argument("-d")) {
        check_devices();
    }
    else if (argv_parse.find_argument("--create") || argv_parse.find_argument("-c")) {
        config.create_template_config();
    }

    if (argv_parse.argv().size() >= 1 && !argv_parse.has_matched_argument()) {
        std::cerr << "error: invalid argument specified (use -h for help)" << std::endl;
        exit(1);
    }

    return 0;
}
