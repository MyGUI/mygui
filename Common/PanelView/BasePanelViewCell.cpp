/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "PanelView/BasePanelViewCell.h"

namespace wraps
{

	const float POSITION_CONTROLLER_TIME = 0.5f;

	BasePanelViewCell::BasePanelViewCell(const std::string & _layout, MyGUI::WidgetPtr _parent) :
		BaseLayout2(_layout, _parent),
		mTextCaption(null),
		mWidgetClient(null),
		m_minimized(false)
	{
		mMainWidget->setPosition(0, 0);

		m_minHeight = mMainWidget->getHeight() - getClient()->getHeight();
		m_maxHeight = mMainWidget->getHeight();
	}

	BasePanelViewCell::~BasePanelViewCell()
	{
	}

	void BasePanelViewCell::setClientHeight(int _height, bool _smooth)
	{
		m_minHeight = mMainWidget->getHeight() - getClient()->getHeight();
		m_maxHeight = m_minHeight + _height;
		if (_smooth) {
			if (!m_minimized) {
				updateMinimized();
			}
		}
		else {
			mMainWidget->setSize(mMainWidget->getWidth(), m_maxHeight);
			eventUpdatePanel(this);
		}
	}

	bool BasePanelViewCell::isMinimized()
	{
		return m_minimized;
	}

	void BasePanelViewCell::setMinimized(bool _minimized)
	{
		m_minimized = _minimized;
		updateMinimized();
	}

	void BasePanelViewCell::updateMinimized()
	{
		if (!m_minimized) {
			MyGUI::IntSize size(mMainWidget->getWidth(), m_maxHeight);
			MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(size, POSITION_CONTROLLER_TIME, MyGUI::newDelegate(MyGUI::action::inertionalMoveFunction));
			controller->eventUpdateAction = newDelegate(this, &BasePanelViewCell::notifyUpdateAction);
			MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);

		}
		else {
			MyGUI::IntSize size(mMainWidget->getWidth(), m_minHeight);
			MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(size, POSITION_CONTROLLER_TIME, MyGUI::newDelegate(MyGUI::action::inertionalMoveFunction));
			MyGUI::ControllerPosition::Accelerated + 1;
			controller->eventUpdateAction = newDelegate(this, &BasePanelViewCell::notifyUpdateAction);
			MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);

		}
	}

	void BasePanelViewCell::notifyUpdateAction(MyGUI::WidgetPtr _widget)
	{
		eventUpdatePanel(this);
	}

} // namespace wraps
