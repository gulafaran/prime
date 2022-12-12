#ifndef OPENGL_H
#define OPENGL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

class opengl_device {
public:
    opengl_device(int count) {
        m_count = count;
    }

    int get_count() {
        return m_count;
    }

    std::string get_vendor() {
        auto vendor = glGetString(GL_VENDOR);

        return std::string(reinterpret_cast<const char*>(vendor));
    }

    std::string get_renderer() {
        auto renderer = glGetString(GL_RENDERER);

        return std::string(reinterpret_cast<const char*>(renderer));
    }

    std::vector<GLint> get_uuid() {
        GLint device_uuid[3];
        std::vector<GLint> ids;
        glGetIntegeri_v(GL_DEVICE_UUID_EXT, m_count, device_uuid);
        for(auto i = 0; i <= 3; i++) {
            ids.push_back(device_uuid[i]);
        }

        return ids;
    }
private:
    int m_count;
};

class opengl
{
public:
    opengl()
    {
        glfwInit();

        GLFWwindow* window;
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
        window = glfwCreateWindow(1, 1, "", NULL, NULL);
        glfwMakeContextCurrent(window);
    }

    ~opengl()
    {
        glfwTerminate();
    }

    std::vector<opengl_device> get_opengl_devices() {
        std::vector<opengl_device> devices;
        int device_count;
        glGetIntegerv(GL_NUM_DEVICE_UUIDS_EXT, &device_count);

        if (device_count == 0) {
            return {};
        }

        for (int i = 0; i < device_count; i++) {
            devices.push_back({i});
        }

        return devices;
    }

private:
};

#endif // OPENGL_H
