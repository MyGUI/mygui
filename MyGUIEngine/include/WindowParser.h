#ifndef _WINDOWPARSER_H_
#define _WINDOWPARSER_H_


#include "Prerequest.h"
#include <Ogre.h>
#include "delegate.h"
#include "ParserManager.h"
#include "Window.h"

namespace MyGUI
{

	namespace parser
	{

		class _MyGUIExport WindowParser
		{
		public:
			WindowParser()
			{
				// менеджер всех парсеров свойств
				ParserManager & parser = ParserManager::getInstance();

				parser.registerDelegate("WindowAutoAlpha") = newDelegate(this, &WindowParser::WindowAutoAlpha);
				parser.registerDelegate("WindowMinMax") = newDelegate(this, &WindowParser::WindowMinMax);
				parser.registerDelegate("WindowToStick") = newDelegate(this, &WindowParser::WindowToStick);
			}

			void WindowToStick(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(WindowPtr, _widget);
				static_cast<WindowPtr>(_widget)->setIsToStick(util::parseBool(_value));
			}

			void WindowAutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(WindowPtr, _widget);
				static_cast<WindowPtr>(_widget)->setAutoAlpha(util::parseBool(_value));
			}

			void WindowMinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(WindowPtr, _widget);
				static_cast<WindowPtr>(_widget)->setMinMax(util::parseIntRect(_value));
			}

		};

	} // namespace parser

} // namespace MyGUI


#endif // _WINDOWPARSER_H_