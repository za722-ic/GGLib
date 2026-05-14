#pragma once

#include <algorithm>
#include <vector>
#include <cassert>

#include "InputManager.h"

// TODO: use floats in layouting?
// TODO: rewrite, and use anchors/docking/other winforms things

// forward declare classes
class Container;
class Control;
class Element;

class Visitor
{
public:
	virtual void visitForControl(Control* control) = 0;

	void visitForContainer(Container* container);

	virtual void visitForFlexContainer(Container* container) = 0;
	virtual void visitForAbsoluteContainer(Container* container) = 0;

	// see: virtual destructor and default TODO
	virtual ~Visitor() = default;
};

class Visitor_SetInputManager : public Visitor
{
public:
	InputManager* inputManager = nullptr;

	void visitForControl(Control* control) override;

	void visitForFlexContainer(Container* container) override;

	void visitForAbsoluteContainer(Container* container) override;

};

class Visitor_GetChildren : public Visitor
{
public:
	std::vector<Element*> children;

	void visitForControl(Control* control) override;

	void visitForFlexContainer(Container* container) override;

	void visitForAbsoluteContainer(Container* container) override;
};

class Visitor_DestroySelfAndChildren : public Visitor
{
public:
	void visitForControl(Control* control) override;

	void visitForFlexContainer(Container* container) override;

	void visitForAbsoluteContainer(Container* container) override;
};

class Visitor_CalculateMinimumSizing: public Visitor
{
public:
	// nothing to calculate for control minimum size
	void visitForControl(Control* control) override;

	// calculates the minimum size of the container
	// TODO this is almost copy paste of fit sizing calculations except im using min w/h instead of actual w/h
	void visitForFlexContainer(Container* container) override;

	void visitForAbsoluteContainer(Container* container) override;
};

class Visitor_Autosize : public Visitor
{
public:
	void visitForControl(Control* control) override;

	// if the container is set to autosize, then resize it accordingly
	void visitForFlexContainer(Container* container) override;

	void visitForAbsoluteContainer(Container* container) override;
};

class Visitor_GrowShrink: public Visitor
{
public:
	void visitForControl(Control* control) override;

	void visitForFlexContainer(Container* container) override;

	void visitForAbsoluteContainer(Container* container) override;

	void breadthFirstGrowShrink(Container* root);

private:
	// TODO: there is some duplicate code in the two grow functions (e.g. find list of growwable items (but be careful as that part is NOT EXACTLY THE SAME)) --> refactor
	void resizeChildrenAlongMainAxis(Container* container);
	void resizeChildrenAlongCrossAxis(Container* container);
};

class Visitor_Positions: public Visitor
{
private:
	int offsetX = 0;
	int offsetY = 0;
public:
	void visitForControl(Control* control) override;

	void visitForFlexContainer(Container* container) override;

	void visitForAbsoluteContainer(Container* container) override;
};
