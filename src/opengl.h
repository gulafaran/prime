#ifndef OPENGL_H
#define OPENGL_H

#include <GL/glew.h>
#include <string>
#include <vector>

namespace prime
{
class opengl_device
{
public:
    explicit opengl_device(int count);
    ~opengl_device() = default;

    int get_count();
    std::string get_vendor();
    std::string get_renderer();
    std::string get_version();
    std::string get_shading_version();
    std::vector<GLint> get_uuid();

private:
    int m_count;
};

class opengl
{
public:
    opengl() = default;
    ~opengl();

    bool create_instance();
    std::vector<opengl_device> get_devices();

private:
    bool m_instance_created{false};
};
}

#endif // OPENGL_H
