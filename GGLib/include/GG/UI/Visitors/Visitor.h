#pragma once

#include <algorithm>
#include <vector>
#include <cassert>

#include "GG/Input/InputManager.h"

namespace GG
{
	// TODO: use floats in layouting?
	// TODO: rewrite, and use anchors/docking/other winforms things

	// forward declare classes
	class Container;
	class Control;
	class Element;
	class Label;

	class Visitor
	{
	public:
		virtual void visitForControl(Control* control) = 0;
		virtual void visitForLabel(Label* label) = 0;

		void visitForContainer(Container* container);
		virtual void visitForFlexContainer(Container* container) = 0;
		virtual void visitForAbsoluteContainer(Container* container) = 0;

		// see: virtual destructor and default TODO
		virtual ~Visitor() = default;
	};

	class Visitor_HitTest: public Visitor
	{
	public:
		const int mouseX = -1;
		const int mouseY = -1;

		Element* hitElement = nullptr;
		Element* hitScrollListener = nullptr;

	public:
		Visitor_HitTest(int mouseX, int mouseY);

		void visitForLabel(Label* label) override;
		void visitForControl(Control* control) override;

		void visitForFlexContainer(Container* container) override;

		void visitForAbsoluteContainer(Container* container) override;

	private:
		void hitTestContainer(Container* container);

		bool isMouseInElement(Element* element);
	};

	class Visitor_GetChildren : public Visitor
	{
	public:
		std::vector<Element*> children;

		void visitForControl(Control* control) override;
		void visitForLabel(Label* label) override;

		void visitForFlexContainer(Container* container) override;
		void visitForAbsoluteContainer(Container* container) override;
	};

	class Visitor_DestroySelfAndChildren : public Visitor
	{
	public:
		void visitForControl(Control* control) override;
		void visitForLabel(Label* label) override;

		void visitForFlexContainer(Container* container) override;

		void visitForAbsoluteContainer(Container* container) override;
	};

	class Visitor_CalculateMinimumSizing : public Visitor
	{
	public:
		// nothing to calculate for control minimum size
		void visitForControl(Control* control) override;
		void visitForLabel(Label* label) override;

		// calculates the minimum size of the container
		// TODO this is almost copy paste of fit sizing calculations except im using min w/h instead of actual w/h
		void visitForFlexContainer(Container* container) override;

		void visitForAbsoluteContainer(Container* container) override;
	};

	class Visitor_Autosize_Horizontal : public Visitor
	{
	public:
		void visitForControl(Control* control) override;
		void visitForLabel(Label* label) override;

		// if the container is set to autosize, then resize it accordingly
		void visitForFlexContainer(Container* container) override;

		void visitForAbsoluteContainer(Container* container) override;
	};
	class Visitor_Autosize_Vertical : public Visitor
	{
	public:
		void visitForControl(Control* control) override;
		void visitForLabel(Label* label) override;

		// if the container is set to autosize, then resize it accordingly
		void visitForFlexContainer(Container* container) override;

		void visitForAbsoluteContainer(Container* container) override;
	};

	class Visitor_WrapText : public Visitor
	{
	public:
		void visitForControl(Control* control) override;
		void visitForLabel(Label* label) override;

		void visitForFlexContainer(Container* container) override;

		void visitForAbsoluteContainer(Container* container) override;
	};

	class Visitor_GrowShrink_Horizontal : public Visitor
	{
	public:
		void visitForControl(Control* control) override;
		void visitForLabel(Label* label) override;

		void visitForFlexContainer(Container* container) override;

		void visitForAbsoluteContainer(Container* container) override;

		void breadthFirstGrowShrink(Container* root);

	private:
		// TODO: there is some duplicate code in the two grow functions (e.g. find list of growwable items (but be careful as that part is NOT EXACTLY THE SAME)) --> refactor
		void resizeChildrenAlongMainAxis(Container* container);
		void resizeChildrenAlongCrossAxis(Container* container);
	};
	class Visitor_GrowShrink_Vertical : public Visitor
	{
	public:
		void visitForControl(Control* control) override;
		void visitForLabel(Label* label) override;

		void visitForFlexContainer(Container* container) override;

		void visitForAbsoluteContainer(Container* container) override;

		void breadthFirstGrowShrink(Container* root);

	private:
		// TODO: there is some duplicate code in the two grow functions (e.g. find list of growwable items (but be careful as that part is NOT EXACTLY THE SAME)) --> refactor
		void resizeChildrenAlongMainAxis(Container* container);
		void resizeChildrenAlongCrossAxis(Container* container);
	};

	class Visitor_Positions : public Visitor
	{
	private:
		int offsetX = 0;
		int offsetY = 0;

	public:
		void visitForLabel(Label* label) override;
		void visitForControl(Control* control) override;

		void visitForFlexContainer(Container* container) override;

		void visitForAbsoluteContainer(Container* container) override;
	};
}
