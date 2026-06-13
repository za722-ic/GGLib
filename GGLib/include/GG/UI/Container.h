#pragma once

#include <vector>
#include <queue>
#include <optional>

#include <SDL3/SDL.h>

#include "GG/MoreMath/MoreMath.h"
#include "GG/UI/Element.h"


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

namespace GG
{
	class Container : public Element
	{
	public:
		bool verticalScroll = false;

		float viewportY_percentage = 0.0f;
		int viewportY = 0;
		int viewportX = 0;
		int logicalHeight = 0; // TODO: logical height cannot be smaller than actual viewport h

		int render_scrollbarWidth = 10;
		int render_scrollbarPaddingVertical = 5;
		int render_scrollbarPaddingHorizontal = 15;
		int render_scrollbarMinHeight = 20;
		int render_scrollbarH;
		int render_scrollbarY;

		float scrollbarScrollInterval = 50; // px

	public: // TODO: protected
		std::vector<Element*> children;

		LayoutDirection layoutDirection = LayoutDirection::TOP_TO_BOTTOM;

		VAlignmentMode verticalAlignmentMode = VAlignmentMode::CENTER;
		HAlignmentMode horizontalAlignmentMode = HAlignmentMode::CENTER;

		LayoutMode layoutMode = LayoutMode::FLEX;


		// TODO: if adding a child, hook it up to input manager events (can check if an element is a child using a visitor). likewise, when removing/deleting a child, remove it from listening to events. This would potentially mean a lot of listeners that do nothing, but since we're only adding controls, we can presume its not _too_ wasteful.
		void add(Element* newChild);

		void remove(Element* child);

		void destroySelfAndChildren();

		void accept(Visitor& visitor) override;

		virtual void render(Canvas* canvas) override;

		void setScrollable(bool isScrollable)
		{
			isScrollEventListener = isScrollable;

			// TODO: is this redundant given that we have isScrollEventListener? Maybe not, since we don't want to couple listening to scroll events with being a scroll event listener? But... is there ever a point where a *container* that listens to scroll events should not scoll vertically?
			verticalScroll = isScrollable;

			// TODO: we are overwriting user-set min height if they set scrollable. Is the below even necessary?
			if (isScrollable)
				setMinHeight(0);
			else
				setMinHeightAuto();
		}

		void uiScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY) override
		{
			viewportY += -scrollY * scrollbarScrollInterval;

			int viewportH = h;
			viewportY = GG::MoreMath::clamp(viewportY, 0, logicalHeight - viewportH);
		}
	};
}
