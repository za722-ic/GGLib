#pragma once

#include <vector>
#include <queue>
#include <optional>

#include <SDL3/SDL.h>

#include "GG/MoreMath/MoreMath.h"
#include "GG/UI/Element.h"

class Container : public Element
{
public: // TODO: protected
	std::vector<Element*> children;

	LayoutDirection layoutDirection;

	VAlignmentMode verticalAlignmentMode;
	HAlignmentMode horizontalAlignmentMode;

	LayoutMode layoutMode = LayoutMode::FLEX;


	// TODO: if adding a child, hook it up to input manager events (can check if an element is a child using a visitor). likewise, when removing/deleting a child, remove it from listening to events. This would potentially mean a lot of listeners that do nothing, but since we're only adding controls, we can presume its not _too_ wasteful.
	void add(Element* newChild);

	void remove(Control* child);

	void destroySelfAndChildren();

	void accept(Visitor& visitor) override;

	void calculateLayout();

	virtual void render(Canvas* canvas) override;
};
