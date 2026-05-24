#pragma once

#include <SDL3/SDL.h>

#include <string>
#include <vector>
#include <functional>
#include <cassert>
#include <iostream>

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
	static void SDLCALL dialogCallback(void* userdata, const char* const* filelist, int filter)
	{
		// in showDialog(), we set userData to "this" i.e, the instance of the dialog class that made the call
		// here we retrieve that instance
		// we have to do this because the callback accepted by SDL_ShowOpenFileDialog is a function pointer, so the callback has to be a static member function
		// now that we have the instance, we can call user-defined lambas stored in the instance, circumventing the limitations of a static function
		auto* instance = static_cast<FileDialog*>(userdata);

		// user did not specify anything to do
		if (!(instance->onDialogClose)) return;
		
		// list of file paths
		std::vector<std::string> filePaths;

		// an error occurred
		if (!filelist)
		{
			std::cout << SDL_GetError() << std::endl;
			instance->onDialogClose(filePaths);
			return;
		}

		// no files selected
		if (!*filelist)
		{
			instance->onDialogClose(filePaths);
			return;
		}

		// retrieve file paths
		while (*filelist) 
		{
			filePaths.push_back(std::string(*filelist));
			filelist++;
		}

		// call user defined code, supplying retrieved file paths
		instance->onDialogClose(filePaths);
	}
};

class OpenFileDialog : public FileDialog
{
public:
	std::vector<FileFilter> filters;

	bool acceptMultiple = false;

public:
	void showDialog(SDL_Window* window) override
	{
		SDL_ShowOpenFileDialog(
			FileDialog::dialogCallback,
			this, // see FileDialog::fileDialogCallback() for why
			window,
			filters.empty() ? NULL : filters.data(),
			filters.size(),
			initialDirectory.c_str(),
			acceptMultiple	
		);
	}
};

class OpenFolderDialog : public FileDialog
{
public:
	bool acceptMultiple = false;

public:
	void showDialog(SDL_Window* window) override
	{
		SDL_ShowOpenFolderDialog(
			FileDialog::dialogCallback,
			this, // see FileDialog::dialogCallback() for why
			window,
			initialDirectory.c_str(),
			acceptMultiple	
		);
	}
};

class SaveFileDialog : public FileDialog
{
public:
	std::vector<FileFilter> filters;

public:
	void showDialog(SDL_Window* window) override
	{
		SDL_ShowSaveFileDialog(
			FileDialog::dialogCallback,
			this, // see FileDialog::dialogCallback() for why
			window,
			filters.empty() ? NULL : filters.data(),
			filters.size(),
			initialDirectory.c_str()
		);
	}
};
