cmake_minimum_required(VERSION 3.12)

project(paint)
add_executable(paint paint.cc)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(paint ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
