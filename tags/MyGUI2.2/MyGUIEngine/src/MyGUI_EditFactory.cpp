/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_EditFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		EditFactory::EditFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("Edit_CursorPosition") = newDelegate(this, &EditFactory::Edit_CursorPosition);
			manager.registerDelegate("Edit_TextSelect") = newDelegate(this, &EditFactory::Edit_TextSelect);
			manager.registerDelegate("Edit_ReadOnly") = newDelegate(this, &EditFactory::Edit_ReadOnly);
			manager.registerDelegate("Edit_Password") = newDelegate(this, &EditFactory::Edit_Password);
			manager.registerDelegate("Edit_MultiLine") = newDelegate(this, &EditFactory::Edit_MultiLine);
			manager.registerDelegate("Edit_PasswordChar") = newDelegate(this, &EditFactory::Edit_PasswordChar);
			manager.registerDelegate("Edit_MaxTextLength") = newDelegate(this, &EditFactory::Edit_MaxTextLength);
			manager.registerDelegate("Edit_OverflowToTheLeft") = newDelegate(this, &EditFactory::Edit_OverflowToTheLeft);
			manager.registerDelegate("Edit_Static") = newDelegate(this, &EditFactory::Edit_Static);
			manager.registerDelegate("Edit_VisibleVScroll") = newDelegate(this, &EditFactory::Edit_VisibleVScroll);
			manager.registerDelegate("Edit_VisibleHScroll") = newDelegate(this, &EditFactory::Edit_VisibleHScroll);
			manager.registerDelegate("Edit_WordWrap") = newDelegate(this, &EditFactory::Edit_WordWrap);
			manager.registerDelegate("Edit_TabPrinting") = newDelegate(this, &EditFactory::Edit_TabPrinting);

			manager.registerDelegate("Edit_ShowVScroll") = newDelegate(this, &EditFactory::Edit_VisibleVScroll);
			manager.registerDelegate("Edit_ShowHScroll") = newDelegate(this, &EditFactory::Edit_VisibleHScroll);
		}

		EditFactory::~EditFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("Edit_CursorPosition");
			manager.unregisterDelegate("Edit_TextSelect");
			manager.unregisterDelegate("Edit_ReadOnly");
			manager.unregisterDelegate("Edit_Password");
			manager.unregisterDelegate("Edit_MultiLine");
			manager.unregisterDelegate("Edit_PasswordChar");
			manager.unregisterDelegate("Edit_MaxTextLength");
			manager.unregisterDelegate("Edit_OverflowToTheLeft");
			manager.unregisterDelegate("Edit_Static");
			manager.unregisterDelegate("Edit_VisibleHScroll");
			manager.unregisterDelegate("Edit_VisibleVScroll");
			manager.unregisterDelegate("Edit_WordWrap");
			manager.unregisterDelegate("Edit_TabPrinting");

			manager.unregisterDelegate("Edit_ShowHScroll");
			manager.unregisterDelegate("Edit_ShowVScroll");
		}

		void EditFactory::Edit_CursorPosition(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setTextCursor(utility::parseSizeT(_value));
		}

		void EditFactory::Edit_TextSelect(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			std::vector<std::string> vec = utility::split(_value);
			if (vec.size() == 2) static_cast<EditPtr>(_widget)->setTextSelection(utility::parseSizeT(vec[0]), utility::parseSizeT(vec[1]));
		}

		void EditFactory::Edit_ReadOnly(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setEditReadOnly(utility::parseBool(_value));
		}

		void EditFactory::Edit_Password(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setEditPassword(utility::parseBool(_value));
		}

		void EditFactory::Edit_MultiLine(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setEditMultiLine(utility::parseBool(_value));
		}

		void EditFactory::Edit_PasswordChar(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setPasswordChar(_value);
		}

		void EditFactory::Edit_MaxTextLength(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setMaxTextLength(utility::parseSizeT(_value));
		}

		void EditFactory::Edit_OverflowToTheLeft(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setOverflowToTheLeft(utility::parseBool(_value));
		}

		void EditFactory::Edit_Static(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setEditStatic(utility::parseBool(_value));
		}

		void EditFactory::Edit_VisibleVScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setVisibleVScroll(utility::parseBool(_value));
		}

		void EditFactory::Edit_VisibleHScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setVisibleHScroll(utility::parseBool(_value));
		}

		void EditFactory::Edit_WordWrap(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setEditWordWrap(utility::parseBool(_value));
		}

		void EditFactory::Edit_TabPrinting(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<EditPtr>(_widget)->setTabPrinting(utility::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI
