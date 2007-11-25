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
			manager.registerDelegate("Edit_SetCursorPosition") = newDelegate(this, &EditFactory::Edit_SetCursorPosition);
			manager.registerDelegate("Edit_SetTextSelect") = newDelegate(this, &EditFactory::Edit_SetTextSelect);
			manager.registerDelegate("Edit_SetReadOnly") = newDelegate(this, &EditFactory::Edit_SetReadOnly);
			manager.registerDelegate("Edit_SetPassword") = newDelegate(this, &EditFactory::Edit_SetPassword);
			manager.registerDelegate("Edit_SetMultiLine") = newDelegate(this, &EditFactory::Edit_SetMultiLine);
		}

		EditFactory::~EditFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.unregisterDelegate("Edit_SetCursorPosition");
			manager.unregisterDelegate("Edit_SetTextSelect");
			manager.unregisterDelegate("Edit_SetReadOnly");
			manager.unregisterDelegate("Edit_SetPassword");
			manager.unregisterDelegate("Edit_SetMultiLine");
		}

		const Ogre::String& EditFactory::getType()
		{
			static Ogre::String type("Edit");
			return type;
		}

		WidgetPtr EditFactory::createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new Edit(_left, _top, _width, _height, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void EditFactory::Edit_SetCursorPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(EditPtr, _widget);
			static_cast<EditPtr>(_widget)->setTextCursor(util::parseSizeT(_value));
		}

		void EditFactory::Edit_SetTextSelect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(EditPtr, _widget);
			std::vector<std::string> vec = util::split(_value);
			if (vec.size() == 2) static_cast<EditPtr>(_widget)->setTextSelect(util::parseSizeT(vec[0]), util::parseSizeT(vec[1]));
		}

		void EditFactory::Edit_SetReadOnly(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(EditPtr, _widget);
			static_cast<EditPtr>(_widget)->setEditReadOnly(util::parseBool(_value));
		}

		void EditFactory::Edit_SetPassword(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(EditPtr, _widget);
			static_cast<EditPtr>(_widget)->setEditPassword(util::parseBool(_value));
		}

		void EditFactory::Edit_SetMultiLine(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(EditPtr, _widget);
			static_cast<EditPtr>(_widget)->setEditMultiLine(util::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI