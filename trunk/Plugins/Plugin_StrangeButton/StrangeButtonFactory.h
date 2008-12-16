/*#ifndef __STRANGE_BUTTON_FACTORY_H__
#define __STRANGE_BUTTON_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "StrangeButton.h"

namespace MyGUI
{
	namespace factory
	{

		class StrangeButtonFactory : public BaseWidgetFactory<StrangeButton>
		{
		public:
			StrangeButtonFactory();
			~StrangeButtonFactory();
		private:
			// методы для парсинга
			void ButtonPressed(WidgetPtr _widget, const std::string &_key, const std::string &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __STRANGE_BUTTON_FACTORY_H__
*/