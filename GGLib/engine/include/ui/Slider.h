#pragma once

#include "DragManager.h"
#include "UIControl.h"
#include "MoreMath.h"

// TODO: template this to support ints or floats or doubles or other numeric types

class Slider : UIControl
{

private:
	DragManager dragManager;

public:
	// TODO: setters, getters

	int min = 0;
	int max = 100;
	int value = 50;
	int interval = 1;

	int rKnob=10.0f;     // of knob --> TODO: non circular knobs? or just leave it to the user to override render method if they want to?
	int rRail=std::min(h,w)/2;
	int textPadding = rKnob + 10;; // TODO: text should also be able to go on the left. also padding should have a minimum value of r, right? allow negative padding though in case someone is insane --> use setters/getters

	bool isMouseHovering = false;
	bool isRenderValueStr = true;
	bool isRenderTicks = true;

	bool isSliderBeingDragged = false;

	Slider()
	{
		w = 500; h = 10;

		dragManager.setOnClick([&](int mouseX, int mouseY){
			if (isCoordInSliderBounds(mouseX, mouseY))
				value = (int)std::roundf(MoreMath::map(mouseX, x, x + w, min, max));
		});
		dragManager.setOnDragStart([&](int mouseX, int mouseY) {
			isSliderBeingDragged = isCoordInSliderBounds(mouseX, mouseY);
		});
		dragManager.setOnDragContinue([&](int mouseX, int mouseY) {
			// we only care if the user _started_ dragging within our bounds
			if (!isSliderBeingDragged) return;
				
			// change slider value based on mouseX
			// TODO this only applies if slider is horizontal --> vertical sliders? diagonal sliders?
			value = (int)std::roundf(MoreMath::mapAndClamp(mouseX, x, x + w, min, max));
		});
		dragManager.setOnDragEnd([&](int mouseX, int mouseY) {
			isSliderBeingDragged = false;
		});
	}

	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{
		dragManager.processDrag(mouseEventType, mouseX, mouseY);

		isMouseHovering = isCoordInSliderBounds(mouseX, mouseY);
	}

	void render(Canvas* canvas) override
	{
		canvas->setColor(32, 255, 100);
		canvas->renderRoundedRect(x, y, w, h, rRail); // TODO: make border radius configurable, and make it such that 

		if (isRenderValueStr)
		{
			canvas->setColor(255);
			canvas->setAlignment(Canvas::Alignment::CENTER_LEFT);
			canvas->drawString(std::to_string(value), x + w + textPadding, y + h / 2);
			canvas->setAlignment(Canvas::Alignment::TOP_LEFT);
			canvas->setColor(32, 255, 100);
		}

		if (isRenderTicks)
		{
			int i = min + interval;
			while (i <= max - interval)
			{
				int tickX = MoreMath::map(i, min, max, x+rRail, x + w-rRail);
				canvas->drawLine(tickX, y, tickX, y + h);

				i += interval;
			}
		}

		if (isMouseHovering)
			canvas->setColor(128);
		else
			canvas->setColor(32, 100, 80);

		int knobX = MoreMath::map(value, min, max, x+rRail, x+w-rRail);
		canvas->renderRegularPolygon(knobX, y+h/2, 16, 0.0f, rKnob);
		canvas->setColor(32, 150, 100);
		canvas->renderRegularPolygon(knobX, y+h/2, 16, 0.0f, rKnob*0.7f);
	}

private:
	bool isCoordInSliderBounds(int coordX, int coordY)
	{
		if (coordY < y || coordY > y + h) return false;
		if (coordX < x || coordX > x + w) return false;
		return true;
	}
};
