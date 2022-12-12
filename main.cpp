#include <iostream>
#include <map>
#include <string.h>
#include <string>
#include <vulkan/vulkan.h>

#include "argvparse.h"
#include "envvar.h"
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
    std::cout << "  -c, --check       check devices that will be used in opengl/vulkan."
              << std::endl
              << std::endl;
    std::cout << "usage: prime <command>" << std::endl;
    std::cout << "example:" << std::endl;
    std::cout
        << "  prime glxgears    will run glxgears with set environment variables to run on dgpu."
        << std::endl;
}

void run_cmd_on_dgpu(int argc, char* argv[])
{
    std::string cmd;
    for (int i = 1; i < argc; i++) {
        cmd += argv[i];
        cmd += " ";
    }

    if (!cmd.empty()) {
        auto status = system(cmd.c_str());
        if (status < 0) {
            std::cerr << "error: " << strerror(errno) << std::endl;
            exit(1);
        } else {
            if (WIFEXITED(status)) {
                exit(status);
            } else {
                std::cerr << "error: program exited abnormaly" << std::endl;
                exit(1);
            }
        }
    }
    exit(1);
}

void print_power()
{
    nvidia nv = nvidia();
    nv.print_proc_power();
    nv.print_sys_control();
}

void check_devices()
{
    auto v = vulkan();
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

    auto gl = opengl();
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

    env::set_env_vars();

    if (argc >= 2 && !str_starts_with(std::string(argv[1]), "-")) {
        run_cmd_on_dgpu(argc, argv);
    }

    argvparse argv_parse(argc, argv);

    if (argv_parse.find_argument("--help") || argv_parse.find_argument("-h")) {
        print_help();
    } else if (argv_parse.find_argument("--print-env") || argv_parse.find_argument("-e")) {
        env::print_env_vars();
    } else if (argv_parse.find_argument("--power") || argv_parse.find_argument("-p")) {
        print_power();
    } else if (argv_parse.find_argument("--check") || argv_parse.find_argument("-c")) {
        check_devices();
    }

    if (argv_parse.argv().size() >= 1 && !argv_parse.has_matched_argument()) {
        std::cerr << "error: invalid argument specified (use -h for help)" << std::endl;
        exit(1);
    }

    return 0;
}