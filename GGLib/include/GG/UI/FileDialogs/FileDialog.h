#pragma once

#include <SDL3/SDL.h>

#include <string>
#include <vector>
#include <functional>
#include <cassert>
#include <iostream>

namespace GG
{
	typedef SDL_DialogFileFilter FileFilter;

	class FileDialog
	{
	public:
		std::string initialDirectory;
		std::function<void(const std::vector<std::string> files)> onDialogClose;

	public:
		// this function returns immediately
		// once the file dialog is closed, dialogCallback() is called
		// which in-turn calls the user's onDialogClose
		virtual void showDialog(SDL_Window* window) = 0;

	protected:
		static void SDLCALL dialogCallback(void* userdata, const char* const* filelist, int filter);
	};
}
