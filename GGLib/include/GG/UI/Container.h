#pragma once

#include <vector>
#include <queue>
#include <optional>

#include <SDL3/SDL.h>

#include "GG/MoreMath/MoreMath.h"
#include "GG/UI/Element.h"

namespace GG
{
	class Container : public Element, public ScrollEventListener
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

		LayoutDirection layoutDirection;

		VAlignmentMode verticalAlignmentMode = VAlignmentMode::CENTER;
		HAlignmentMode horizontalAlignmentMode = HAlignmentMode::CENTER;

		LayoutMode layoutMode = LayoutMode::FLEX;


		// TODO: if adding a child, hook it up to input manager events (can check if an element is a child using a visitor). likewise, when removing/deleting a child, remove it from listening to events. This would potentially mean a lot of listeners that do nothing, but since we're only adding controls, we can presume its not _too_ wasteful.
		void add(Element* newChild);

		void remove(Element* child);

		void destroySelfAndChildren();

		void accept(Visitor& visitor) override;

		void calculateLayout();

		virtual void render(Canvas* canvas) override;

		virtual void onScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY) override;
	};
}
