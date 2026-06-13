# disable as this has some weird dependency issues on windows
# AVIF is not essential to this project
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)

# Add SDL libraries
# EXCLUDE_FROM_ALL ensures we only compile what GGLib and Demo_Sandbox actually link to (i think)
add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(SDL_mixer EXCLUDE_FROM_ALL)
add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)