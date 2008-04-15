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
		}

		const Ogre::String& EditFactory::getType()
		{
			return Edit::_getType();
		}

		WidgetPtr EditFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name)
		{
			return new Edit(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void EditFactory::Edit_CursorPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(EditPtr, _widget, _key);
			static_cast<EditPtr>(_widget)->setTextCursor(utility::parseSizeT(_value));
		}

		void EditFactory::Edit_TextSelect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(EditPtr, _widget, _key);
			std::vector<std::string> vec = utility::split(_value);
			if (vec.size() == 2) static_cast<EditPtr>(_widget)->setTextSelect(utility::parseSizeT(vec[0]), utility::parseSizeT(vec[1]));
		}

		void EditFactory::Edit_ReadOnly(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(EditPtr, _widget, _key);
			static_cast<EditPtr>(_widget)->setEditReadOnly(utility::parseBool(_value));
		}

		void EditFactory::Edit_Password(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(EditPtr, _widget, _key);
			static_cast<EditPtr>(_widget)->setEditPassword(utility::parseBool(_value));
		}

		void EditFactory::Edit_MultiLine(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(EditPtr, _widget, _key);
			static_cast<EditPtr>(_widget)->setEditMultiLine(utility::parseBool(_value));
		}

		void EditFactory::Edit_PasswordChar(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(EditPtr, _widget, _key);
			static_cast<EditPtr>(_widget)->setPasswordChar(_value);
		}

	} // namespace factory
} // namespace MyGUI
