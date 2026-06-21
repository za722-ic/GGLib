#pragma once

#include "GG/UI/Container.h"
#include "GG/UI/RootContainer.h"
#include "GG/UI/Controls/Button.h"
#include "GG/UI/Control.h"

namespace GG
{
	/* 
		TODO: this does not receive input events under the updated top-down input scheme
		since it still needs to use onMouseEvent

		since when the combobiox is expanded, the mouse needs to interact with locations outside
		the explicit bounds of the combobox.
		to solve this, maybe make it so that we can distinguish between the hitbox bounds of a control,
		its layout bounds, and its visual bounds

		so e.g., for layout, this is just a small rectangle even when expanded
		but when expanded the visual bounds and hitbox update to be taller than the layout bounds
	*/
	class Combobox : public Control
	{
	protected:

		SDL_Color backColorOnHover = { 200,200,200, 255 };
		SDL_Color backColorOnMouseDown = { 180,180,180,255 };
		SDL_Color backColor = { 230,230,230,255 };
		SDL_Color foreColor = { 0, 0, 0, 255 };

		std::string text = "Select option";

		std::vector<std::string> options = { text };

		int selectedOptionIndex = -1;

		GG::RootContainer* root = nullptr;

		GG::Container* pnlDropDownOptions = nullptr;

	public:
		Combobox(std::string text, GG::RootContainer* root);

		// bounds
		void setBounds(int _x, int _y, int _w, int _h);

		// color
		void setForeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		void setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

		// text
		void setText(std::string _text);
		std::string getText() const;

		// options
		void addOption(std::string optionText);
		void clearOptions();
		std::vector<std::string> getOptions();
		std::string getSelectedOption();
		void setSelectedOptionIndex(size_t index)
		{
			selectedOptionIndex = index;
			setText(options.at(index));
		}


		void render(Canvas* canvas) override;

		bool isMouseDown = false;
		bool isInBounds = false;
		void onMouseUp(int mouseX, int mouseY) override
		{
			// TODO: handle click off combobox
			//if (pnlDropDownOptions != nullptr)
			//{
			//	onMouseClickOff(-1, -1);
			//	return;
			//}
			isMouseDown = false;

			pnlDropDownOptions = new GG::Container;
			pnlDropDownOptions->layoutDirection = GG::LayoutDirection::TOP_TO_BOTTOM;
			pnlDropDownOptions->isScrollEventListener = true;
			pnlDropDownOptions->verticalScroll = true;
			pnlDropDownOptions->verticalAutosize = true;
			pnlDropDownOptions->verticalAlignmentMode = GG::VAlignmentMode::TOP;
			pnlDropDownOptions->setMaxHeight(100);
			pnlDropDownOptions->setWidthAbs(w);
			pnlDropDownOptions->x = screenX;
			pnlDropDownOptions->y = screenY;
			for (size_t i = 0; i < getOptions().size(); i++)
			{
				GG::Button* btnOption = new GG::Button(getOptions().at(i));
				btnOption->setOnClick([this, i]() {
					this->setSelectedOptionIndex(i);

					this->root->overlay = nullptr;
					this->root->hitElement = nullptr;

					auto tmp = this->pnlDropDownOptions;
					this->pnlDropDownOptions = nullptr;
					tmp->destroySelfAndChildren();

				});
				btnOption->setHeightAbs(30);
				pnlDropDownOptions->add(btnOption);
			}
			root->overlay = pnlDropDownOptions;
		}

		void onMouseClickOff(int mouseX, int mouseY) override
		{
			// TODO --> on click off, minimise combobox
			//if (GG::MoreMath::isPointInRect(mouseX, mouseY, pnlDropDownOptions->screenX, pnlDropDownOptions->screenY, pnlDropDownOptions->w, pnlDropDownOptions->h)) return;
			//if (pnlDropDownOptions == nullptr) return;

			//this->root->overlay = nullptr;
			//this->root->hitElement = nullptr;

			//auto tmp = this->pnlDropDownOptions;
			//this->pnlDropDownOptions = nullptr;
			//tmp->destroySelfAndChildren();
		}

		void onMouseDown(int mouseX, int mouseY)
		{
			isMouseDown = true;
		}

		void onMouseEnter(int mouseX, int mouseY)
		{
			isInBounds = true;
		}

		void onMouseExit(int mouseX, int mouseY)
		{
			isInBounds = false;
		}
	};

}