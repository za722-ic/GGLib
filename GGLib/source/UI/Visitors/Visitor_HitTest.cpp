#include "GG/UI/UI.h"
#include "GG/UI/Visitors/Visitor.h"

GG::Visitor_HitTest::Visitor_HitTest(int mouseX, int mouseY) : mouseX(mouseX), mouseY(mouseY)
{}

void GG::Visitor_HitTest::visitForLabel(Label* label)
{
	visitForControl(label);
}

void GG::Visitor_HitTest::visitForControl(Control* control)
{
	if (!isMouseInElement(control)) return;

	hitElement = control;

	if (control->isScrollEventListener) hitScrollListener = control;
}

void GG::Visitor_HitTest::visitForFlexContainer(Container* container)
{
	hitTestContainer(container);
}

void GG::Visitor_HitTest::visitForAbsoluteContainer(Container* container)
{
	hitTestContainer(container);
}

void GG::Visitor_HitTest::hitTestContainer(Container* container)
{
	if (!isMouseInElement(container)) return;

	hitElement = container;

	if (container->isScrollEventListener) hitScrollListener = container;

	for (auto child : container->children)
	{
		child->accept(*this);
	}
}

bool GG::Visitor_HitTest::isMouseInElement(Element* element)
{
	return GG::MoreMath::isPointInRect(mouseX, mouseY, element->screenX, element->screenY, element->w, element->h);
}

