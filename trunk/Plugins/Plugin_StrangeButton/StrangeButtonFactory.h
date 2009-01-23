/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/

#ifndef __STRANGE_BUTTON_FACTORY_H__
#define __STRANGE_BUTTON_FACTORY_H__

#include "StrangeButton.h"
#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"

namespace plugin
{
	namespace factory
	{

		class StrangeButtonFactory : public MyGUI::factory::BaseWidgetFactory<StrangeButton>
		{
		public:
			StrangeButtonFactory();
			~StrangeButtonFactory();
		private:
			// методы для парсинга
			void ButtonPressed(MyGUI::WidgetPtr _widget, const std::string &_key, const std::string &_value);
		};

	} // namespace factory
} // namespace plugin

#endif // __STRANGE_BUTTON_FACTORY_H__
