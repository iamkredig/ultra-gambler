cmake_minimum_required(VERSION 4.1)
project(floppy_c)

set(CMAKE_CXX_STANDARD 20)

set(HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(floppy_c src/main.cpp)

if (APPLE)
    find_package(PNG REQUIRED)
    find_library(GLUT_LIBRARY GLUT)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COCOA_LIBRARY Cocoa)

    target_link_libraries(floppy_c PRIVATE
            ${PNG_LIBRARIES}
            ${GLUT_LIBRARY}
            ${OPENGL_LIBRARY}
            ${COCOA_LIBRARY}
    )

    target_compile_options(floppy_c PRIVATE -Wno-inconsistent-missing-override)
endif ()

target_include_directories(floppy_c PRIVATE
        ${HEADER_DIR}
        ${PNG_INCLUDE_DIRS}
)
