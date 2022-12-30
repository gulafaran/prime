cmake_minimum_required(VERSION 3.5)
set(PROJECT_VERSION "0.0.1")

project(prime LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(PRIME_SRC
    src/prime.cpp
    src/configparse.h
    src/configparse.cpp
    src/xdg.h
    src/xdg.cpp)

set(PRIMEQT_SRC
    src/primeqt.cpp
    src/primewindow.h
    src/primewindow.cpp
    src/primewindow.ui
    src/configparse.h
    src/configparse.cpp
    src/opengl.h
    src/opengl.cpp
    src/xdg.h
    src/xdg.cpp
    src/vulkan.h
    src/vulkan.cpp)

add_executable(prime ${PRIME_SRC})
add_executable(primeqt ${PRIMEQT_SRC})

find_package(Vulkan REQUIRED)
target_link_libraries(primeqt ${Vulkan_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(primeqt ${GLEW_LIBRARIES})

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
target_link_libraries(primeqt ${OPENGL_LIBRARY})

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(primeqt glfw)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)

target_link_libraries (
    primeqt
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

add_definitions(
    ${Qt5Core_DEFINITIONS}
    ${Qt5Widgets_DEFINITIONS}
)

install(TARGETS prime DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS primeqt DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
