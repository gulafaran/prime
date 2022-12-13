#ifndef ARGVPARSE_H
#define ARGVPARSE_H

#include "utils.h"
#include <string>
#include <vector>

namespace prime
{
class arg
{
public:
    arg(std::string const& key, std::string const& value)
        : m_key(key)
        , m_value(value)
    {
    }
    std::string key() const
    {
        return m_key;
    }

    std::string value() const
    {
        return m_value;
    }

private:
    std::string m_key{};
    std::string m_value{};
};

class argvparse
{
public:
    argvparse(int argc, char** argv)
    {
        for (int i = 1; i < argc; i++) {
            std::string const a = argv[i];
            auto is_valid_short_prefix = [&a]() {
                if (!str_starts_with(a, "-") || a.size() <= 1) {
                    return false;
                }
                if (contains_alpha(a.substr(1, 1))) {
                    return true;
                }
                return false;
            };
            auto is_valid_long_prefix = [&a]() {
                if (!str_starts_with(a, "--") || a.size() <= 2) {
                    return false;
                }
                if (contains_alpha(a.substr(2, 1))) {
                    return true;
                }
                return false;
            };

            if (a.empty()) {
                continue;
            }
            if (!is_valid_short_prefix()) {
                if (!is_valid_long_prefix()) {
                    continue;
                }
            }

            std::string key = "";
            auto key_pos = a.find('=');
            if (key_pos != std::string::npos) {
                key = a.substr(0, key_pos);
            } else {
                key = argv[i];
            }

            auto val_pos = a.find('=');
            std::string value = "";
            if (val_pos != std::string::npos) {
                value = a.substr(val_pos + 1);
            }

            if (!std::any_of(
                    m_argv.begin(), m_argv.end(), [&key](auto a) { return key == a.key(); })) {
                m_argv.push_back({key, value});
            }
        }
    }

    bool has_matched_argument()
    {
        return m_has_matched;
    }

    bool find_argument(std::string const& key)
    {
        return std::any_of(m_argv.begin(), m_argv.end(), [&key, this](auto a) {
            if (a.key() == key) {
                m_has_matched = true;
                return true;
            }
            return false;
        });
    }

    std::vector<arg> argv()
    {
        return m_argv;
    }

private:
    std::vector<arg> m_argv;
    bool m_has_matched{false};
};
}
#endif // ARGVPARSE_H
