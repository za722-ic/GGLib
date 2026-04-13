#pragma once

#include "Control.h"

class PerlinViewer : public Control
{
public:
public:

	PerlinViewer()
	{
		setColor(0x407848);
		borderColor = { 38, 84, 44, 255 };
		borderThickness = 4;
		shadowThickness = 6;
	}
};