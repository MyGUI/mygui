/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_FACTORY_H__
#define __MYGUI_WIDGET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport WidgetFactory : public WidgetFactoryInterface
		{
		public:
			WidgetFactory();
			~WidgetFactory();

			// реализация интерфейса фабрики
			const std::string& getType();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const std::string& _name);

		private:
			void notifyChangeLanguage(const std::string & _language);

			// методы для парсинга
			void Widget_Caption(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Move(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Size(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Show(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Colour(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_FontName(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_FontHeight(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Alpha(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_State(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_NeedKey(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_NeedMouse(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_AlignText(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_Enabled(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_InheritsAlpha(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Widget_InheritsPeek(WidgetPtr _widget, const std::string &_key, const std::string &_value);
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
