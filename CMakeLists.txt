cmake_minimum_required(VERSION 3.22)
project(EasyRT)

set(CMAKE_CXX_STANDARD 14)

link_directories(/usr/local/Cellar/freeglut/3.2.2/lib)
link_directories(/usr/local/Cellar/glew/2.2.0_1/lib)

include_directories(/Users/xyz/include/glm/)
include_directories(/Users/xyz/include)
include_directories(/usr/local/Cellar/freeglut/3.2.2/include)
include_directories(/usr/local/Cellar/glew/2.2.0_1/include)

# dependency
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLM REQUIRED)

add_executable(EasyRT main.cpp common.h)

target_link_libraries(
        EasyRT
        GLUT::GLUT
        GLEW::GLEW
)
