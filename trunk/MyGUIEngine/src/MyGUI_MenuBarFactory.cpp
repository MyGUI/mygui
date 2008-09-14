/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "MyGUI_MenuBarFactory.h"
#include "MyGUI_MenuBar.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		MenuBarFactory::MenuBarFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("MenuBar_AddItem") = newDelegate(this, &MenuBarFactory::MenuBar_AddItem);
		}

		MenuBarFactory::~MenuBarFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("MenuBar_AddItem");
		}

		const std::string & MenuBarFactory::getTypeName()
		{
			return MenuBar::getClassTypeName();
		}

		WidgetPtr MenuBarFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name)
		{
			return new MenuBar(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void MenuBarFactory::MenuBar_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(MenuBar, _widget, _key);
			static_cast<MenuBarPtr>(_widget)->addItem(_value);
		}

	} // namespace factory
} // namespace MyGUI
