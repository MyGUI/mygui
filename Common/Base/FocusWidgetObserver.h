/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef FOCUS_WIDGET_OBSERVER_H_
#define FOCUS_WIDGET_OBSERVER_H_

#include <MyGUI.h>

namespace diagnostic
{

	// Tracks the geometry and lifetime of a widget highlighted by a focus overlay.
	class FocusWidgetObserver
	{
	public:
		FocusWidgetObserver() = default;
		FocusWidgetObserver(const FocusWidgetObserver&) = delete;
		FocusWidgetObserver& operator=(const FocusWidgetObserver&) = delete;

		~FocusWidgetObserver()
		{
			unsubscribe();
		}

		MyGUI::Widget* getWidget() const
		{
			return mWidget;
		}

		void setWidget(MyGUI::Widget* _widget)
		{
			if (mWidget == _widget)
				return;
			unsubscribe();
			mWidget = _widget;
			if (mWidget != nullptr)
			{
				mWidget->eventChangeAbsoluteCoord += MyGUI::newDelegate(this, &FocusWidgetObserver::notifyCoord);
				mWidget->eventWidgetDestroyed += MyGUI::newDelegate(this, &FocusWidgetObserver::notifyDestroyed);
			}
			eventChange();
		}

		MyGUI::delegates::MultiDelegate<> eventChange;

	private:
		void unsubscribe()
		{
			if (mWidget != nullptr)
			{
				mWidget->eventChangeAbsoluteCoord -= MyGUI::newDelegate(this, &FocusWidgetObserver::notifyCoord);
				mWidget->eventWidgetDestroyed -= MyGUI::newDelegate(this, &FocusWidgetObserver::notifyDestroyed);
			}
		}

		void notifyCoord(MyGUI::Widget* _sender)
		{
			eventChange();
		}

		void notifyDestroyed(MyGUI::Widget* _sender)
		{
			setWidget(nullptr);
		}

		MyGUI::Widget* mWidget{nullptr};
	};

}

#endif
