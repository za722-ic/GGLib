# gather source files
file(GLOB_RECURSE DEMO_SOURCES "source/*.cpp" "source/*.c")

# create executable, ensuring Main.cpp at root is included
add_executable(Demo_Sandbox Main.cpp ${DEMO_SOURCES})

# set working directory for Visual Studio (Windows)
set_target_properties(Demo_Sandbox PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Demo_Sandbox>"
)
    
# sdd Demo_Sandbox specific include directory
target_include_directories(Demo_Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# link GGLib
target_link_libraries(Demo_Sandbox PRIVATE GGLib)

# this runs after the executable is built
add_custom_command(TARGET Demo_Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:Demo_Sandbox>/resources"
    COMMENT "Copying resources to the build directory..."
)