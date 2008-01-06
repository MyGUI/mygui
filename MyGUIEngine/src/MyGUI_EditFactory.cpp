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
		}

		EditFactory::~EditFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// регестрируем все парсеры
			manager.unregisterDelegate("Edit_CursorPosition");
			manager.unregisterDelegate("Edit_TextSelect");
			manager.unregisterDelegate("Edit_ReadOnly");
			manager.unregisterDelegate("Edit_Password");
			manager.unregisterDelegate("Edit_MultiLine");
		}

		const Ogre::String& EditFactory::getType()
		{
			return Edit::getType();
		}

		WidgetPtr EditFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new Edit(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void EditFactory::Edit_CursorPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(EditPtr, _widget);
			static_cast<EditPtr>(_widget)->setTextCursor(util::parseSizeT(_value));
		}

		void EditFactory::Edit_TextSelect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(EditPtr, _widget);
			std::vector<std::string> vec = util::split(_value);
			if (vec.size() == 2) static_cast<EditPtr>(_widget)->setTextSelect(util::parseSizeT(vec[0]), util::parseSizeT(vec[1]));
		}

		void EditFactory::Edit_ReadOnly(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(EditPtr, _widget);
			static_cast<EditPtr>(_widget)->setEditReadOnly(util::parseBool(_value));
		}

		void EditFactory::Edit_Password(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(EditPtr, _widget);
			static_cast<EditPtr>(_widget)->setEditPassword(util::parseBool(_value));
		}

		void EditFactory::Edit_MultiLine(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(EditPtr, _widget);
			static_cast<EditPtr>(_widget)->setEditMultiLine(util::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI
