cmake_minimum_required(VERSION 3.20)
project(GGLIB VERSION 1.0.0 LANGUAGES C CXX)

# using C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# force static linking for all subprojects (including vendor libraries)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

# SDL3 specific flags to ensure it only builds static components
set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build a static version of the library" FORCE)

# add subdirs in dependency order
add_subdirectory(Vendor)
add_subdirectory(GGLib)
add_subdirectory(Demo_Sandbox)
add_subdirectory(Examples)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Demo_Sandbox)
