#include "GG/UI/FileDialogs/OpenFileDialog.h"

void GG::OpenFileDialog::showDialog(SDL_Window* window)
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
