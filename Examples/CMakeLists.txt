# define a reusable blueprint for all tge example apps
function(add_gglib_example APP_NAME)
    
    # gather source files for this specific app
    # (looks for main.cpp at the app root, and any files in a 'source' subfolder)
    file(GLOB_RECURSE APP_SOURCES 
        "${APP_NAME}/*.cpp" 
        "${APP_NAME}/source/*.cpp" 
        "${APP_NAME}/source/*.c"
    )

    # create executable
    add_executable(${APP_NAME} ${APP_SOURCES})
    
    # set the working directory for visual studio (windows)
    set_target_properties(${APP_NAME} PROPERTIES 
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${APP_NAME}>"
    )

    # add include directories (if the app has an 'include' folder)
    target_include_directories(${APP_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${APP_NAME}/include")

    # link against GGLib (this automatically brings in SDL3 stuff too)
    target_link_libraries(${APP_NAME} PRIVATE GGLib)

    # automatically copy the resources folder IF it exists for this app
    # e.g., if there is a resources folder, then we need to copy that to be relative to the executable so thta the executable can access those resources
    # TODO: this will be slow for large resources
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${APP_NAME}/resources")
        add_custom_command(TARGET ${APP_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/${APP_NAME}/resources"
                "$<TARGET_FILE_DIR:${APP_NAME}>/resources"
            COMMENT "Copying resources for ${APP_NAME}..."
        )
    endif()

endfunction()



# ======================================
#  HEY! ADD/REMOVE EXAMPLES HERE PLEASE
# ======================================

add_gglib_example(Demo_ButtonCounter)
add_gglib_example(Demo_ChapterSix)
add_gglib_example(Demo_GameOfLife)
add_gglib_example(Demo_ManyControls)
add_gglib_example(Demo_Noise)
add_gglib_example(Demo_Scrollables)