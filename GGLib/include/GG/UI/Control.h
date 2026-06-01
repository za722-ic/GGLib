#pragma once

#include "GG/UI/Element.h"

namespace GG
{
	class Control : public Element
	{
	public:
		Control();

	public:
		virtual void accept(Visitor& visitor) override;
	};
}
