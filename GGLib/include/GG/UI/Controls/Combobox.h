#pragma once

#include "GG/UI/Control.h"

namespace GG
{
	/* 
		TODO: this does not receive input events under the updated top-down input scheme
		since it still needs to use onMouseEvent

		since when the combobiox is expanded, the mouse needs to interact with locations outside
		the explicit bounds of the combobox.
		to solve this, maybe make it so that we can distinguish between the hitbox bounds of a control,
		its layout bounds, and its visual bounds

		so e.g., for layout, this is just a small rectangle even when expanded
		but when expanded the visual bounds and hitbox update to be taller than the layout bounds
	*/
	class Combobox : Control, public MouseEventListener
	{
	protected:

		SDL_Color backColor = { 200, 200, 200, 255 };
		SDL_Color foreColor = { 0, 0, 0, 255 };

		std::string text = "Select option";

		std::vector<std::string> options = { "A", "B", "C" };

	public:

		// bounds
		void setBounds(int _x, int _y, int _w, int _h);

		// color
		void setForeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		void setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

		// text
		void setText(std::string _text);
		std::string getText() const;

		void render(Canvas* canvas) override;

	private:

		// mouse events
		bool isExpanded = false;
		int indexToHighlight = -1;
		bool isInBounds = false;
		void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;
	};
}
