#include <iostream>
#include <string>
#include <unistd.h>

#include "configparse.h"
#include "xdg.h"

void run_cmd(int argc, char* argv[])
{
    char** app_argv = argv + 1;
    int ret = execvp(app_argv[0], app_argv);

    if (ret == -1) {
        std::cerr << "error: error running application: " << app_argv[0] << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::cerr << "no application specified use prime <application> to run the application "
                     "with the prime environment variables set."
                  << std::endl;
        exit(1);
    }

    auto config = prime::configparse(prime::get_xdg_config() + "/prime", "prime.cfg");

    if (!config.config_exist()) {
        std::cerr << "error: config does not exist, no environment variables will be used." << std::endl;
    } else {
        config.load_config();
        auto cfg_map = config.get_config_map();

        if (cfg_map.empty()) {
            std::cerr << "error: config either empty or having wrong syntax, no environment "
                         "variables is read."
                      << std::endl;
        } else {
            for (auto const& [key, val] : cfg_map) {
                prime::set_env_var(key, val, 1);
            }
        }
    }

    run_cmd(argc, argv);

    return 0;
}
