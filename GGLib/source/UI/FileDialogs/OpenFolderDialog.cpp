#include "GG/UI/FileDialogs/OpenFolderDialog.h"

void GG::OpenFolderDialog::showDialog(SDL_Window* window)
{
	SDL_ShowOpenFolderDialog(
		FileDialog::dialogCallback,
		this, // see FileDialog::dialogCallback() for why
		window,
		initialDirectory.c_str(),
		acceptMultiple
	);
}
