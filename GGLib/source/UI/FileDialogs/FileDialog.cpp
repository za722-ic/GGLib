#include "GG/UI/FileDialogs/FileDialog.h"

void SDLCALL GG::FileDialog::dialogCallback(void* userdata, const char* const* filelist, int filter)
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