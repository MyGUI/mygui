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
	struct MYGUI_EXPORT ItemInfo
	{

		ItemInfo() : index(ITEM_NONE) { }

		ItemInfo(size_t _index, size_t _select, size_t _active, size_t _accept, size_t _refuse, bool _update, bool _drag) :
			index(_index),
			update(_update),
			select(_index == _select),
			active(_index == _active),
			drag(_drag),
			drag_accept(_index == _accept),
			drag_refuse(_index == _refuse)
		{
		}


		// индекс этого элемента
		/** Index of element */
		size_t index;
		// изменилось не только состояние, но и содержимое
		/** State and interdan data changed */
		bool update;
		// нажат ли виджет
		/** Is widget selected */
		bool select;
		// активен ли виджет
		/** Is widget active */
		bool active;
		// виджет для перетаскивания или нет
		/** Is widget able to be dragged */
		bool drag;
		// айтем принимамет дроп
		/** Is widget accept drag */
		bool drag_accept;
		// айтем не берет дроп
		/** Is widget refuse drag */
		bool drag_refuse;
	};

	enum MYGUI_OBSOLETE_START("use : NotifyItemData::NotifyItem")
	{
		NOTIFY_MOUSE_PRESSED,
		NOTIFY_MOUSE_RELEASED,
		NOTIFY_KEY_PRESSED,
		NOTIFY_KEY_RELEASED,
	} MYGUI_OBSOLETE_END ;

	struct MYGUI_EXPORT NotifyItemData
	{
		typedef enum
		{
			MousePressed,
			MouseReleased,
			KeyPressed,
			KeyReleased
		} NotifyItem;

		NotifyItemData(size_t _index, NotifyItem _notify, int _x, int _y, MouseButton _id) :
			index(_index), notify(_notify), x(_x), y(_y), id(_id), code(KeyCode::None), key(0) {}

		NotifyItemData(size_t _index, NotifyItem _notify, KeyCode _code, Char _key) :
			index(_index), notify(_notify), x(0), y(0), id(MouseButton::None), code(_code), key(_key) { }

		NotifyItemData(size_t _index, NotifyItem _notify, KeyCode _code) :
			index(_index), notify(_notify), x(0), y(0), id(MouseButton::None), code(_code), key(KeyCode::None) { }

		size_t index;
		NotifyItem notify;
		int x;
		int y;
		MouseButton id;
		KeyCode code;
		Char key;
	};

	// делегат для событий айтема
	typedef delegates::CDelegate2<WidgetPtr, const NotifyItemData &> EventHandle_WidgetNotifyItemData;

	// делегаты для обновления
	typedef delegates::CDelegate2<WidgetPtr, WidgetPtr> EventHandle_WidgetWidget;
	typedef delegates::CDelegate3<WidgetPtr, IntCoord&, bool> EventHandle_WidgetRefCoordBool;
	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr, const ItemInfo &> EventHandle_WidgetWidgetItemInfo;

} // namespace MyGUI

#endif // __MYGUI_ITEM_INFO_H__
