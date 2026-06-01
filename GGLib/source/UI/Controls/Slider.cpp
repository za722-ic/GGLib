#include "GG/UI/Controls/Slider.h"

GG::Slider::Slider()
{
	setHeightAbs(6);
	setMinWidth(170);

	isDragListener = true;
	isMouseEventListener = true;
	isScrollEventListener = true;

	rRail = std::min(h, w) / 2;
	textPadding = rKnob + 5;
	borderThickness = 3;
}

void GG::Slider::onMouseDown(int mouseX, int mouseY)
{
	value = (int)std::roundf(MoreMath::map(mouseX, screenX, screenX + w, min, max));
}
void GG::Slider::onMouseDrag(int mouseX, int mouseY)
{
	// change slider value based on mouseX
	float unsnappedValue = MoreMath::map(mouseX, screenX, screenX + w, min, max);
	float intervalsFromStart = std::round(((unsnappedValue - min) / interval));
	float snappedValue = intervalsFromStart * interval + min;
	value = MoreMath::clamp(snappedValue, min, max);
}
void GG::Slider::onMouseEnter(int mouseX, int mouseyY)
{
	isMouseHovering = true;
}
void GG::Slider::onMouseExit(int mouseX, int mouseyY)
{
	isMouseHovering = false;
}
void GG::Slider::uiScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY)
{
	value = MoreMath::clamp(value + interval * scrollY, min, max);
}



void GG::Slider::render(Canvas* canvas)
{
	int x = screenX;
	int y = screenY;

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
