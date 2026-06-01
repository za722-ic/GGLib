#include "GG/UI/Controls/Toggle.h"

GG::Toggle::Toggle()
{
	radius = 10;
	setHeightAbs(radius * 2);
	setWidthAbs(radius * 4);
	padding = 2;
	shadowThickness = 4;

	isMouseEventListener = true;
}

void GG::Toggle::render(Canvas* canvas)
{
	// render body
	if (isChecked)
		canvas->setColor(onBackColor);
	else
		canvas->setColor(offBackColor);

	canvas->fillRoundedRect(screenX, screenY, w, h, h / 2, trianglesPerCorner);
	canvas->drawRoundedRect(screenX - shadowThickness, screenY - shadowThickness,
		w + 2 * shadowThickness, h + 2 * shadowThickness,
		h / 2 + shadowThickness,
		shadowThickness,
		{ 0,0,0,96 }, { 0,0,0,0 },
		trianglesPerCorner
	);

	// render toggle button
	if (isMouseHovering)
		canvas->setColor(hoverToggleColor);
	else
		canvas->setColor(toggleColor);

	int toggleX;
	int toggleW = h - padding * 2;
	int toggleY = screenY + padding;
	if (isChecked)
		toggleX = screenX + w - toggleW - padding;
	else
		toggleX = screenX + padding;

	canvas->fillRoundedRect(toggleX, screenY + padding, toggleW, toggleW, h / 2 - padding, trianglesPerCorner);
	canvas->drawRoundedRect(toggleX - shadowThickness, toggleY - shadowThickness,
		toggleW + 2 * shadowThickness, toggleW + 2 * shadowThickness,
		toggleW / 2 + shadowThickness,
		shadowThickness,
		{ 0,0,0,96 }, { 0,0,0,0 },
		trianglesPerCorner);
}

void GG::Toggle::onMouseUp(int mouseX, int mouseY)
{
	isChecked = !isChecked;
}
void GG::Toggle::onMouseExit(int mouseX, int mouseY)
{
	isMouseHovering = false;
}
void GG::Toggle::onMouseEnter(int mouseX, int mouseY)
{
	isMouseHovering = true;
}
