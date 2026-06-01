#pragma once

#include <memory>

#include "GG/UI/Control.h"
#include "GG/UI/Enums.h"
#include "GG/Rendering/Text.h"

namespace GG
{
	class Label : public Control
	{
	public:
		std::unique_ptr<Text> tttext = nullptr;

		VAlignmentMode verticalAlignmentMode = VAlignmentMode::CENTER;

	public:
		// TODO: make use of width, height --> crop text with "..." if out of bounds (how do other libs handle this?)

		Label(std::string textStr = "");

		void accept(Visitor& visitor) override;

		void render(Canvas* canvas) override;

		void setText(std::string newText);
		std::string getText();

		void setHAlignment(HAlignmentMode alignment);

		void setVAlignment(VAlignmentMode alignment);

		void setForeColor(SDL_Color color);
		void setBackColor(SDL_Color color);

		std::pair<int, int> getTextDimensions();
	};
}
