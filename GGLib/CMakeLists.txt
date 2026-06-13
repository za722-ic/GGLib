# gather source files from the source directory
file(GLOB_RECURSE GGLIB_SOURCES "source/*.cpp" "source/*.c")

# GGLib is a static library
add_library(GGLib STATIC ${GGLIB_SOURCES})

# expose GGLib include folder to anyone linking this library
target_include_directories(GGLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# link SDL libraries to GGLib
# PUBLIC means Demo_Sandbox will implicitly get these dependencies too (i think)
target_link_libraries(GGLib PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    SDL3_ttf::SDL3_ttf
)




