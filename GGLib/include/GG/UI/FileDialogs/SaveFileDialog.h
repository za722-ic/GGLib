#pragma once

#include "GG/UI/FileDialogs/FileDialog.h"

namespace GG
{
	class SaveFileDialog : public FileDialog
	{
	public:
		std::vector<FileFilter> filters;

	public:
		void showDialog(SDL_Window* window) override;
	};
}
