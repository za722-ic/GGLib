#include "GG/UI/Container.h"

void GG::Container::add(Element* newChild)
{
	children.push_back(newChild);
	newChild->parent = this;
}

void GG::Container::remove(Element* child)
{
	child->parent = nullptr;
	std::erase(children, child);
}

void GG::Container::removeAllChildren()
{
	for (auto child : children)
		remove(child);
}


void GG::Container::destroySelfAndChildren()
{
	Visitor_DestroySelfAndChildren destroyer;

	accept(destroyer);
}

void GG::Container::accept(Visitor& visitor)
{
	visitor.visitForContainer(this);
}

void GG::Container::render(Canvas* canvas)
{
	// render this element
	Element::render(canvas);



	//// scrollable stuff
	//if (verticalScroll)
	//{
	//	int viewportH = h;
	//	int maxViewportY = logicalHeight - viewportH;

	//	render_scrollbarH = GG::MoreMath::map(viewportH / logicalHeight, 0, 1, render_scrollbarMinHeight, viewportH - 2 * render_scrollbarPaddingVertical);
	//	render_scrollbarY = GG::MoreMath::map(viewportY, 0, maxViewportY, render_scrollbarPaddingVertical, h - render_scrollbarH - render_scrollbarPaddingVertical);

	//	canvas->setColor(150);
	//	canvas->fillRect(screenX + w - render_scrollbarPaddingHorizontal,
	//		screenY + render_scrollbarY,
	//		render_scrollbarWidth,
	//		render_scrollbarH
	//	);
	//}

	// render its children, clipping them to the container bounds
	int forcedShadowPadding = 0; // TODO: this is here because clipping doesn't take into account drop-shadow/borders, so we force a padding value to not clip them --> this is not correct. It breaks things like user interaction (as hittesting uses rects without this forced padding) and just generally violates pixel-correctness.
	canvas->pushClipRect({ screenX - forcedShadowPadding, screenY - forcedShadowPadding, w + 2*forcedShadowPadding, h + 2*forcedShadowPadding});
	for (auto child : children)
	{
		child->render(canvas);
	}
	canvas->popClipRect();
}
