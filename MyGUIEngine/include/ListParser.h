#ifndef _LISTPARSER_H_
#define _LISTPARSER_H_

#include "Prerequest.h"
#include "delegate.h"
#include "ParserManager.h"
#include "List.h"

namespace MyGUI
{

	namespace parser
	{

		class _MyGUIExport ListParser
		{
		public:
			ListParser()
			{
				// менеджер всех парсеров свойств
				ParserManager & parser = ParserManager::getInstance();

				parser.registerDelegate("ListAddString") = newDelegate(this, &ListParser::ListAddString);
			}

			void ListAddString(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(ListPtr, _widget);
				static_cast<ListPtr>(_widget)->addItemString(_value);
			}

		};

	} // namespace parser

} // namespace MyGUI


#endif // _LISTPARSER_H_