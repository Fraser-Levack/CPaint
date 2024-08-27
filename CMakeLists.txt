cmake_minimum_required(VERSION 3.10)
project(CPaint)

find_package(OpenGL REQUIRED)

# Manually specify the paths to GLUT
set(GLUT_INCLUDE_DIR "C:/MinGW/include")
set(GLUT_LIBRARY "C:/MinGW/lib/libfreeglut.a")

include_directories(${GLUT_INCLUDE_DIR})
link_directories("C:/path/to/external/mingw/lib")

add_executable(CPaint utils/array_utils.c main.c)

target_link_libraries(CPaint ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_link_libraries(CPaint -lopengl32 -g -lfreeglut)