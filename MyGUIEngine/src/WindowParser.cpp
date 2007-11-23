
#include "WindowParser.h"
#include "Window.h"
#include "delegate.h"
#include "ParserManager.h"

namespace MyGUI
{
	namespace parser
	{

		WindowParser::WindowParser()
		{
			// менеджер всех парсеров свойств
			MyGUI::ParserManager & parser = MyGUI::ParserManager::getInstance();

			parser.registerDelegate("WindowAutoAlpha") = newDelegate(this, &WindowParser::WindowAutoAlpha);
			parser.registerDelegate("WindowMinMax") = newDelegate(this, &WindowParser::WindowMinMax);
			parser.registerDelegate("WindowToStick") = newDelegate(this, &WindowParser::WindowToStick);
		}

		void WindowParser::WindowToStick(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setIsToStick(util::parseBool(_value));
		}

		void WindowParser::WindowAutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setAutoAlpha(util::parseBool(_value));
		}

		void WindowParser::WindowMinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setMinMax(util::parseIntRect(_value));
		}

	} // namespace parser
} // namespace MyGUI