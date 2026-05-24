#include "GG/UI/Container.h"

void GG::Container::add(Element* newChild)
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

void GG::Container::remove(Element* child)
{
	std::erase(children, child);
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

void GG::Container::calculateLayout()
{
	Visitor_CalculateMinimumSizing visitor_calculateMinimumSizing;
	accept(visitor_calculateMinimumSizing);




	Visitor_Autosize_Horizontal visitor_Autosize_Horizontal;
	accept(visitor_Autosize_Horizontal);

	Visitor_GrowShrink_Horizontal visitor_GrowShrink_Horizontal;
	visitor_GrowShrink_Horizontal.breadthFirstGrowShrink(this);



	Visitor_WrapText visitor_WrapText;
	accept(visitor_WrapText);



	Visitor_Autosize_Vertical visitor_Autosize_Vertical;
	accept(visitor_Autosize_Vertical);

	Visitor_GrowShrink_Vertical visitor_GrowShrink_Vertical;
	visitor_GrowShrink_Vertical.breadthFirstGrowShrink(this);




	Visitor_Positions visitor_Positions;
	accept(visitor_Positions);



	//Visitor_CalculateMinimumSizing visitor_calculateMinimumSizing;
	//accept(visitor_calculateMinimumSizing);

	//Visitor_Autosize visitor_Autosize;
	//accept(visitor_Autosize);

	//Visitor_GrowShrink visitor_GrowShrink;
	//visitor_GrowShrink.breadthFirstGrowShrink(this);

	//Visitor_Positions visitor_Positions;
	//accept(visitor_Positions);
}

void GG::Container::render(Canvas* canvas)
{
	// rener this element
	Element::render(canvas);

	// render its children
	for (auto child : children)
	{
		child->render(canvas);
	}
}

void GG::Container::onScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY)
{}
