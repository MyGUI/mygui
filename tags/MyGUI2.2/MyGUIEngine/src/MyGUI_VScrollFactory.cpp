/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_VScrollFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		VScrollFactory::VScrollFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("Scroll_Range") = newDelegate(this, &VScrollFactory::Scroll_Range);
			manager.registerDelegate("Scroll_Position") = newDelegate(this, &VScrollFactory::Scroll_Position);
			manager.registerDelegate("Scroll_Page") = newDelegate(this, &VScrollFactory::Scroll_Page);
			manager.registerDelegate("Scroll_ViewPage") = newDelegate(this, &VScrollFactory::Scroll_ViewPage);
		}

		VScrollFactory::~VScrollFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("Scroll_Range");
			manager.unregisterDelegate("Scroll_Position");
			manager.unregisterDelegate("Scroll_Page");
		}

		void VScrollFactory::Scroll_Range(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<VScrollPtr>(_widget)->setScrollRange(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_Position(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<VScrollPtr>(_widget)->setScrollPosition(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_Page(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<VScrollPtr>(_widget)->setScrollPage(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_ViewPage(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<VScrollPtr>(_widget)->setScrollViewPage(utility::parseSizeT(_value));
		}

	} // namespace factory

} // namespace MyGUI
