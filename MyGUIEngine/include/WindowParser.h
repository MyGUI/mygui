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

				parser.registerDelegate("ShowWindowCaption") = newDelegate(this, &WindowParser::ShowWindowCaption);
				parser.registerDelegate("ShowWindowX") = newDelegate(this, &WindowParser::ShowWindowX);
				parser.registerDelegate("ShowWindowResize") = newDelegate(this, &WindowParser::ShowWindowResize);
			}

			void ShowWindowCaption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(WindowPtr, _widget);
				static_cast<WindowPtr>(_widget)->showWindowCaption(_value == "true");
			}

			void ShowWindowX(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(WindowPtr, _widget);
				static_cast<WindowPtr>(_widget)->showWindowX(_value == "true");
			}

			void ShowWindowResize(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(WindowPtr, _widget);
				static_cast<WindowPtr>(_widget)->showWindowResize(_value == "true");
			}

		};

	} // namespace parser

} // namespace MyGUI


#endif // _WINDOWPARSER_H_