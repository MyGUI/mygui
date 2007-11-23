#ifndef __BUTTON_PARSER_H__
#define __BUTTON_PARSER_H__

#include "Prerequest.h"
#include "delegate.h"
#include "ParserManager.h"
#include "Button.h"

namespace MyGUI
{

	namespace parser
	{

		class _MyGUIExport ButtonParser
		{
		public:
			ButtonParser()
			{
				// менеджер всех парсеров свойств
				ParserManager & parser = ParserManager::getInstance();

				parser.registerDelegate("ButtonPressed") = newDelegate(this, &ButtonParser::ButtonPressed);
			}

			void ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(ButtonPtr, _widget);
				static_cast<ButtonPtr>(_widget)->setButtonPressed(util::parseBool(_value));
			}

		};

	} // namespace parser

} // namespace MyGUI


#endif // __BUTTON_PARSER_H__