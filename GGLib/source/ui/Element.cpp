#include "ui/Element.h"


InputManager* Element::inputManager = nullptr;

void Element::setChildGap(int gap)
{
	this->gap = gap;
}

void Element::setMinWidth(int minW) { minWidth = minW; }
void Element::setMinWidthAuto() { minWidth = std::nullopt; }
int Element::getMinWidth()
{
	return minWidth.has_value() ? minWidth.value() : minWidthCalculated;
}

void Element::setMaxWidth(int maxW) { maxWidth = maxW; }
void Element::setMaxWidthAuto() { maxWidth = std::nullopt; }
int Element::getMaxWidth()
{
	return maxWidth.has_value() ? maxWidth.value() : maxWidthCalculated;
}

void Element::setMinHeight(int minH) { minHeight = minH; }
void Element::setMinHeightAuto() { minHeight = std::nullopt; }
int Element::getMinHeight()
{
	return minHeight.has_value() ? minHeight.value() : minHeightCalculated;
}

void Element::setMaxHeight(int maxH) { maxHeight = maxH; }
void Element::setMaxHeightAuto() { maxHeight = std::nullopt; }
int Element::getMaxHeight()
{
	return maxHeight.has_value() ? maxHeight.value() : maxHeightCalculated;
}

// TODO --> this is just setting w/h, not preferredWidth or preferredHeight
void Element::setPreferredWidth(int prefW)
{
	//preferredWidth = prefW;  
	w = prefW;
}
void Element::setPreferredHeight(int prefH)
{
	//preferredHeight = prefH;  
	h = prefH;
}

/*
	TODO: invalidation/dirty flag
	
	if you create a button, and set it so that the button moves itself when clicked
	then it can go from being below the cursor to somewhere where it is not below the cursor

	issue: the button still renders itself as if it is below the cursor (with backColorOnHover)
	this is resolved as soon as the cursor is moved, but it feels a little off

	for an example of the issue, see Demo_Sandbox in the commit where absolute layouts are added

	this whole thing should be solved with the introduction of invalidation/dirty flags when you implement them later on (hopefully)
	since e.g., if setXAbs() is called, then you'll invalidate the layout, which will cause fresh layout calculations, which will cause a re-render, without the user moving their mouse first
*/
void Element::setXAbs(int newX) { xAbs = newX; }
void Element::setYAbs(int newY) { yAbs = newY; }

int Element::getPreferredWidth() { return preferredWidth; }
int Element::getPreferredHeight() { return preferredHeight; }
int Element::getX() { return x; }
int Element::getY() { return y; }

void Element::setPadding(int padding)
{
	paddingLeft = padding;
	paddingTop = padding;
	paddingRight = padding;
	paddingBottom = padding;
}

void Element::setPadding(int left, int right, int top, int bottom)
{
	this->paddingLeft = left;
	this->paddingTop = top;
	this->paddingRight = right;
	this->paddingBottom = bottom;
}

void Element::setWidthAbs(int w)
{
	setMinWidth(w);
	setMaxWidth(w);
	setPreferredWidth(w);
}
void Element::setHeightAbs(int h)
{
	setMinHeight(h);
	setMaxHeight(h);
	setPreferredHeight(h);
}

bool Element::isAutosize(Axis axis)
{
	if (axis == Axis::HORIZONTAL) return horizontalAutosize;
	if (axis == Axis::VERTICAL) return verticalAutosize;
}

bool Element::isGrowable(Axis axis)
{
	if (isAutosize(axis)) return false;
	if (axis == Axis::HORIZONTAL) return getMinWidth() < getPreferredWidth();
	if (axis == Axis::VERTICAL) return getMinHeight() < getPreferredHeight();
}

bool Element::isShrinkable(Axis axis)
{
	if (isAutosize(axis)) return false; // TODO check winforms that autosize trumps min/max sizing (cus w/autosize, a control is tied to its min size right?)
	if (axis == Axis::HORIZONTAL) return getPreferredWidth() < getMaxWidth();
	if (axis == Axis::VERTICAL) return getPreferredHeight() < getMaxHeight();
}

bool Element::isResizable(Axis axis)
{
	return isGrowable(axis) || isShrinkable(axis);
}


void Element::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	color = { r,g,b,a };
}

void Element::setColor(int hexCode)
{
	// hexCode is 32 bits ARGB, 8 bytes per channel

	//unsigned char a = (hexCode >> 24) & 0xFF;
	unsigned char a = 255;
	unsigned char r = (hexCode >> 16) & 0xFF;
	unsigned char g = (hexCode >> 8) & 0xFF;
	unsigned char b = (hexCode) & 0xFF;

	setColor(r, g, b, a);
}

void Element::setColor(const SDL_Color& color)
{
	this->color = color;
}

void Element::render(Canvas* canvas)
{
	if (!isVisible) return;

	int x = screenX;
	int y = screenY;


	int borderRadius = radius + borderThickness;
	int shadowRadius = borderRadius + shadowThickness;
	int numTriangles = 12;

	canvas->setColor(color);
	canvas->fillRoundedRect(x, y, w, h, radius, numTriangles);

	canvas->drawRoundedRect(x - borderThickness, y - borderThickness, w + 2 * borderThickness, h + 2 * borderThickness, borderRadius, borderThickness, borderColor, borderColor, numTriangles);

	canvas->drawRoundedRect(x - borderThickness - shadowThickness,
		y - borderThickness - shadowThickness,
		w + 2 * borderThickness + 2 * shadowThickness,
		h + 2 * borderThickness + 2 * shadowThickness,
		shadowRadius,
		shadowThickness,
		{ 0,0,0,128 }, { 0,0,0,0 },
		numTriangles);
}
