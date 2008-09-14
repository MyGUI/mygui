/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_VSCROLL_FACTORY_H__
#define __MYGUI_VSCROLL_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport VScrollFactory : public IWidgetFactory
		{
		public:
			VScrollFactory();
			~VScrollFactory();

			// реализация интерфейса фабрики
			const std::string & getTypeName();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name);

			// методы для парсинга, вызываются для базового класа, поэтому работает и для HScroll
			void Scroll_Range(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Scroll_Position(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Scroll_Page(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Scroll_ViewPage(WidgetPtr _widget, const std::string &_key, const std::string &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_VSCROLL_FACTORY_H__
