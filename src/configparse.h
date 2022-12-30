#ifndef CONFIGPARSE_H
#define CONFIGPARSE_H

#include <map>
#include <string>

namespace prime
{
class configparse
{
public:
    configparse(std::string const& dir_name, std::string const& config_name);
    ~configparse() = default;

    bool config_exist();
    void load_config();
    std::map<std::string, std::string> get_config_map();
    void write_config_map(std::map<std::string, std::string> const& map);

private:
    std::string m_path;
    std::string m_config_name;
    std::string m_config_file;
    std::map<std::string, std::string> m_c_map;
};
}
#endif // CONFIGPARSE_H
