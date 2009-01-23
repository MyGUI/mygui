/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __PANEL_VIEW_CELL_H__
#define __PANEL_VIEW_CELL_H__

#include "PanelView/BasePanelViewItem.h"

namespace demo
{

	class PanelViewCell : public wraps::BasePanelViewCell
	{
	public:
		PanelViewCell(MyGUI::WidgetPtr _parent) : BasePanelViewCell("PanelCell.layout", _parent)
		{
			assignWidget(mTextCaption, "text_Caption");
			assignWidget(mButtonMinimize, "button_Minimize");
			assignWidget(mWidgetClient, "widget_Client");

			mTextCaption->eventMouseButtonDoubleClick = MyGUI::newDelegate(this, &PanelViewCell::notifyMouseButtonDoubleClick);
			mButtonMinimize->eventMouseButtonPressed = MyGUI::newDelegate(this, &PanelViewCell::notfyMouseButtonPressed);
		}

		virtual void setMinimized(bool _minimized)
		{
			wraps::BasePanelViewCell::setMinimized(_minimized);
			mButtonMinimize->setButtonPressed(isMinimized());
		}

	private:
		void notfyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
		{
			if (_id == MyGUI::MouseButton::Left) {
				setMinimized( ! isMinimized() );
			}
		}

		void notifyMouseButtonDoubleClick(MyGUI::WidgetPtr _sender)
		{
			setMinimized( ! isMinimized() );
		}

	private:
		MyGUI::ButtonPtr mButtonMinimize;

	};

} // namespace demo

#endif // __PANEL_VIEW_CELL_H__
