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
    {
        config_file = path + "/" + config_name;
        if (!file_exist(config_file)) {
            std::filesystem::create_directories(path);
            std::ofstream file;
            file.open(config_file);
            file.close();
        }
    }
    ~configparse() = default;

    std::map<std::string, std::string> get_config_map()
    {
        std::ifstream f(config_file);

        if (!f) {
            std::cerr << "error: cant open file: " << config_file << std::endl;
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

private:
    std::string config_file;
};
}
#endif // CONFIGPARSE_H
