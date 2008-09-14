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
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport TabFactory : public IWidgetFactory
		{
		public:
			TabFactory();
			~TabFactory();

			// реализация интерфейса фабрики
			const std::string & getTypeName();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name);

			// методы для парсинга
			void Tab_ButtonWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Tab_ButtonAutoWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Tab_SmoothShow(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Tab_AddSheet(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Tab_SelectSheet(WidgetPtr _widget, const std::string &_key, const std::string &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_TAB_FACTORY_H__
