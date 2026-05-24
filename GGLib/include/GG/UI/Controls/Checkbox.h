#pragma once

#include <functional>

#include "GG/UI/Controls/Button.h"

// TODO use width
// TODO don't inherit button
class Checkbox : public Button
{
public:
	bool isChecked = false;
	bool isMouseOver = false;

public:
	Checkbox();

	void render(Canvas* canvas) override;
};
