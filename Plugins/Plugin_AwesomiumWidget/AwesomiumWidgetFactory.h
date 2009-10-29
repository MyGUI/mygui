/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/
#ifndef __AWESOMIUM_WIDGET_FACTORY_H__
#define __AWESOMIUM_WIDGET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "WebCore.h"
#include "KeyboardHook.h"

namespace Awesomium
{
	namespace factory
	{

		class AwesomiumWidgetFactory : public MyGUI::IWidgetFactory, public HookListener
		{
		public:
			AwesomiumWidgetFactory();
			virtual ~AwesomiumWidgetFactory();

			const std::string& getTypeName();
			MyGUI::WidgetPtr createWidget(MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, MyGUI::WidgetPtr _parent, MyGUI::ICroppedRectangle* _croppedParent, MyGUI::IWidgetCreator* _creator, const std::string& _name);
			bool isFalseType(MyGUI::WidgetPtr _ptr, const std::string &_key);

			static Awesomium::WebCore* getCore() { return msCore; }

		private:
			void Awesomium_SourceURL(MyGUI::WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Awesomium_Transparent(MyGUI::WidgetPtr _widget, const std::string &_key, const std::string &_value);

			void notifyFrameStart(float _time);
			virtual void handleKeyMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		private:
			static Awesomium::WebCore* msCore;
			KeyboardHook* mKeyboardHook;
		};

	} // namespace factory

} // namespace Awesomium

#endif // __AWESOMIUM_WIDGET_FACTORY_H__
