/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_TabFactory.h"
#include "MyGUI_Tab.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		TabFactory::TabFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("Tab_ButtonWidth") = newDelegate(this, &TabFactory::Tab_ButtonWidth);
			manager.registerDelegate("Tab_ButtonAutoWidth") = newDelegate(this, &TabFactory::Tab_ButtonAutoWidth);
			manager.registerDelegate("Tab_SmoothShow") = newDelegate(this, &TabFactory::Tab_SmoothShow);
			manager.registerDelegate("Tab_AddSheet") = newDelegate(this, &TabFactory::Tab_AddSheet);
			manager.registerDelegate("Tab_SelectSheet") = newDelegate(this, &TabFactory::Tab_SelectSheet);
		}

		TabFactory::~TabFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Tab_ButtonWidth");
			manager.unregisterDelegate("Tab_ButtonAutoWidth");
			manager.unregisterDelegate("Tab_SmoothShow");
			manager.unregisterDelegate("Tab_AddSheet");
			manager.unregisterDelegate("Tab_SelectSheet");
		}

		const std::string & TabFactory::getTypeName()
		{
			return Tab::getClassTypeName();
		}

		WidgetPtr TabFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name)
		{
			return new Tab(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void TabFactory::Tab_ButtonWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Tab, _widget, _key);
			static_cast<TabPtr>(_widget)->setButtonDefaultWidth(utility::parseInt(_value));
		}

		void TabFactory::Tab_ButtonAutoWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Tab, _widget, _key);
			static_cast<TabPtr>(_widget)->setButtonAutoWidth(utility::parseBool(_value));
		}

		void TabFactory::Tab_SmoothShow(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Tab, _widget, _key);
			static_cast<TabPtr>(_widget)->setSmoothShow(utility::parseBool(_value));
		}

		void TabFactory::Tab_AddSheet(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Tab, _widget, _key);
			static_cast<TabPtr>(_widget)->addSheet(_value);
		}

		void TabFactory::Tab_SelectSheet(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Tab, _widget, _key);
			static_cast<TabPtr>(_widget)->selectSheetIndex(utility::parseSizeT(_value), false);
		}

	} // namespace factory

} // namespace MyGUI
