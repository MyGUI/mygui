/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/
#ifndef __AWESOMIUM_WIDGET_FACTORY_H__
#define __AWESOMIUM_WIDGET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "WebCore.h"
#include "KeyboardHook.h"

namespace Awesomium
{

	class AwesomiumWidgetFactory : public HookListener
	{
	public:
		AwesomiumWidgetFactory();
		virtual ~AwesomiumWidgetFactory();

		static Awesomium::WebCore* getCore() { return msCore; }

	private:
		void notifyFrameStart(float _time);
		virtual void handleKeyMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		static Awesomium::WebCore* msCore;
		KeyboardHook* mKeyboardHook;
	};

} // namespace Awesomium

#endif // __AWESOMIUM_WIDGET_FACTORY_H__
