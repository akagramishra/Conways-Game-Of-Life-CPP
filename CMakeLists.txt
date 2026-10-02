#only the CMakeLists.txt is written by AI

cmake_minimum_required(VERSION 3.25)

project(Game_Of_Life
    VERSION 0.1.0
    DESCRIPTION "Conway's Game of Life"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        Debug Release RelWithDebInfo MinSizeRel)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
foreach(cfg IN ITEMS DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg} "${CMAKE_BINARY_DIR}/bin")
endforeach()

set(RAYLIB_VERSION 6.0)

# --- Locally installed raylib -------------------------------------------------
# No network / FetchContent. raylib must already be present on this machine:
#   1. as an installed CMake package (vcpkg, system prefix, ...), or
#   2. as a local raylib source tree, built as part of this project.
set(RAYLIB_DIR "" CACHE PATH
    "Path to a local raylib source tree (the folder containing src/raylib.h)")

# Let find_package() see vcpkg's installed trees even without the vcpkg toolchain
# file. The user-wide MSBuild integration injects vcpkg's headers and import
# libraries anyway, so CMake must resolve raylib to that same copy.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    foreach(_vcpkg_root IN ITEMS "$ENV{VCPKG_ROOT}" "C:/vcpkg" "$ENV{USERPROFILE}/vcpkg")
        if(_vcpkg_root AND IS_DIRECTORY "${_vcpkg_root}/installed")
            file(GLOB _vcpkg_trees LIST_DIRECTORIES true "${_vcpkg_root}/installed/*")
            foreach(_tree IN LISTS _vcpkg_trees)
                if(IS_DIRECTORY "${_tree}/share")
                    list(APPEND CMAKE_PREFIX_PATH "${_tree}")
                endif()
            endforeach()
        endif()
    endforeach()
endif()

find_package(raylib ${RAYLIB_VERSION} QUIET)

if(raylib_FOUND)
    message(STATUS "Using installed raylib ${raylib_VERSION} from ${raylib_DIR}")
else()
    set(_raylib_candidates
        "${RAYLIB_DIR}"
        "$ENV{RAYLIB_DIR}"
        "$ENV{RAYLIB_PATH}"
        "C:/raylib/raylib"
        "$ENV{USERPROFILE}/raylib"
        "$ENV{HOME}/raylib"
        "${CMAKE_CURRENT_SOURCE_DIR}/../raylib")

    set(_raylib_src "")
    foreach(_cand IN LISTS _raylib_candidates)
        if(_cand AND EXISTS "${_cand}/src/raylib.h" AND EXISTS "${_cand}/CMakeLists.txt")
            set(_raylib_src "${_cand}")
            break()
        endif()
    endforeach()

    if(NOT _raylib_src)
        message(FATAL_ERROR
            "raylib ${RAYLIB_VERSION} was not found locally. "
            "Install it, or point CMake at your local raylib source tree: "
            "cmake -B build -DRAYLIB_DIR=<path-to-raylib>")
    endif()

    message(STATUS "Building local raylib source tree: ${_raylib_src}")

    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(CUSTOMIZE_BUILD OFF CACHE BOOL "" FORCE)

    add_subdirectory("${_raylib_src}" "${CMAKE_BINARY_DIR}/_raylib" EXCLUDE_FROM_ALL SYSTEM)
endif()

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Game.cpp
    src/Grid.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework CoreAudio")
elseif(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads m dl)
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Place any shared libraries raylib needs next to the executable.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMAND_EXPAND_LISTS
    COMMENT "Copying runtime dependencies")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
        COMMENT "Copying assets")
endif()

if(MSVC)
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
endif()

option(HIDE_CONSOLE "Hide the console window in Release builds" ON)
if(WIN32 AND HIDE_CONSOLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>)
    # The GUI subsystem looks for WinMain; keep the standard main() entry point.
    if(MSVC)
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<NOT:$<CONFIG:Debug>>:/ENTRY:mainCRTStartup>)
    endif()
endif()

if(EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PRIVATE
        -sUSE_GLFW=3 -sASYNCIFY -sALLOW_MEMORY_GROWTH)
endif()
