#ifndef XDG_H
#define XDG_H

#include <string>

namespace prime
{
std::string get_env_var(std::string const& env);
bool set_env_var(std::string const& env, std::string const& value, int overwrite);
bool unset_env_var(std::string const& env);

std::string get_xdg_home();
std::string get_xdg_config();
std::string get_xdg_data();

}

#endif // XDG_H
