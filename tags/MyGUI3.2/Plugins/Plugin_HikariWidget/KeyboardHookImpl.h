#ifndef __KeyboardHookImpl_H__
#define __KeyboardHookImpl_H__

#include "KeyboardHook.h"
#include "HikariWidget.h"

class HookListenerImpl :
	public HookListener
{
public:
	virtual void handleKeyMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		MyGUI::Widget* widget = MyGUI::InputManager::getInstance().getKeyFocusWidget();
		if (widget)
		{
			Hikari::HikariWidget* hwidget = widget->castType<Hikari::HikariWidget>(false);
			if (hwidget)
				hwidget->getControl()->handleKeyEvent(msg, wParam, lParam);
		}
	}
};

#endif
