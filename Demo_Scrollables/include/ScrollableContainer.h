#pragma once

#include <functional>

#include "GG/UI/Container.h"
#include "GG/UI/Control.h"

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
class ScrollableContainer : public GG::Container
{
public:

	ScrollableContainer()
	{
		layoutMode = GG::LayoutMode::FLEX;
		verticalScroll = true;
		setMinHeight(0);

		setColor(0x407848);
		borderColor = { 38, 84, 44, 255 };
		borderThickness = 4;
		shadowThickness = 6;

		//Control* c1 = new Control;
		//c1->setWidthAbs(100);
		//c1->setHeightAbs(200);
		//c1->xAbs = 100;
		//c1->yAbs = 100;
		//c1->setColor(0x429ef5);
		//children.push_back(c1);

		//Control* c2 = new Control;
		//c2->setWidthAbs(300);
		//c2->setHeightAbs(600);
		//c2->xAbs = 300;
		//c2->yAbs = 800;
		//c2->setColor(0xf0d630);
		//children.push_back(c2);

		//Control* c3 = new Control;
		//c3->setWidthAbs(300);
		//c3->setHeightAbs(300);
		//c3->xAbs = 600;
		//c3->yAbs = 200;
		//c3->setColor(0xe63c8b);
		//children.push_back(c3);

		//Control* c4 = new Control;
		//c4->setWidthAbs(300);
		//c4->setHeightAbs(300);
		//c4->xAbs = 600;
		//c4->yAbs = 4000;
		//c4->setColor(0xe63c8b);
		//children.push_back(c4);
	}

	void render(GG::Canvas* canvas) override
	{
		Element::render(canvas);

		int viewportH = h;
		int maxViewportY = logicalHeight - viewportH;

		render_scrollbarH = GG::MoreMath::map(viewportH / logicalHeight, 0, 1, render_scrollbarMinHeight, viewportH - 2 * render_scrollbarPaddingVertical);
		render_scrollbarY = GG::MoreMath::map(viewportY, 0, maxViewportY, render_scrollbarPaddingVertical, h - render_scrollbarH - render_scrollbarPaddingVertical);

		canvas->setColor(150);
		canvas->fillRect(screenX + w - render_scrollbarPaddingHorizontal,
			screenY + render_scrollbarY,
			render_scrollbarWidth,
			render_scrollbarH
		);

		canvas->setColor(255, 0, 255, 255);
		canvas->drawString(std::to_string(viewportY), 0, 0);
		canvas->drawString(std::to_string(render_scrollbarH), 500, 0);
		canvas->setColor(255);

		// draw children of scrollable container, clipping them to the container bounds
		SDL_Rect b = { screenX, screenY, w, h };
		
		canvas->pushClipRect(b);
		{
			for (auto child : children)
			{
				child->render(canvas);
			}
		}
		canvas->popClipRect();
	}

	void onScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY) override
	{
		if (!isCoordInBounds(mouseX, mouseY)) return;

		viewportY += -scrollY * scrollbarScrollInterval;

		int viewportH = h;
		viewportY = GG::MoreMath::clamp(viewportY, 0, logicalHeight - viewportH);
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
