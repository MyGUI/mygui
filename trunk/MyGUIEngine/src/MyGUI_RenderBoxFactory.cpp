/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_RenderBoxFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		RenderBoxFactory::RenderBoxFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("RenderBox_BackgroungColour") = newDelegate(this, &RenderBoxFactory::RenderBox_BackgroungColour);
		}

		RenderBoxFactory::~RenderBoxFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("RenderBox_BackgroungColour");
		}

		void RenderBoxFactory::RenderBox_BackgroungColour(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<RenderBoxPtr>(_widget)->setBackgroungColour(Colour::parse(_value).toColourValue());
		}

	} // namespace factory
} // namespace MyGUI
