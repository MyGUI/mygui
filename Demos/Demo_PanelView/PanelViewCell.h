/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef PANEL_VIEW_CELL_H_
#define PANEL_VIEW_CELL_H_

#include "PanelView/BasePanelViewItem.h"

namespace demo
{

	class PanelViewCell : public wraps::BasePanelViewCell
	{
	public:
		PanelViewCell(MyGUI::Widget* _parent) :
			BasePanelViewCell("PanelCell.layout", _parent)
		{
			assignWidget(mTextCaption, "text_Caption");
			assignWidget(mButtonMinimize, "button_Minimize");
			assignWidget(mWidgetClient, "widget_Client");

			mTextCaption->eventMouseButtonDoubleClick +=
				MyGUI::newDelegate(this, &PanelViewCell::notifyMouseButtonDoubleClick);
			mButtonMinimize->eventMouseButtonPressed +=
				MyGUI::newDelegate(this, &PanelViewCell::notfyMouseButtonPressed);
		}

		void setMinimized(bool _minimized) override
		{
			wraps::BasePanelViewCell::setMinimized(_minimized);
			mButtonMinimize->setStateSelected(isMinimized());
		}

	private:
		void notfyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
		{
			if (_id == MyGUI::MouseButton::Left)
			{
				setMinimized(!isMinimized());
			}
		}

		void notifyMouseButtonDoubleClick(MyGUI::Widget* _sender)
		{
			setMinimized(!isMinimized());
		}

	private:
		MyGUI::Button* mButtonMinimize;
	};

} // namespace demo

#endif // PANEL_VIEW_CELL_H_
