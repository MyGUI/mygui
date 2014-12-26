/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#ifndef BASE_PANEL_VIEW_CELL_H_
#define BASE_PANEL_VIEW_CELL_H_

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace wraps
{

	class BasePanelViewCell :
		public BaseLayout
	{
	public:

		BasePanelViewCell(const std::string& _layout, MyGUI::Widget* _parent) :
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

		void setCaption(const MyGUI::UString& _caption)
		{
			if (mTextCaption)
				mTextCaption->setCaption(_caption);
		}

		MyGUI::Widget* getClient()
		{
			return mWidgetClient ? mWidgetClient : mMainWidget;
		}

		MyGUI::Widget* getMainWidget()
		{
			return mMainWidget;
		}

		void setClientHeight(int _height, bool _smooth = true)
		{
			m_minHeight = mMainWidget->getHeight() - getClient()->getHeight();
			m_maxHeight = m_minHeight + _height;
			if (_smooth)
			{
				if (!m_minimized)
				{
					updateMinimized();
				}
			}
			else
			{
				mMainWidget->setSize(mMainWidget->getWidth(), m_maxHeight);
				eventUpdatePanel(this);
			}
		}

		bool isMinimized() const
		{
			return m_minimized;
		}

		virtual void setMinimized(bool _minimized)
		{
			m_minimized = _minimized;
			updateMinimized();
		}

		void setVisible(bool _visible)
		{
			mMainWidget->setVisible(_visible);
		}

		bool getVisible()
		{
			return mMainWidget->getVisible();
		}

		MyGUI::delegates::CDelegate1<BasePanelViewCell*> eventUpdatePanel;

	private:
		void notifyUpdateAction(MyGUI::Widget* _widget, MyGUI::ControllerItem* _controller)
		{
			eventUpdatePanel(this);
		}

		void updateMinimized()
		{
			const float POSITION_CONTROLLER_TIME = 0.5f;
			if (!m_minimized)
			{
				MyGUI::IntSize size(mMainWidget->getWidth(), m_maxHeight);
				MyGUI::ControllerPosition* controller = createControllerPosition(size, POSITION_CONTROLLER_TIME, MyGUI::newDelegate(MyGUI::action::inertionalMoveFunction));
				controller->eventUpdateAction += newDelegate(this, &BasePanelViewCell::notifyUpdateAction);
				MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);
			}
			else
			{
				MyGUI::IntSize size(mMainWidget->getWidth(), m_minHeight);
				MyGUI::ControllerPosition* controller = createControllerPosition(size, POSITION_CONTROLLER_TIME, MyGUI::newDelegate(MyGUI::action::inertionalMoveFunction));
				controller->eventUpdateAction += newDelegate(this, &BasePanelViewCell::notifyUpdateAction);
				MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);
			}
		}

		MyGUI::ControllerPosition* createControllerPosition(const MyGUI::IntSize& _size, float _time, MyGUI::ControllerPosition::FrameAction::IDelegate* _action)
		{
			MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance().createItem(MyGUI::ControllerPosition::getClassTypeName());
			MyGUI::ControllerPosition* controller = item->castType<MyGUI::ControllerPosition>();

			controller->setSize(_size);
			controller->setTime(_time);
			controller->setAction(_action);

			return controller;
		}

	protected:
		MyGUI::TextBox* mTextCaption;
		MyGUI::Widget* mWidgetClient;
		bool m_minimized;

		int m_minHeight;
		int m_maxHeight;
	};

} // namespace wraps

#endif // BASE_PANEL_VIEW_CELL_H_
