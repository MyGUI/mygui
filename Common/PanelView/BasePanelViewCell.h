/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __BASE_PANEL_VIEW_CELL_H__
#define __BASE_PANEL_VIEW_CELL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace wraps
{

	class BasePanelViewCell : public BaseLayout
	{
	public:

		BasePanelViewCell(const std::string & _layout, MyGUI::WidgetPtr _parent) :
			BaseLayout(_layout, _parent),
			mTextCaption(nullptr),
			mWidgetClient(nullptr),
			m_minimized(false)
		{
			mMainWidget->setPosition(0, 0);

			m_minHeight = mMainWidget->getHeight() - getClient()->getHeight();
			m_maxHeight = mMainWidget->getHeight();
		}

		virtual ~BasePanelViewCell()
		{
		}

		void setCaption(const Ogre::UTFString & _caption)
		{
			if (mTextCaption) mTextCaption->setCaption(_caption);
		}

		MyGUI::WidgetPtr getClient()
		{
			return mWidgetClient ? mWidgetClient : mMainWidget;
		}

		MyGUI::WidgetPtr getMainWidget()
		{
			return mMainWidget;
		}

		void setClientHeight(int _height, bool _smooth = true)
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

		bool isMinimized()
		{
			return m_minimized;
		}

		virtual void setMinimized(bool _minimized)
		{
			m_minimized = _minimized;
			updateMinimized();
		}

		void setVisible(bool _visible) { mMainWidget->setVisible(_visible); }
		bool isVisible() { return mMainWidget->isVisible(); }

		MyGUI::delegates::CDelegate1<BasePanelViewCell*> eventUpdatePanel;

	private:
		void notifyUpdateAction(MyGUI::WidgetPtr _widget)
		{
			eventUpdatePanel(this);
		}

		void updateMinimized()
		{
			const float POSITION_CONTROLLER_TIME = 0.5f;
			if (!m_minimized) {
				MyGUI::IntSize size(mMainWidget->getWidth(), m_maxHeight);
				MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(size, POSITION_CONTROLLER_TIME, MyGUI::newDelegate(MyGUI::action::inertionalMoveFunction));
				controller->eventUpdateAction = newDelegate(this, &BasePanelViewCell::notifyUpdateAction);
				MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);

			}
			else {
				MyGUI::IntSize size(mMainWidget->getWidth(), m_minHeight);
				MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(size, POSITION_CONTROLLER_TIME, MyGUI::newDelegate(MyGUI::action::inertionalMoveFunction));
				controller->eventUpdateAction = newDelegate(this, &BasePanelViewCell::notifyUpdateAction);
				MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);

			}
		}

	protected:
		MyGUI::StaticTextPtr mTextCaption;
		MyGUI::WidgetPtr mWidgetClient;
		bool m_minimized;

		int m_minHeight;
		int m_maxHeight;
	};

} // namespace wraps

#endif // __BASE_PANEL_VIEW_CELL_H__
