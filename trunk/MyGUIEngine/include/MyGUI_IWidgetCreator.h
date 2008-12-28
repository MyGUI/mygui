/*!
	@file
	@author		Albert Semenov
	@date		03/2008
	@module
*/
#ifndef __MYGUI_WIDGET_CREATOR_H__
#define __MYGUI_WIDGET_CREATOR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_Align.h"

namespace MyGUI
{

	class MYGUI_EXPORT IWidgetCreator
	{
		friend class WidgetManager;

	public:
		virtual ~IWidgetCreator() { }

	protected:
		virtual WidgetPtr baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name) = 0;

		// удяляет неудачника
		virtual void _destroyChildWidget(WidgetPtr _widget) = 0;

		// удаляет всех детей
		virtual void _destroyAllChildWidget() = 0;

		// удаляет виджет с закрытым конструктором
		void _deleteWidget(WidgetPtr _widget);

	public:
		// добавляет в список виджет
		// имплементировать только для рутовых креаторов
		virtual void _linkChildWidget(WidgetPtr _widget) { }
		// удаляет из списка
		// имплементировать только для рутовых креаторов
		virtual void _unlinkChildWidget(WidgetPtr _widget) { }

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_CREATOR_H__
