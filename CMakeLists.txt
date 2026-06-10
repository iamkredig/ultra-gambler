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
        src/entities/Win.h
        src/GameState.h
        src/logic/GameLogic.cpp
        src/logic/GameLogic.h
        src/logic/InputHandler.cpp
        src/logic/InputHandler.h
        src/logic/Renderer.cpp
        src/logic/Renderer.h
        src/assets/Assets.cpp
        src/assets/Assets.h
        src/logic/FormatNumber.h
        src/logic/QuantumRNG.cpp
        src/logic/QuantumRNG.h)

find_path(PNG_INCLUDE_DIR NAMES png.h PATHS /opt/homebrew/include /usr/local/include) #static PNG LIB in build

if (APPLE)
    find_library(PNG_STATIC NAMES libpng.a PATHS /opt/homebrew/lib /usr/local/lib) #static PNG LIB in build
    find_library(ZLIB_LIBRARY NAMES z) #static PNG LIB in build
    find_library(GLUT_LIBRARY GLUT)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COCOA_LIBRARY Cocoa)
    set(Python3_ROOT_DIR /Library/Frameworks/Python.framework/Versions/3.13)
    find_package(Python3 COMPONENTS Development REQUIRED)

    target_link_libraries(ultra_gambler PRIVATE
            ${PNG_STATIC}
            ${ZLIB_LIBRARY}
            ${GLUT_LIBRARY}
            ${OPENGL_LIBRARY}
            ${COCOA_LIBRARY}
            ${Python3_LIBRARIES}
    )

    target_compile_options(ultra_gambler PRIVATE -Wno-inconsistent-missing-override)
endif ()

add_custom_target(CopyPythonScript ALL
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/src/quantum_rng.py"
        "$<TARGET_FILE_DIR:ultra_gambler>/quantum_rng.py")

add_custom_command(TARGET ultra_gambler POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:ultra_gambler>/assets")

add_dependencies(ultra_gambler CopyPythonScript)

target_include_directories(ultra_gambler PRIVATE
        ${HEADER_DIR}
        ${PNG_INCLUDE_DIR} #static PNG LIB in build
        ${Python3_INCLUDE_DIRS}
)
