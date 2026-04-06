cmake_minimum_required(VERSION 4.1.2)
project(ultra_gambler)

set(CMAKE_CXX_STANDARD 20)

set(HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(ultra_gambler src/main.cpp
        src/entities/Column.h
        src/entities/Lever.h
        src/entities/Lever.cpp
        src/entities/Column.cpp
        src/entities/Win.cpp
        src/entities/Win.h)

if (APPLE)
    find_package(PNG REQUIRED)
    find_library(GLUT_LIBRARY GLUT)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COCOA_LIBRARY Cocoa)

    target_link_libraries(ultra_gambler PRIVATE
            ${PNG_LIBRARIES}
            ${GLUT_LIBRARY}
            ${OPENGL_LIBRARY}
            ${COCOA_LIBRARY}
    )

    target_compile_options(ultra_gambler PRIVATE -Wno-inconsistent-missing-override)
endif ()

target_include_directories(ultra_gambler PRIVATE
        ${HEADER_DIR}
        ${PNG_INCLUDE_DIRS}
)
