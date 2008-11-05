/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#ifndef __MYGUI_ITEM_INFO_H__
#define __MYGUI_ITEM_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	// структура информации об отображении элемента
	struct ItemInfo
	{

		ItemInfo() : index(ITEM_NONE) { }

		ItemInfo(size_t _index, size_t _select, size_t _active, size_t _accept, size_t _refuse, bool _update, bool _drag) :
			index(_index),
			select(_index == _select),
			active(_index == _active),
			drag_accept(_index == _accept),
			drag_refuse(_index == _refuse),
			update(_update),
			drag(_drag)
		{
		}


		// индекс этого элемента
		/** Index of element */
		size_t index;
		// изменилось не только состо€ние, но и содержимое
		/** State and interdan data changed */
		bool update;
		// нажат ли виджет
		/** Is widget selected */
		bool select;
		// активен ли виджет
		/** Is widget active */
		bool active;
		// виджет дл€ перетаскивани€ или нет
		/** Is widget able to be dragged */
		bool drag;
		// айтем принимамет дроп
		/** Is widget accept drag */
		bool drag_accept;
		// айтем не берет дроп
		/** Is widget refuse drag */
		bool drag_refuse;
	};

	enum NotifyItem
	{
		NOTIFY_MOUSE_PRESSED,
		NOTIFY_MOUSE_RELEASED,
		NOTIFY_KEY_PRESSED,
		NOTIFY_KEY_RELEASED,
	};

	struct NotifyItemData
	{
		NotifyItemData(size_t _index, NotifyItem _notify, int _x, int _y, MouseButton _id) :
			index(_index), notify(_notify), x(_x), y(_y), id(_id), code(KC_UNASSIGNED), key(0) {}

		NotifyItemData(size_t _index, NotifyItem _notify, KeyCode _code, Char _key) :
			index(_index), notify(_notify), x(0), y(0), id(MB_None), code(_code), key(_key) { }

		NotifyItemData(size_t _index, NotifyItem _notify, KeyCode _code) :
			index(_index), notify(_notify), x(0), y(0), id(MB_None), code(_code), key(KC_UNASSIGNED) { }

		size_t index;
		NotifyItem notify;
		int x;
		int y;
		MouseButton id;
		KeyCode code;
		Char key;
	};

	// делегат дл€ событий айтема
	typedef delegates::CDelegate2<WidgetPtr, const NotifyItemData &> EventInfo_WidgetNotifyItemData;

	// делегаты дл€ обновлени€
	typedef delegates::CDelegate2<WidgetPtr, WidgetPtr> EventInfo_WidgetWidget;
	typedef delegates::CDelegate3<WidgetPtr, IntCoord&, bool> EventInfo_WidgetWidgetRefCoordBool;
	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr, const ItemInfo &> EventInfo_WidgetWidgetItemInfo;

} // namespace MyGUI

#endif // __MYGUI_ITEM_INFO_H__
