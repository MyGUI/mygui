#ifndef _SCROLLPARSER_H_
#define _SCROLLPARSER_H_

#include "Prerequest.h"
#include "delegate.h"
#include "ParserManager.h"
#include "ScrollBase.h"

namespace MyGUI
{

	namespace parser
	{

		class _MyGUIExport ScrollParser
		{
		public:
			ScrollParser()
			{
				// менеджер всех парсеров свойств
				ParserManager & parser = ParserManager::getInstance();

				parser.registerDelegate("ScrollRange") = newDelegate(this, &ScrollParser::ScrollRange);
				parser.registerDelegate("ScrollPosition") = newDelegate(this, &ScrollParser::ScrollPosition);
			}

			void ScrollRange(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(ScrollBasePtr, _widget);
				static_cast<ScrollBasePtr>(_widget)->setScrollRange(parseValue<size_t>(_value));
			}

			void ScrollPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(ScrollBasePtr, _widget);
				static_cast<ScrollBasePtr>(_widget)->setScrollPosition(parseValue<size_t>(_value));
			}

		};

	} // namespace parser

} // namespace MyGUI


#endif // _SCROLLPARSER_H_