#pragma once

#include "GG/UI/FileDialogs/FileDialog.h"

namespace GG
{
	class OpenFolderDialog : public FileDialog
	{
	public:
		bool acceptMultiple = false;

	public:
		void showDialog(SDL_Window* window) override;
	};
}
