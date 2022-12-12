#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <unistd.h>

template<typename V, typename T>
bool str_starts_with(V const& string, T const& arg)
{
    auto arg_view = std::string_view(string);
    return arg_view.starts_with(arg);
}

template<typename V, typename T>
bool str_contains(V const& string1, T const& string2)
{
    if (string1.find(string2) != std::string::npos) {
        return true;
    }
    return false;
}

template<typename V>
bool contains_alpha(V const& string)
{
    return std::regex_match(string, std::regex("^[A-Za-z]+$"));
}

template<typename V>
bool file_exist(V const& path)
{
    if (access(path.c_str(), F_OK) != -1) {
        return true;
    }

    return false;
}

template<typename V>
void print_file(V const& file)
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

#endif // UTILS_H
