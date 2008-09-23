/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "PanelCell.h"

const float POSITION_CONTROLLER_TIME = 0.5f;

PanelCell::PanelCell() :
	BaseLayout("PanelCell.layout")
{
}

void PanelCell::initialise(MyGUI::WidgetPtr _parent)
{
	loadLayout(_parent);
	mMainWidget->setPosition(0, 0);

	assignWidget(mTextCaption, "text_Caption");
	assignWidget(mButtonMinimize, "button_Minimize");
	assignWidget(mWidgetClient, "widget_Client");

	mButtonMinimize->eventMouseButtonPressed = MyGUI::newDelegate(this, &PanelCell::notfyMouseButtonPressed);

	m_minHeight = mMainWidget->getHeight() - mWidgetClient->getHeight();
	m_maxHeight = mMainWidget->getHeight();
}

void PanelCell::setClientHeight(int _height, bool _smooth)
{
	m_maxHeight = mMainWidget->getHeight() - mWidgetClient->getHeight() + _height;
	if (_smooth) {
		if (!mButtonMinimize->getButtonPressed()) {
			mButtonMinimize->setButtonPressed(true);
			notfyMouseButtonPressed(null, 0, 0, MyGUI::MB_Left);
		}
	}
	else {
		mMainWidget->setSize(mMainWidget->getWidth(), m_maxHeight);
		eventUpdatePanel(this);
	}
}

bool PanelCell::isMinimized()
{
	return mMainWidget->getHeight() == m_minHeight;
}

void PanelCell::setMinimized(bool _minimized)
{
	mButtonMinimize->setButtonPressed(!_minimized);
	notfyMouseButtonPressed(null, 0, 0, MyGUI::MB_Left);
}

void PanelCell::notfyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
{
	if (_id == MyGUI::MB_Left) {
		if (mButtonMinimize->getButtonPressed()) {
			mButtonMinimize->setButtonPressed(false);

			MyGUI::IntSize size(mMainWidget->getWidth(), m_maxHeight);
			MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(size, POSITION_CONTROLLER_TIME, MyGUI::ControllerPosition::Inertional);
			controller->eventUpdateAction = newDelegate(this, &PanelCell::notifyUpdateAction);
			MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);
		}
		else {
			mButtonMinimize->setButtonPressed(true);

			MyGUI::IntSize size(mMainWidget->getWidth(), m_minHeight);
			MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(size, POSITION_CONTROLLER_TIME, MyGUI::ControllerPosition::Inertional);
			controller->eventUpdateAction = newDelegate(this, &PanelCell::notifyUpdateAction);
			MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);
		}
	}
}

void PanelCell::notifyUpdateAction(MyGUI::WidgetPtr _widget)
{
	eventUpdatePanel(this);
}
