#ifndef _EDITPARSER_H_
#define _EDITPARSER_H_

#include "Prerequest.h"
//#include <Ogre.h>
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

				parser.registerDelegate("Edit_SetCursorPosition") = newDelegate(this, &EditParser::Edit_SetCursorPosition);
				parser.registerDelegate("Edit_SetTextSelect") = newDelegate(this, &EditParser::Edit_SetTextSelect);
				parser.registerDelegate("Edit_SetReadOnly") = newDelegate(this, &EditParser::Edit_SetReadOnly);
				parser.registerDelegate("Edit_SetPassword") = newDelegate(this, &EditParser::Edit_SetPassword);
				parser.registerDelegate("Edit_SetMultiLine") = newDelegate(this, &EditParser::Edit_SetMultiLine);
			}

			void Edit_SetCursorPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(EditPtr, _widget);
				static_cast<EditPtr>(_widget)->setTextCursor(util::parseSizeT(_value));
			}

			void Edit_SetTextSelect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(EditPtr, _widget);
				std::vector<std::string> vec = util::split(_value);
				if (vec.size() == 2) static_cast<EditPtr>(_widget)->setTextSelect(util::parseSizeT(vec[0]), util::parseSizeT(vec[1]));
			}

			void Edit_SetReadOnly(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(EditPtr, _widget);
				static_cast<EditPtr>(_widget)->setEditReadOnly(util::parseBool(_value));
			}

			void Edit_SetPassword(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(EditPtr, _widget);
				static_cast<EditPtr>(_widget)->setEditPassword(util::parseBool(_value));
			}

			void Edit_SetMultiLine(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(EditPtr, _widget);
				static_cast<EditPtr>(_widget)->setEditMultiLine(util::parseBool(_value));
			}

		};

	} // namespace parser

} // namespace MyGUI


#endif