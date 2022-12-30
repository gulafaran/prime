#include "opengl.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace prime
{
opengl_device::opengl_device(int count)
{
    m_count = count;
}

int opengl_device::get_count()
{
    return m_count;
}

std::string opengl_device::get_vendor()
{
    auto vendor = glGetString(GL_VENDOR);

    return std::string(reinterpret_cast<const char*>(vendor));
}

std::string opengl_device::get_renderer()
{
    auto renderer = glGetString(GL_RENDERER);

    return std::string(reinterpret_cast<const char*>(renderer));
}

std::string opengl_device::get_version()
{
    auto version = glGetString(GL_VERSION);

    return std::string(reinterpret_cast<const char*>(version));
}

std::string opengl_device::get_shading_version()
{
    auto version = glGetString(GL_SHADING_LANGUAGE_VERSION);

    return std::string(reinterpret_cast<const char*>(version));
}

std::vector<GLint> opengl_device::get_uuid()
{
    GLint device_uuid[3];
    std::vector<GLint> ids;
    glGetIntegeri_v(GL_DEVICE_UUID_EXT, m_count, device_uuid);
    for (auto i = 0; i <= 3; i++) {
        ids.push_back(device_uuid[i]);
    }

    return ids;
}

opengl::~opengl()
{
    glfwTerminate();
}

bool opengl::create_instance()
{
    if (m_instance_created) {
        // already created dont try again.
        return true;
    }

    if (glfwInit() != GLFW_TRUE) {
        return false;
    }

    GLFWwindow* window;
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    window = glfwCreateWindow(1, 1, "", NULL, NULL);
    glfwMakeContextCurrent(window);
    m_instance_created = true;
    return true;
}

std::vector<opengl_device> opengl::get_devices()
{
    std::vector<opengl_device> devices;
    int device_count;
    glGetIntegerv(GL_NUM_DEVICE_UUIDS_EXT, &device_count);

    if (device_count == 0) {
        return {};
    }

    for (int i = 0; i < device_count; i++) {
        opengl_device dev(i);
        devices.push_back(dev);
    }

    return devices;
}
}
