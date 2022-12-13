cmake_minimum_required(VERSION 3.5)

project(prime LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(prime src/main.cpp src/argvparse.h src/envvar.h src/opengl.h src/nvidia.h src/utils.h src/vulkan.h)
find_package(Vulkan REQUIRED)
target_link_libraries(prime ${Vulkan_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(prime ${GLEW_LIBRARIES})

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
target_link_libraries(prime ${OPENGL_LIBRARY})

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(prime glfw)

install(TARGETS prime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
