#pragma once

#include <optional>

#include "Visitor.h"
#include "Canvas.h"

enum class Axis
{
	HORIZONTAL,
	VERTICAL
};

// TODO implement margin (left top right bottom) (or calculate margin from gap)
class Element
{
public: // TODO: protected
	// x and y are relative to parent. for absolute coordinates, see screenX, screenY
	int x = 0;
	int y = 0;
	int w = 50;
	int h = 10;

	int preferredWidth;
	int preferredHeight;

	int paddingLeft;
	int paddingTop;
	int paddingRight;
	int paddingBottom;

	int gap;

	// std::nullopt means auto. auto means defer to the calculated versions of these values, rather than the user defined ones
	std::optional<int> minWidth = std::nullopt;
	std::optional<int> minHeight = std::nullopt;
	std::optional<int> maxWidth = std::nullopt;
	std::optional<int> maxHeight = std::nullopt;

	// TODO: actually calculate maxwidth and maxheight of children based on the maxwidth/height of parents?
	int minWidthCalculated = 1;
	int minHeightCalculated = 1;
	int maxWidthCalculated = INT_MAX;
	int maxHeightCalculated = INT_MAX;

	int screenX;
	int screenY;

	Container* parent;

	SDL_Color color;

	bool horizontalAutosize = false;
	bool verticalAutosize = false;

public:
	// TODO see: virtual destructors
	virtual ~Element() {}

	virtual void accept(Visitor& visitor) = 0;

	void setChildGap(int gap)
	{
		this->gap = gap;
	}

	void setMinWidth(int minW) { minWidth = minW; }
	void setMinWidthAuto() { minWidth = std::nullopt; }
	int getMinWidth()
	{
		return minWidth.has_value() ? minWidth.value() : minWidthCalculated;
	}

	void setMaxWidth(int maxW) { maxWidth = maxW; }
	void setMaxWidthAuto() { maxWidth = std::nullopt; }
	int getMaxWidth()
	{
		return maxWidth.has_value() ? maxWidth.value() : maxWidthCalculated;
	}

	void setMinHeight(int minH) { minHeight = minH; }
	void setMinHeightAuto() { minHeight = std::nullopt; }
	int getMinHeight()
	{
		return minHeight.has_value() ? minHeight.value() : minHeightCalculated;
	}

	void setMaxHeight(int maxH) { maxHeight = maxH; }
	void setMaxHeightAuto() { maxHeight = std::nullopt; }
	int getMaxHeight()
	{
		return maxHeight.has_value() ? maxHeight.value() : maxHeightCalculated;
	}

	// TODO --> this is just setting w/h, not preferredWidth or preferredHeight
	void setPreferredWidth(int prefW)
	{
		//preferredWidth = prefW;  
		w = prefW;
	}
	void setPreferredHeight(int prefH)
	{
		//preferredHeight = prefH;  
		h = prefH;
	}
	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }

	int getPreferredWidth() { return preferredWidth;  }
	int getPreferredHeight() { return preferredHeight;  }
	int getX() { return x; }
	int getY() { return y; }

	void setPadding(int padding)
	{
		paddingLeft = padding;
		paddingTop = padding;
		paddingRight = padding;
		paddingBottom = padding;
	}

	void setPadding(int left, int right, int top, int bottom)
	{
		this->paddingLeft = left;
		this->paddingTop = top;
		this->paddingRight = right;
		this->paddingBottom = bottom;
	}

	void setWidthAbs(int w)
	{
		setMinWidth(w);
		setMaxWidth(w);
		setPreferredWidth(w);
	}
	void setHeightAbs(int h)
	{
		setMinHeight(h);
		setMaxHeight(h);
		setPreferredHeight(h);
	}

	bool isAutosize(Axis axis)
	{
		if (axis == Axis::HORIZONTAL) return horizontalAutosize;
		if (axis == Axis::VERTICAL) return verticalAutosize;
	}

	bool isGrowable(Axis axis)
	{
		if (isAutosize(axis)) return false;
		if (axis == Axis::HORIZONTAL) return getMinWidth() < getPreferredWidth();
		if (axis == Axis::VERTICAL) return getMinHeight() < getPreferredHeight();
	}

	bool isShrinkable(Axis axis)
	{
		if (isAutosize(axis)) return false; // TODO check winforms that autosize trumps min/max sizing (cus w/autosize, a control is tied to its min size right?)
		if (axis == Axis::HORIZONTAL) return getPreferredWidth() < getMaxWidth();
		if (axis == Axis::VERTICAL) return getPreferredHeight() < getMaxHeight();
	}

	bool isResizable(Axis axis)
	{
		return isGrowable(axis) || isShrinkable(axis);
	}


	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
	{
		color = { r,g,b,a };
	}

	void setColor(int hexCode)
	{
		// hexCode is 32 bits ARGB, 8 bytes per channel

		//unsigned char a = (hexCode >> 24) & 0xFF;
		unsigned char a = 255;
		unsigned char r = (hexCode >> 16) & 0xFF;
		unsigned char g = (hexCode >> 8) & 0xFF;
		unsigned char b = (hexCode) & 0xFF;

		setColor(r, g, b, a);
	}

	virtual void render(Canvas* canvas)
	{
		int innerRadius = 12;
		int borderThickeness = 1;
		int outerRadius = innerRadius + borderThickeness;
		SDL_Color borderColor = { 109, 171, 118, color.a }; // TODO: this way of setting alpha is a hack --> move this border rendering to canvas

		// render this element
		canvas->setColor(borderColor);
		canvas->fillRoundedRect(screenX, screenY, w, h, outerRadius);

		canvas->setColor(color);
		canvas->fillRoundedRect(screenX + borderThickeness, screenY + borderThickeness, w - 2*borderThickeness, h - 2*borderThickeness, innerRadius);
	}
};
