#pragma once

#include <functional>

#include "Control.h"

/*
TODO:

Making controls/containers scrollable if the size needed to fit their content exceeds that element's max size
In such a scenario, the following should happen:

- Scrollbar is added to the element
- Only render a certain viewport of the content instead of the entire logical area
	- This can be mostly acheived woth SDL_RenderSetClipRect
	- Can automatically cull elements who bottom is above the viewports top, and whose top is below the viewports bottom

- If something is at e.g. y=1000 in a container, but the viewport is at y = 800, then the rendering location relative to the container's top left is 1000-800=200

*/
class Scrollable : public Control
{
public:
	int viewportY = 0;
	int viewportX = 0;
	int logicalHeight = 5000; // TODO: logical height cannot be smaller than actual viewport h

	int scrollbarWidth = 10;
	int scrollbarPaddingVertical = 5;
	int scrollbarPaddingHorizontal = 15;
	int scrollbarMinHeight = 20;
	int scrollbarH;
	int scrollbarY;

	int scrollbarScrollInterval = 100;

	std::vector<Control*> children;


public:
	
	Scrollable()
	{
		setColor(0x407848);
		borderColor = { 38, 84, 44, 255 };
		borderThickness = 4;
		shadowThickness = 6;

		Control *c1 = new Control;
		c1->w = 100;
		c1->h = 200;
		c1->x = 100;
		c1->y = 100;
		c1->setColor(0x429ef5);
		children.push_back(c1);

		Control *c2 = new Control;
		c2->w = 300;
		c2->h = 600;
		c2->x = 300;
		c2->y = 800;
		c2->setColor(0xf0d630);
		children.push_back(c2);

		Control *c3 = new Control;
		c3->w = 300;
		c3->h = 300;
		c3->x = 600;
		c3->y = 200;
		c3->setColor(0xe63c8b);
		children.push_back(c3);

		Control *c4 = new Control;
		c4->w = 300;
		c4->h = 300;
		c4->x = 600;
		c4->y = 4000;
		c4->setColor(0xe63c8b);
		children.push_back(c4);
	}

	void render(Canvas* canvas) override
	{
		Element::render(canvas);

		int viewportH = h;
		int maxViewportY = logicalHeight - viewportH;

		scrollbarH = MoreMath::map(viewportH / logicalHeight, 0, 1, scrollbarMinHeight, viewportH - 2* scrollbarPaddingVertical);
		
		scrollbarY = MoreMath::map(viewportY, 0, maxViewportY, scrollbarPaddingVertical, h - scrollbarH - scrollbarPaddingVertical);

		canvas->setColor(255);
		canvas->fillRect(screenX + scrollbarPaddingHorizontal,
						 screenY + scrollbarY, 
						 scrollbarWidth, 
						 scrollbarH);

		canvas->drawString(std::to_string(viewportY), 0, 1000);
		canvas->drawString(std::to_string(scrollbarH), 0, 1020);

		SDL_Rect b = { screenX, screenY, w, h };
		SDL_RenderSetClipRect(canvas->getSDLRenderer(), &b);
		for (auto child : children)
		{
			child->screenX = this->screenX + child->x - viewportX;
			child->screenY = this->screenY + child->y - viewportY;

			child->render(canvas);
		}
		SDL_RenderSetClipRect(canvas->getSDLRenderer(), NULL);
	}

	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{
	}

	void onScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY) override
	{
		if (!isCoordInBounds(mouseX, mouseY)) return;

		viewportY += -scrollY * scrollbarScrollInterval;

		int viewportH = h;
		viewportY = MoreMath::clamp(viewportY, 0, logicalHeight - viewportH);
	}

private:
	int scrollbarMaxY()
	{
		int viewportH = h;
		
	}
	
	bool isCoordInBounds(int coordX, int coordY)
	{
		if (coordY < screenY || coordY > screenY + h) return false;
		if (coordX < screenX || coordX > screenX + w) return false;
		return true;
	}

};
