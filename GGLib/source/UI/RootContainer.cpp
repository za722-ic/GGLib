#include "GG/UI/RootContainer.h"

void GG::RootContainer::calculateLayout(int screenX, int screenY, int screenW, int screenH)
{
	// set bounds
	x = screenX;
	y = screenY;
	setWidthAbs(screenW);
	setHeightAbs(screenH);


	// calculate laout using visitor pipeline
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
}

void GG::RootContainer::setInputManager(InputManager* inputManager)
{
	unsubscribeFromInputs();
	this->inputManager = inputManager;
	subscribeToInputs();
}

void GG::RootContainer::onScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY)
{
	// there is no active scrollable, so nothing to pass the event to
	if (activeScrollListener == nullptr) return;

	// check just in case the scrollable was changed to no longer be scrollable, before any new hit testing took place
	if (!activeScrollListener->isScrollEventListener)
	{
		activeScrollListener = nullptr;
		return;
	}

	// pass scroll event to the active scroll listener
	activeScrollListener->uiScrollEvent(mouseX, mouseY, scrollX, scrollY);
}

void GG::RootContainer::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
	// perform hit testing
	Visitor_HitTest visitor_hitTest(mouseX, mouseY);
	accept(visitor_hitTest);

	// get hit element and scrollable
	activeScrollListener = visitor_hitTest.hitScrollListener;

	// if hit element has changed since last mouse event, then mouse has exited that element and entered a new element (if any)
	if (visitor_hitTest.hitElement != this->hitElement)
	{
		// we've exited old element (if there was one)
		if (hitElement != nullptr && hitElement->isMouseEventListener)
			hitElement->onMouseExit(mouseX, mouseY);

		// update to new hitElement
		hitElement = visitor_hitTest.hitElement;

		// we've entered new hit element (if there is one)
		if (hitElement != nullptr && hitElement->isMouseEventListener)
			hitElement->onMouseEnter(mouseX, mouseY);

		return;
	}

	if (hitElement == nullptr) return;

	// we have not changed the hitElement, so the mouse has done something in the element's bounds
	if (state == MouseState::MOUSE_UP && mouseEventType == LEFT_MOUSE_DOWN)
	{
		state = MouseState::MOUSE_DOWN;

		// if mouse event listener, trigger mouse down event
		if (hitElement->isMouseEventListener) 
			hitElement->onMouseDown(mouseX, mouseY);

		// if draggable, remember it (drags persist only when the mouse is *released*, even if the mouse exits)
		if (activeDragListener == nullptr && hitElement->isDragListener)
			activeDragListener = hitElement;

		// deselect old activeTextInput if we clicked off of it
		if (hitElement != activeTextInputListener && activeTextInputListener != nullptr)
			activeTextInputListener->onMouseClickOff(); 

		// if selected a (different) textinput, select it
		if (hitElement->isTextInputListener)
			activeTextInputListener = hitElement;
		else
			activeTextInputListener = nullptr;
	}
	else if (state == MouseState::MOUSE_DOWN && mouseEventType == LEFT_MOUSE_UP)
	{
		state = MouseState::MOUSE_UP;

		// if mouse event listener, trigger mouse up event
		if (hitElement->isMouseEventListener) hitElement->onMouseUp(mouseX, mouseY);

		// end any dragging
		activeDragListener = nullptr;
	}
	else if (state == MouseState::MOUSE_DOWN && mouseEventType == MOUSE_MOVE)
	{
		if (activeDragListener != nullptr) activeDragListener->onMouseDrag(mouseX, mouseY);
		state = MouseState::MOUSE_DOWN;
	}
}

void GG::RootContainer::onTextInputEvent(const std::string text)
{
	/*
		in onMouseEvent
			if the mouse was clicked AND the clicked element is a text input, set it to the active text input
			if the mouse was clicked AND the clicked element is NOT a text input, clear active text input

		in onTextInputEvent (this function)
			pass text input event to the active text input listener, if there is one
	*/
	if (activeTextInputListener != nullptr) activeTextInputListener->onTextInput(text);
}

void GG::RootContainer::onKeyEvent(KeyEventType keyEventType, SDL_Keycode key)
{
	// pass key events to active text input
	if (activeTextInputListener != nullptr) activeTextInputListener->uiKeyEvent(keyEventType, key);
}

void GG::RootContainer::subscribeToInputs()
{
	if (inputManager == nullptr) return;

	inputManager->addKeyEventListener(this);
	inputManager->addMouseEventListener(this);
	inputManager->addScrollEventListener(this);
	inputManager->addTextInputEventListener(this);
}

void GG::RootContainer::unsubscribeFromInputs()
{
	if (inputManager == nullptr) return;

	inputManager->removeKeyEventListener(this);
	inputManager->removeMouseEventListener(this);
	inputManager->removeScrollEventListener(this);
	inputManager->removeTextInputEventListener(this);
}
