/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __MYGUI_SCROLL_VIEW_FACTORY_H__
#define __MYGUI_SCROLL_VIEW_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_ScrollView.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT ScrollViewFactory : public BaseWidgetFactory<ScrollView>
		{
		public:
			ScrollViewFactory();
			~ScrollViewFactory();
		private:
			// методы для парсинга
			void ScrollView_VScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void ScrollView_HScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void ScrollView_CanvasAlign(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void ScrollView_CanvasSize(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_SCROLL_VIEW_FACTORY_H__
