/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_TAB_FACTORY_H__
#define __MYGUI_TAB_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_Tab.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT TabFactory : public BaseWidgetFactory<Tab>
		{
		public:
			TabFactory();
			~TabFactory();
		private:
			// методы для парсинга
			void Tab_ButtonWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Tab_ButtonAutoWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Tab_SmoothShow(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Tab_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Tab_SelectItem(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_TAB_FACTORY_H__
