#pragma once

#include <optional>

#include "GG/UI/Visitor.h"
#include "GG/Rendering/Canvas.h"
#include "GG/Input/InputManager.h"
#include "GG/UI/Enums.h"

// TODO implement margin (left top right bottom) (or calculate margin from gap)
class Element
{
public: // TODO: protected
	// x and y are relative to parent. for absolute coordinates, see screenX, screenY
	int x = 0;
	int y = 0;
	int w = 50;
	int h = 10;

	int xAbs = 0;
	int yAbs = 0;

	int preferredWidth = 0;
	int preferredHeight = 0;

	int paddingLeft = 0;
	int paddingTop = 0;
	int paddingRight = 0;
	int paddingBottom = 0;

	int gap = 0;

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

	bool isVisible = true;
	int radius = 12;
	int borderThickness = 0;
	int shadowThickness = 0;
	SDL_Color borderColor;

	static InputManager* inputManager; // TODO: is this the best way of doing things (also see add() func of Container)

public:
	// TODO see: virtual destructors
	virtual ~Element() {}

	virtual void accept(Visitor& visitor) = 0;

	void setChildGap(int gap);

	void setMinWidth(int minW);
	void setMinWidthAuto();
	int getMinWidth();

	void setMaxWidth(int maxW);
	void setMaxWidthAuto();
	int getMaxWidth();

	void setMinHeight(int minH);
	void setMinHeightAuto();
	int getMinHeight();

	void setMaxHeight(int maxH);
	void setMaxHeightAuto();
	int getMaxHeight();

	void setPreferredWidth(int prefW);
	void setPreferredHeight(int prefH);
	void setXAbs(int newX);
	void setYAbs(int newY);

	int getPreferredWidth();
	int getPreferredHeight();
	int getX();
	int getY();

	void setPadding(int padding);
	void setPadding(int left, int right, int top, int bottom);

	void setWidthAbs(int w);
	void setHeightAbs(int h);

	bool isAutosize(Axis axis);
	bool isGrowable(Axis axis);
	bool isShrinkable(Axis axis);
	bool isResizable(Axis axis);

	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	void setColor(int hexCode);
	void setColor(const SDL_Color& color);

	virtual void render(Canvas* canvas);
};
