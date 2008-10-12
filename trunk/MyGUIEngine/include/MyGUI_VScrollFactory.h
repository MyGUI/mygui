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
#include "MyGUI_VScroll.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport VScrollFactory : public BaseWidgetFactory<VScroll>
		{
		public:
			VScrollFactory();
			~VScrollFactory();
		private:
			// ������ ��� ��������, ���������� ��� �������� �����, ������� �������� � ��� HScroll
			void Scroll_Range(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Scroll_Position(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Scroll_Page(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Scroll_ViewPage(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_VSCROLL_FACTORY_H__
