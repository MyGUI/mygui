#ifndef _EDITPARSER_H_
#define _EDITPARSER_H_

#include "Prerequest.h"
#include <Ogre.h>
#include "delegate.h"
#include "ParserManager.h"
#include "Edit.h"

namespace MyGUI
{

	namespace parser
	{

		class _MyGUIExport EditParser
		{
		public:
			EditParser()
			{
				// менеджер всех парсеров свойств
				ParserManager & parser = ParserManager::getInstance();

//				parser.registerDelegate("ButtonPressed") = newDelegate(this, &ButtonParser::ButtonPressed);
			}

			/*void ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(ButtonPtr, _widget);
				static_cast<ButtonPtr>(_widget)->setButtonPressed(_value == "true");
			}*/

		};

	} // namespace parser

} // namespace MyGUI


#endif