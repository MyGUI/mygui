/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_BUTTON_FACTORY_H__
#define __MYGUI_BUTTON_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_Button.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT ButtonFactory : public BaseWidgetFactory<Button>
		{
		public:
			ButtonFactory();
			~ButtonFactory();
		private:
			// методы для парсинга
			void Button_Pressed(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_BUTTON_FACTORY_H__
