#pragma once

#include <vector>
#include <queue>
#include <optional>

#include <SDL.h>

#include "MoreMath.h"
#include "Element.h"

enum class LayoutDirection
{
	TOP_TO_BOTTOM,
	LEFT_TO_RIGHT
};

// TODO: reuse the ones from Canvas, and make it common to both
enum class VAlignmentMode
{
	TOP,
	CENTER,
	BOTTOM
};
enum class HAlignmentMode
{
	LEFT,
	CENTER,
	RIGHT
};

class Container : public Element
{
public: // TODO: protected
	std::vector<Element*> children;

	LayoutDirection layoutDirection;

	VAlignmentMode verticalAlignmentMode;
	HAlignmentMode horizontalAlignmentMode;

	// TODO: if adding a child, hook it up to input manager events (can check if an element is a child using a visitor). likewise, when removing/deleting a child, remove it from listening to events. This would potentially mean a lot of listeners that do nothing, but since we're only adding controls, we can presume its not _too_ wasteful.
	void add(Element* newChild)
	{
		children.push_back(newChild);
		newChild->parent = this;

		if (inputManager != nullptr)
		{
			Visitor_SetInputManager visitor_SetInputManager;
			visitor_SetInputManager.inputManager = inputManager;
			newChild->accept(visitor_SetInputManager);
		}
	}

	void remove(Control* child)
	{
		std::erase(children, child);
	}

	void destroySelfAndChildren()
	{
		Visitor_DestroySelfAndChildren destroyer;

		accept(destroyer);
	}

	void accept(Visitor& visitor) override
	{
		visitor.visitForContainer(this);
	}

	void calculateLayout()
	{
		Visitor_CalculateMinimumSizing visitor_calculateMinimumSizing;
		accept(visitor_calculateMinimumSizing);

		Visitor_Autosize visitor_Autosize;
		accept(visitor_Autosize);

		Visitor_GrowShrink visitor_GrowShrink;
		visitor_GrowShrink.breadthFirstGrowShrink(this);

		Visitor_Positions visitor_Positions;
		accept(visitor_Positions);
	}


	virtual void render(Canvas* canvas) override
	{
		// rener this element
		Element::render(canvas);

		// render its children
		for (auto child : children)
		{
			child->render(canvas);
		}
	}
};
