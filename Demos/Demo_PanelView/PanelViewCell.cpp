/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "PanelViewCell.h"

namespace demo
{

	PanelViewCell::PanelViewCell(MyGUI::WidgetPtr _parent) : BasePanelViewCell("PanelCell.layout", _parent)
	{
		assignWidget(mTextCaption, "text_Caption");
		assignWidget(mButtonMinimize, "button_Minimize");
		assignWidget(mWidgetClient, "widget_Client");

		mTextCaption->eventMouseButtonDoubleClick = MyGUI::newDelegate(this, &PanelViewCell::notifyMouseButtonDoubleClick);
		mButtonMinimize->eventMouseButtonPressed = MyGUI::newDelegate(this, &PanelViewCell::notfyMouseButtonPressed);
	}

	void PanelViewCell::notfyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MB_Left) {
			setMinimized( ! isMinimized() );
			mButtonMinimize->setButtonPressed(isMinimized());
		}
	}

	void PanelViewCell::notifyMouseButtonDoubleClick(MyGUI::WidgetPtr _sender)
	{
		setMinimized( ! isMinimized() );
		mButtonMinimize->setButtonPressed(isMinimized());
	}

} // namespace demo
