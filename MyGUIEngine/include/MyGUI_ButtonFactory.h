/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_BUTTON_FACTORY_H__
#define __MYGUI_BUTTON_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport ButtonFactory : public WidgetFactoryInterface
		{
		public:
			ButtonFactory();
			~ButtonFactory();

			// реализация интерфейса фабрики
			const std::string& getType();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const std::string& _name);

			// методы для парсинга
			void Button_Pressed(WidgetPtr _widget, const std::string &_key, const std::string &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_BUTTON_FACTORY_H__
