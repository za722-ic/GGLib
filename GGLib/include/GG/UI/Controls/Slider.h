#pragma once

#include "GG/Input/DragManager.h"
#include "GG/UI/Control.h"
#include "GG/MoreMath/MoreMath.h"

// TODO: template this to support ints or floats or doubles or other numeric types

namespace GG
{
	class Slider : public Control
	{
	public:
		// TODO: setters, getters
		float min = 0;
		float max = 100;
		float value = 50;
		float interval = 1;

		int rKnob = 7;     // of knob --> TODO: non circular knobs? or just leave it to the user to override render method if they want to?
		int numTriangles = 6;

		int rRail;
		int textPadding; // TODO: text should also be able to go on the left OR RIGHT (maybe also top/bottom). also padding should have a minimum value of r, right? allow negative padding though in case someone is insane --> use setters/getters

		bool isMouseHovering = false;
		bool isRenderValueStr = true;

		bool isSliderBeingDragged = false;

		Slider();

		void render(Canvas* canvas) override;


	private:
		void onMouseDown(int mouseX, int mouseY) override;
		void onMouseDrag(int mouseX, int mouseY) override;
		void onMouseEnter(int mouseX, int mouseyY) override;
		void onMouseExit(int mouseX, int mouseyY) override;
		void uiScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY);
	};
}
