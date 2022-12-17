#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <pwd.h>
#include <regex>
#include <string>
#include <string_view>
#include <unistd.h>

namespace prime
{
extern bool str_starts_with(std::string const& string, std::string const& arg)
{
    auto arg_view = std::string_view(string);
    return arg_view.starts_with(arg);
}

extern bool str_contains(std::string const& string1, std::string const& string2)
{
    if (string1.find(string2) != std::string::npos) {
        return true;
    }
    return false;
}

extern bool contains_alpha(std::string const& string)
{
    return std::regex_match(string, std::regex("^[A-Za-z]+$"));
}

extern bool file_exist(std::string const& path)
{
    if (access(path.c_str(), F_OK) != -1) {
        return true;
    }

    return false;
}

extern void print_file(std::string const& file)
{
    std::ifstream output(file);

    if (!output) {
        std::cerr << "error: cant open file: " << file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(output, line)) {
        std::cout << line << std::endl;
    }

    output.close();
}

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

template<typename V>
std::string get_first_dir_entry(V const& path)
{
    std::filesystem::path fs(path);
    if (fs.empty()) {
        std::cerr << "error: cant open path: " << path << std::endl;
        return "";
    }

    auto entry = std::filesystem::directory_iterator{fs};
    if (!entry->exists()) {
        std::cerr << "error: path has no subdirs, path: " << path << std::endl;
        return "";
    }

    std::string trimmed = entry->path();
    trimmed.erase(0, path.size());
    return trimmed;
}

extern std::string get_xdg_home()
{
    auto home_env = get_env_var("XDG_HOME");
    if (home_env.empty()) {
        struct passwd* pw = getpwuid(getuid());
        return std::string(pw->pw_dir);
    }
    return home_env;
}

extern std::string get_xdg_config()
{
    auto config_env = get_env_var("XDG_DATA_CONFIG");
    if (config_env.empty()) {
        return get_xdg_home() + "/.config";
    }
    return config_env;
}

extern std::string get_xdg_data()
{
    auto data_env = get_env_var("XDG_DATA_CONFIG");
    if (data_env.empty()) {
        return get_xdg_home() + "/.local/share";
    }
    return data_env;
}

extern void run_cmd(int argc, char* argv[])
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

}

#endif // UTILS_H
