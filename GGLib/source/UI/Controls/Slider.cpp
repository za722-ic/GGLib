#include "GG/UI/Controls/Slider.h"

Slider::Slider()
{
	setHeightAbs(6);
	setMinWidth(170);

	rRail = std::min(h, w) / 2;
	textPadding = rKnob + 5;
	borderThickness = 3;

	dragManager.setOnClick([&](int mouseX, int mouseY) {
		if (isCoordInSliderBounds(mouseX, mouseY))
			value = (int)std::roundf(MoreMath::map(mouseX, screenX, screenX + w, min, max));
		});
	dragManager.setOnDragStart([&](int mouseX, int mouseY) {
		isSliderBeingDragged = isCoordInSliderBounds(mouseX, mouseY);
		});
	dragManager.setOnDragContinue([&](int mouseX, int mouseY) {
		// we only care if the user _started_ dragging within our bounds
		if (!isSliderBeingDragged) return;

		// change slider value based on mouseX
		// TODO this only applies if slider is horizontal --> vertical sliders? diagonal sliders?
		// value = (int)std::roundf(MoreMath::mapAndClamp(mouseX, screenX, screenX + w, min, max));

		// TODO: (i) clean up and (ii) the same calculations should go in setOnClick
		float unsnappedValue = MoreMath::map(mouseX, screenX, screenX + w, min, max);
		float intervalsFromStart = std::round(((unsnappedValue - min) / interval));
		float snappedValue = intervalsFromStart * interval + min;
		value = MoreMath::clamp(snappedValue, min, max);
		});
	dragManager.setOnDragEnd([&](int mouseX, int mouseY) {
		isSliderBeingDragged = false;
		});
}

void Slider::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
	dragManager.processDrag(mouseEventType, mouseX, mouseY);

	isMouseHovering = isCoordInSliderBounds(mouseX, mouseY);
}

void Slider::onScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY)
{
	if (isCoordInSliderBounds(mouseX, mouseY))
		value = MoreMath::clamp(value + interval * scrollY, min, max);
}

void Slider::render(Canvas* canvas)
{
	int x = screenX;
	int y = screenY;

	//canvas->setColor(255, 0, 0, 128);
	//canvas->fillRect(x, y, w, h);
	//return;


	// draw text
	int textWidth, textHeight;
	canvas->getTextDimensions(std::to_string(value), &textWidth, &textHeight);
	canvas->setColor(255);
	canvas->drawString(std::to_string(value), x - textWidth - textPadding, y + h / 2 - textHeight / 2);

	// calculate knob position
	int knobMinX = x - rKnob;
	int knobMaxX = x + w - rKnob;
	int knobX = MoreMath::map(value, min, max, knobMinX, knobMaxX);
	int knobY = y + h / 2 - rKnob;

	// draw slider bar from start to knob
	int wStartToKnob = knobX - knobMinX;
	canvas->setColor(32, 225, 100);
	canvas->fillRoundedRect(x, y, wStartToKnob, h, rRail); // TODO: make border radius configurable, and make it such that 
	canvas->drawRoundedRect(x - borderThickness, y - borderThickness, wStartToKnob + 2 * borderThickness, h + 2 * borderThickness, rRail + borderThickness, borderThickness, { 0,0,0,96 }, { 0,0,0,0 }, numTriangles);

	// draw slider bar from knob to end
	int wKnobToEnd = w - wStartToKnob;
	canvas->setColor(255, 32, 100);
	canvas->fillRoundedRect(x + wStartToKnob, y, wKnobToEnd, h, rRail); // TODO: make border radius configurable, and make it such that 
	canvas->drawRoundedRect(x + wStartToKnob - borderThickness, y - borderThickness, wKnobToEnd + 2 * borderThickness, h + 2 * borderThickness, rRail + borderThickness, borderThickness, { 0,0,0,96 }, { 0,0,0,0 }, numTriangles);


	if (isMouseHovering)
		canvas->setColor(180);
	else
		canvas->setColor(230);

	canvas->fillRoundedRect(knobX, knobY, 2 * rKnob, 2 * rKnob, rKnob, numTriangles);

	int borderRadius = rKnob + borderThickness;
	int knobDiameter = 2 * rKnob;
	canvas->drawRoundedRect(knobX - borderThickness, knobY - borderThickness, knobDiameter + 2 * borderThickness, knobDiameter + 2 * borderThickness, borderRadius, borderThickness, { 0,0,0,96 }, { 0,0,0,0 }, numTriangles);
}

// TODO: shoiuld this just get placed in Element?
bool Slider::isCoordInSliderBounds(int coordX, int coordY)
{
	if (coordY < screenY || coordY > screenY + h) return false;
	if (coordX < screenX || coordX > screenX + w) return false;
	return true;
}
