/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_FACTORY_H__
#define __MYGUI_WIDGET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT WidgetFactory : public BaseWidgetFactory<Widget>
		{
		public:
			WidgetFactory();
			~WidgetFactory();
		private:
			void notifyChangeLanguage(const std::string & _language);

			// методы для парсинга
			void Widget_Caption(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Position(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Size(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Coord(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Visible(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_TextColour(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_FontName(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_FontHeight(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Alpha(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_State(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_NeedKey(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_NeedMouse(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_TextAlign(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Enabled(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_InheritsAlpha(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_InheritsPick(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_MaskPick(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_NeedToolTip(WidgetPtr _widget, const std::string &_key, const std::string &_value);

			void Widget_eventMouseLostFocus(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_eventMouseSetFocus(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_eventMouseDrag(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_eventMouseMove(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_eventMouseWheel(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_eventMouseButtonPressed(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_eventMouseButtonReleased(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_eventMouseButtonClick(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_eventMouseButtonDoubleClick(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			//... many events still missed
			void Widget_eventToolTip(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		private:
			bool mNeedTranslate;

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_WIDGET_FACTORY_H__
