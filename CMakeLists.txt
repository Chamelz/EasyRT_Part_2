cmake_minimum_required(VERSION 3.22)
project(EasyRT)

set(CMAKE_CXX_STANDARD 14)
link_directories(/usr/local/Cellar/glfw/3.3.7/lib)

include_directories(/Users/xyz/include/glm/)
include_directories(/Users/xyz/include)
include_directories(/Users/xyz/include/glad/include)
include_directories(/usr/local/Cellar/glfw/3.3.7/include)

# dependency

add_executable(EasyRT main.cpp common.h /Users/xyz/include/glad/src/glad.c)

target_link_libraries(
        EasyRT
        glfw
)