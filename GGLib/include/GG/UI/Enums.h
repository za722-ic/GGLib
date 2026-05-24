#pragma once

namespace GG
{
	enum class Axis
	{
		HORIZONTAL,
		VERTICAL
	};

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

	enum class LayoutMode
	{
		FLEX,
		ABSOLUTE
	};
}
