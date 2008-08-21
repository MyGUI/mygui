/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "Panel.h"

const float POSITION_CONTROLLER_TIME = 0.5f;

Panel::Panel() :
	BaseLayout("Panel.layout")
{
}

void Panel::initialise(MyGUI::WidgetPtr _parent)
{
	loadLayout(_parent);
	mMainWidget->setPosition(0, 0);

	assignWidget(mTextCaption, "text_Caption");
	assignWidget(mButtonMinimize, "button_Minimize");
	assignWidget(mWidgetClient, "widget_Client");

	mButtonMinimize->eventMouseButtonPressed = MyGUI::newDelegate(this, &Panel::notfyMouseButtonPressed);

	m_minHeight = mMainWidget->getHeight() - mWidgetClient->getHeight();
	m_maxHeight = mMainWidget->getHeight();
}

/*void Panel::show()
{
	mMainWidget->show();
}

void Panel::hide()
{
	mMainWidget->hide();
}*/

void Panel::notfyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
{
	if (_id == MyGUI::MB_Left) {
		if (mButtonMinimize->getButtonPressed()) {
			mButtonMinimize->setButtonPressed(false);

			MyGUI::IntSize size(mMainWidget->getWidth(), m_maxHeight);
			MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(size, POSITION_CONTROLLER_TIME, MyGUI::ControllerPosition::Inertional);
			controller->eventUpdateAction = newDelegate(this, &Panel::notifyUpdateAction);
			MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);
		}
		else {
			mButtonMinimize->setButtonPressed(true);

			MyGUI::IntSize size(mMainWidget->getWidth(), m_minHeight);
			MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(size, POSITION_CONTROLLER_TIME, MyGUI::ControllerPosition::Inertional);
			controller->eventUpdateAction = newDelegate(this, &Panel::notifyUpdateAction);
			MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);
		}
	}
}

void Panel::notifyUpdateAction(MyGUI::WidgetPtr _widget)
{
	eventUpdatePanel(this);
}
