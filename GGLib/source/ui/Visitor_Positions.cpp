#include "Visitor.h"
#include "Container.h"
#include "Control.h"

void Visitor_Positions::visitForControl(Control* control)
{
	control->screenX = control->x + offsetX;
	control->screenY = control->y + offsetY;
}

void Visitor_Positions::visitForAbsoluteContainer(Container* container)
{
	container->screenX = container->x + offsetX;
	container->screenY = container->y + offsetY;

	offsetX = container->screenX;
	offsetY = container->screenY;
	for (auto child : container->children)
	{
		// TODO: maybe the intent here would be better represented by using a stack?
		int oldOffsetX = offsetX;
		int oldOffsetY = offsetY;
		offsetX += child->xAbs;
		offsetY += child->yAbs;
		child->accept(*this);
		offsetX = oldOffsetX;
		offsetY = oldOffsetY;
	}
}

void Visitor_Positions::visitForFlexContainer(Container* container)
{
	container->screenX = container->x + offsetX;
	container->screenY = container->y + offsetY;

	// TODO refactor alignment calculations
	// calculate alignment offset ALONG the layout axis
	int inlineAlignmentOffsetX = 0;
	int inlineAlignmentOffsetY = 0;
	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT && container->horizontalAlignmentMode != HAlignmentMode::LEFT)
	{
		int remainingWidth = container->w;
		remainingWidth -= container->paddingLeft;
		remainingWidth -= container->paddingRight;
		for (auto child : container->children) remainingWidth -= child->w;
		remainingWidth -= (container->children.size() - 1) * container->gap;

		if (container->horizontalAlignmentMode == HAlignmentMode::CENTER)
		{
			inlineAlignmentOffsetX += remainingWidth / 2;
		}
		else if (container->horizontalAlignmentMode == HAlignmentMode::RIGHT)
		{
			inlineAlignmentOffsetX += remainingWidth;
		}
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM && container->verticalAlignmentMode != VAlignmentMode::TOP)
	{
		int remainingHeight = container->h;
		remainingHeight -= container->paddingTop;
		remainingHeight -= container->paddingBottom;
		for (auto child : container->children) remainingHeight -= child->h;
		remainingHeight -= (container->children.size() - 1) * container->gap;

		if (container->verticalAlignmentMode == VAlignmentMode::CENTER)
		{
			inlineAlignmentOffsetY += remainingHeight / 2;
		}
		else if (container->verticalAlignmentMode == VAlignmentMode::BOTTOM)
		{
			inlineAlignmentOffsetY += remainingHeight;
		}
	}

	// render everything else
	offsetX = container->screenX + container->paddingLeft + inlineAlignmentOffsetX;
	offsetY = container->screenY + container->paddingTop + inlineAlignmentOffsetY;
	for (auto child : container->children)
	{
		// calculate alignment offset ACROSS the layout axis
		int acrossAlignmentOffsetX = 0;
		int acrossAlignmentOffsetY = 0;
		if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT && container->verticalAlignmentMode != VAlignmentMode::TOP)
		{
			int remainingHeight = container->h;
			remainingHeight -= container->paddingTop;
			remainingHeight -= container->paddingBottom;
			remainingHeight -= child->h;

			if (container->verticalAlignmentMode == VAlignmentMode::CENTER)
			{
				acrossAlignmentOffsetY += remainingHeight / 2;
			}
			else if (container->verticalAlignmentMode == VAlignmentMode::BOTTOM)
			{
				acrossAlignmentOffsetY += remainingHeight;
			}
		}
		else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM && container->horizontalAlignmentMode != HAlignmentMode::LEFT)
		{
			int remainingWidth = container->w;
			remainingWidth -= container->paddingLeft;
			remainingWidth -= container->paddingRight;
			remainingWidth -= child->w;

			if (container->horizontalAlignmentMode == HAlignmentMode::CENTER)
			{
				acrossAlignmentOffsetX += remainingWidth / 2;
			}
			else if (container->horizontalAlignmentMode == HAlignmentMode::RIGHT)
			{
				acrossAlignmentOffsetX += remainingWidth;
			}
		}

		// TODO: maybe the intent here would be better represented by using a stack?
		int oldOffsetX = offsetX;
		int oldOffsetY = offsetY;
		offsetX += acrossAlignmentOffsetX;
		offsetY += acrossAlignmentOffsetY;
		//child->calculatePositions(offsetX + acrossAlignmentOffsetX, offsetY + acrossAlignmentOffsetY);
		child->accept(*this);
		offsetX = oldOffsetX;
		offsetY = oldOffsetY;

		if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
			offsetX += child->w + container->gap;
		else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
			offsetY += child->h + container->gap;
	}
}

