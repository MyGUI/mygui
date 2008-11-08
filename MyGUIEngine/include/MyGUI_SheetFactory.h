/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_SHEET_FACTORY_H__
#define __MYGUI_SHEET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_Sheet.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport SheetFactory : public BaseWidgetFactory<Sheet>
		{
		public:
			SheetFactory();
			~SheetFactory();
		private:
			// методы для парсинга
			void Sheet_ButtonWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Sheet_Select(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_SHEET_FACTORY_H__
