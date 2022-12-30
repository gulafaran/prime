#include "configparse.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <unistd.h>

namespace prime
{
configparse::configparse(std::string const& path, std::string const& config_name)
    : m_path{path}
    , m_config_name{config_name}
    , m_config_file{path + "/" + config_name}
{
}

bool configparse::config_exist()
{
    if (access(m_config_file.c_str(), F_OK) != -1) {
        return true;
    }

    return false;
}

void configparse::load_config()
{
    std::ifstream f(m_config_file);

    if (!f || !f.is_open()) {
        return;
    }

    std::string line;
    m_c_map.clear();

    auto str_starts_with = [](auto string1, auto string2) {
        if (string1.find(string2) != std::string::npos) {
            return true;
        }
        return false;
    };

    while (std::getline(f, line)) {
        if (!str_starts_with(line, "#")) {
            auto key_pos = line.find('=');
            if (key_pos == std::string::npos) {
                continue;
            }

            std::string key = line.substr(0, key_pos);
            auto val_pos = line.find('=');
            if (val_pos == std::string::npos) {
                continue;
            }

            std::string value = line.substr(val_pos + 1);
            m_c_map.insert(std::pair<std::string, std::string>(key, value));
        }
    }
}

std::map<std::string, std::string> configparse::get_config_map()
{
    return m_c_map;
}

void configparse::write_config_map(std::map<std::string, std::string> const& map)
{
    std::filesystem::path path = m_path;
    std::filesystem::create_directory(path);

    std::ofstream os(m_config_file, std::ios::out | std::ios::trunc);

    if (!os || !os.is_open()) {
        return;
    }

    os << "# this is an example, you can add or remove env vars as you like using the "
          "environment_var=value syntax."
       << std::endl;
    os << "# that will be assigned before prime runs anything. beware primeqt overwrites this file."
       << std::endl;

    for (auto const& [key, val] : map) {
        os << key + "=" + val << std::endl;
    }
    os.close();
}
}
