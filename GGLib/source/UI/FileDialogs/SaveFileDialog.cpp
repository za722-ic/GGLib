#pragma once

#include "GG/UI/FileDialogs/SaveFileDialog.h"

void SaveFileDialog::showDialog(SDL_Window* window)
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
