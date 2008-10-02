/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_EditFactory.h"
#include "MyGUI_Edit.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		EditFactory::EditFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

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
			manager.registerDelegate("Edit_ShowVScroll") = newDelegate(this, &EditFactory::Edit_ShowVScroll);
			manager.registerDelegate("Edit_ShowHScroll") = newDelegate(this, &EditFactory::Edit_ShowHScroll);
			manager.registerDelegate("Edit_WordWrap") = newDelegate(this, &EditFactory::Edit_WordWrap);
		}

		EditFactory::~EditFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

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
			manager.unregisterDelegate("Edit_ShowHScroll");
			manager.unregisterDelegate("Edit_ShowVScroll");
			manager.unregisterDelegate("Edit_WordWrap");
		}

		const std::string & EditFactory::getTypeName()
		{
			return Edit::getClassTypeName();
		}

		WidgetPtr EditFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name)
		{
			return new Edit(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void EditFactory::Edit_CursorPosition(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			static_cast<EditPtr>(_widget)->setTextCursor(utility::parseSizeT(_value));
		}

		void EditFactory::Edit_TextSelect(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			std::vector<std::string> vec = utility::split(_value);
			if (vec.size() == 2) static_cast<EditPtr>(_widget)->setTextSelect(utility::parseSizeT(vec[0]), utility::parseSizeT(vec[1]));
		}

		void EditFactory::Edit_ReadOnly(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			static_cast<EditPtr>(_widget)->setEditReadOnly(utility::parseBool(_value));
		}

		void EditFactory::Edit_Password(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			static_cast<EditPtr>(_widget)->setEditPassword(utility::parseBool(_value));
		}

		void EditFactory::Edit_MultiLine(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			static_cast<EditPtr>(_widget)->setEditMultiLine(utility::parseBool(_value));
		}

		void EditFactory::Edit_PasswordChar(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			static_cast<EditPtr>(_widget)->setPasswordChar(_value);
		}

		void EditFactory::Edit_MaxTextLength(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			static_cast<EditPtr>(_widget)->setMaxTextLength(utility::parseSizeT(_value));
		}

		void EditFactory::Edit_OverflowToTheLeft(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			static_cast<EditPtr>(_widget)->setOverflowToTheLeft(utility::parseBool(_value));
		}

		void EditFactory::Edit_Static(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			static_cast<EditPtr>(_widget)->setEditStatic(utility::parseBool(_value));
		}

		void EditFactory::Edit_ShowVScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			static_cast<EditPtr>(_widget)->showVScroll(utility::parseBool(_value));
		}

		void EditFactory::Edit_ShowHScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			static_cast<EditPtr>(_widget)->showHScroll(utility::parseBool(_value));
		}

		void EditFactory::Edit_WordWrap(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Edit, _widget, _key);
			static_cast<EditPtr>(_widget)->setEditWordWrap(utility::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI
