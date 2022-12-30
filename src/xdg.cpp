#include "xdg.h"
#include <pwd.h>
#include <unistd.h>

namespace prime
{
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

extern bool unset_env_var(std::string const& env)
{
    int ret = unsetenv(env.c_str());
    if (ret == -1) {
        return false;
    }
    return true;
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

}
