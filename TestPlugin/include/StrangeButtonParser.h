#ifndef _STRANGEBUTTONPARSER_H_
#define _STRANGEBUTTONPARSER_H_


#include "Prerequest.h"
#include <Ogre.h>
#include "delegate.h"
#include "ParserManager.h"
#include "StrangeButton.h"

namespace MyGUI
{

	namespace parser
	{

		class StrangeButtonParser
		{
		public:
			StrangeButtonParser()
			{
				// менеджер всех парсеров свойств
				ParserManager & parser = ParserManager::getInstance();

				parser.registerDelegate("StrangeButtonPressed") = newDelegate(this, &StrangeButtonParser::ButtonPressed);
			}

			void ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(StrangeButtonPtr, _widget);
				static_cast<StrangeButtonPtr>(_widget)->setButtonPressed(util::parseBool(_value));
			}

		};

	} // namespace parser

} // namespace MyGUI


#endif