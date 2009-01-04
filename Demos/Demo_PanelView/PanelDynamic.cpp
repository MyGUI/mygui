/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "PanelDynamic.h"

namespace demo
{

	PanelDynamic::PanelDynamic() :
		PanelBase()
	{
	}

	void PanelDynamic::initialiseCell(wraps::PanelCell * _cell)
	{
		PanelBase::initialiseCell(_cell);

		mPanelCell->setCaption("Dynamic panel");

		const int height = 24;
		const int height_step = 26;
		const int width = 55;
		const int width_step = 3;
		int height_current = 0;
		for (size_t pos=0; pos<16; ++pos) {
			MyGUI::StaticTextPtr text = mWidgetClient->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(width_step, height_current, width, height), MyGUI::Align::Left | MyGUI::Align::Top);
			text->setTextAlign(MyGUI::Align::Right | MyGUI::Align::VCenter);
			text->setCaption(MyGUI::utility::toString("line ", pos + 1, " : "));
			mItemsText.push_back(text);

			MyGUI::EditPtr edit = mWidgetClient->createWidget<MyGUI::Edit>("Edit", MyGUI::IntCoord(width_step + width_step + width, height_current, mWidgetClient->getWidth() - (width_step + width_step + width_step + width), height), MyGUI::Align::HStretch | MyGUI::Align::Top);
			mItemsEdit.push_back(edit);

			height_current += height_step;
		}

		mPanelCell->setClientHeight(height_current, false);
	}

	void PanelDynamic::shutdownCell()
	{
		mItemsText.clear();
		mItemsEdit.clear();
		PanelBase::shutdownCell();
	}

	void PanelDynamic::setVisibleCount(size_t _count)
	{
		const int height_step = 26;
		int height_current = 0;
		for (size_t pos=0; pos<16; ++pos) {
			if (pos < _count) {
				mItemsText[pos]->show();
				mItemsEdit[pos]->show();
				height_current += height_step;
			}
			else {
				mItemsText[pos]->hide();
				mItemsEdit[pos]->hide();
			}
		}
		mPanelCell->setClientHeight(height_current, true);
	}

} // namespace demo
