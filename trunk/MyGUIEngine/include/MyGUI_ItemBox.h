/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_ITEM_BOX_H__
#define __MYGUI_ITEM_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_Any.h"

namespace MyGUI
{

	// ��������� ���������� �� ����������� ��������
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


		// ������ ����� ��������
		/** Index of element */
		size_t index;
		// ���������� �� ������ ���������, �� � ����������
		/** State and interdan data changed */
		bool update;
		// ����� �� ������
		/** Is widget selected */
		bool select;
		// ������� �� ������
		/** Is widget active */
		bool active;
		// ������ ��� �������������� ��� ���
		/** Is widget able to be dragged */
		bool drag;
		// ����� ���������� ����
		/** Is widget accept drag */
		bool drag_accept;
		// ����� �� ����� ����
		/** Is widget refuse drag */
		bool drag_refuse;
	};

	enum NotifyItem {
		NOTIFY_MOUSE_PRESSED,
		NOTIFY_MOUSE_RELEASED,
		NOTIFY_KEY_PRESSED,
		NOTIFY_KEY_RELEASED,
	};

	struct NotifyItemData {
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

	// ������� ��� ������� ������
	typedef delegates::CDelegate2<WidgetPtr, const NotifyItemData &> EventInfo_WidgetNotifyItemData;

	// �������� ��� ����������
	typedef delegates::CDelegate2<WidgetPtr, WidgetPtr> EventInfo_WidgetWidget;
	typedef delegates::CDelegate3<WidgetPtr, IntCoord&, bool> EventInfo_WidgetWidgetRefCoordBool;
	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr, const ItemInfo &> EventInfo_WidgetWidgetItemInfo;

	class _MyGUIExport ItemBox : public DDContainer
	{
		// ��� ������ ��������� ������������
		friend class factory::ItemBoxFactory;

		MYGUI_RTTI_CHILD_HEADER;

	protected:

		struct ItemDataInfo
		{
			ItemDataInfo(Any _data) :
				data(_data)
			{
			}
			Any data;
		};
		typedef std::vector<ItemDataInfo> VectorItemInfo;

	protected:
		ItemBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

	public:
		//------------------------------------------------------------------------------//
		// ����������� ��������

		//! Get number of items
		size_t getItemCount() { return (size_t)mCountItems; }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(Any _data = Any::Null) { insertItemAt(ITEM_NONE, _data); }

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();


		//------------------------------------------------------------------------------//
		// ����������� �����������

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemIndexSelected() { return mIndexSelect; }

		//! Select specified _index
		void setItemSelectedAt(size_t _index);

		//! Clear item selection
		void clearItemSelected() { setItemSelectedAt(ITEM_NONE); }


		//------------------------------------------------------------------------------//
		// ����������� �������

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ItemBox::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}



		// #ifdef MYGUI_USING_OBSOLETE

		MYGUI_OBSOLETE("use ItemBox::insertItemAt(size_t _index, Any _data)")
		void insertItem(size_t _index, Any _data = Any::Null) { insertItemAt(_index, _data); }

		MYGUI_OBSOLETE("use ItemBox::setItemDataAt(size_t _index, Any _data)")
		void setItemData(size_t _index, Any _data) { setItemDataAt(_index, _data); }

		MYGUI_OBSOLETE("use ItemBox::removeItemAt(size_t _index)")
		void deleteItem(size_t _index) { removeItemAt(_index); }

		MYGUI_OBSOLETE("use ItemBox::removeAllItems()")
		void deleteAllItems() { removeAllItems(); }

		MYGUI_OBSOLETE("use ItemBox::getItemIndexSelected()")
		size_t getItemSelect() { return getItemIndexSelected(); }

		MYGUI_OBSOLETE("use ItemBox::clearItemSelected()")
		void resetItemSelect() { clearItemSelected(); }

		MYGUI_OBSOLETE("use ItemBox::setItemSelectedAt(size_t _index)")
		void setItemSelect(size_t _index) { setItemSelectedAt(_index); }

		// #endif // MYGUI_USING_OBSOLETE


		void setItemBoxAlignVert(bool _vert);
		bool getItemBoxAlignVert() { return mAlignVert; }

		// ���������� ������ ��������, �� ��������� �� ������ ������
		size_t getIndexByWidget(WidgetPtr _widget);

		// ���������� ������, ��������� ��� �����
		WidgetPtr getWidgetDrop() { return mItemDrag; }

		// ���������� ������ �������, ���� �� �����
		WidgetPtr getWidgetByIndex(size_t _index);

		void resetDrop() { endDrop(true); }

		//----------------------------------------------------------------//
		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		virtual void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		void setPosition(int _left, int _top) { Widget::setPosition(IntPoint(_left, _top)); }
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		void setPosition(int _left, int _top, int _width, int _height) { setPosition(IntCoord(_left, _top, _width, _height)); }
		//! @copydoc Widget::setSize(int _width, int _height)
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }

		// event : ������ �� �������� ������
		// signature : void method(MyGUI::WidgetPtr _sender, WidgetPtr _item)
		EventInfo_WidgetWidget requestCreateWidgetItem;

		// event : ������ �� ������ ������
		// signature : void method(MyGUI::WidgetPtr _sender, WidgetPtr _client, IntCoord & _coord, bool _drop)
		EventInfo_WidgetWidgetRefCoordBool requestCoordWidgetItem;

		// event : ������ �� ���������� ������
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _item, const MyGUI::ItemInfo & _info)
		EventInfo_WidgetWidgetItemInfo requestUpdateWidgetItem;

		// event : ������� ������ ���� ��� Enter �� ��������
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _index)
		EventInfo_WidgetSizeT eventSelectItemAccept;

		// event : ���������� ������� ����������� ��������
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _index)
		EventInfo_WidgetSizeT eventChangeItemPosition;

		// event : ������ ���� �� ��������
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _index)
		EventInfo_WidgetSizeT eventMouseItemActivate;

		// event : ������� ��������� � ���������� �������
		// signature : void method(MyGUI::WidgetPtr _sender, const MyGUI::NotifyItemData & _info)
		EventInfo_WidgetNotifyItemData eventNotifyItem;

	protected:

		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMouseDrag(int _left, int _top);

		virtual void onMouseWheel(int _rel);
		virtual void onKeyLostFocus(WidgetPtr _new);
		virtual void onKeySetFocus(WidgetPtr _old);

		void notifyKeyButtonPressed(WidgetPtr _sender, KeyCode _key, Char _char);
		void notifyKeyButtonReleased(WidgetPtr _sender, KeyCode _key);
		void notifyScrollChangePosition(WidgetPtr _sender, size_t _index);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);
		void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old);
		void notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new);
		void notifyMouseButtonDoubleClick(WidgetPtr _sender);
		void requestGetContainer(WidgetPtr _sender, WidgetPtr & _container, size_t & _index);
		void notifyMouseDrag(WidgetPtr _sender, int _left, int _top);
		void notifyMouseButtonPressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseButtonReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id);

		virtual void removeDropItems();
		virtual void updateDropItems();
		virtual void updateDropItemsState(const DropWidgetState & _state);

		// ��������� ������ � �������, ��� ��������� �������� 
		void updateMetrics();

		// ��������� ������, �� ������� ��������
		void updateScroll();

		// ������ ��������� ��� ������� ��� �����
		void _updateAllVisible(bool _redraw);

		void updateFromResize(const IntSize& _size);

		// ���������� ��������� �����, ���� ��� ���, �� ���������
		// ������� ������ ���������������
		WidgetPtr getItemWidget(size_t _index);

		void _updateScrollWidget();

		void setContainerItemInfo(size_t _index, bool _set, bool _accept);

		// ���������� ������ ���������
		void resetCurrentActiveItem();
		// ���� � ������������� ������������ �����
		void findCurrentActiveItem();

		// ����������� � ��������� ����� �� ������� ����
		virtual size_t getContainerIndex(const IntPoint & _point);

		// ���������� �����������, ��� ����� ��������������� ���������
		virtual void resetContainer(bool _update);

	private:
		VScrollPtr mWidgetScroll;

		// ���� ���� � �������
		VectorWidgetPtr mVectorItems;

		// ������ ������ ������
		IntSize mSizeItem;

		// ����������� ������� � ��������
		int mScrollRange;
		// ������� ������� � ��������
		int mScrollPosition;

		// ����������� ������� � ����� ������
		int mCountItemInLine;
		// ����������� �����
		int mCountLines;
		// ����������� ������� �����
		int mCountItems;
		// ������������ ����������� ������� �����
		int mCountLineVisible;


		// ����� ������� ������
		int mLineTop;
		// ������� �������� �������� �������� � ��������
		// ������� ��� �������� �� ����� ������
		int mOffsetTop;

		// ������� ���������� ������� ��� ITEM_NONE
		size_t mIndexSelect;
		// ������������ ������� ��� ITEM_NONE
		size_t mIndexActive;
		// ������ �� ��������� ������ ��� ITEM_NONE
		size_t mIndexAccept;
		// ������ �� ��������� ������ ��� ITEM_NONE
		size_t mIndexRefuse;


		// ����� �� �� ����� �����
		bool mIsFocus;

		// ��������� ������ �� ������
		VectorItemInfo mItemsInfo;

		WidgetPtr mItemDrag;
		IntPoint mPointDragOffset;

		bool mAlignVert;

	}; // class ItemBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_ITEM_BOX_H__
