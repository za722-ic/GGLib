#pragma once

#include "GG/UI/FileDialogs/FileDialog.h"

namespace GG
{
	class OpenFileDialog : public FileDialog
	{
	public:
		std::vector<FileFilter> filters;

		bool acceptMultiple = false;

	public:
		void showDialog(SDL_Window* window) override;
	};
}
