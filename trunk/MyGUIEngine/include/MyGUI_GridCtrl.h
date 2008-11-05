/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#ifndef __MYGUI_GRID_CTRL_H__
#define __MYGUI_GRID_CTRL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_ItemInfo.h"
#include "MyGUI_Any.h"

namespace MyGUI
{

	class _MyGUIExport GridCtrl : public DDContainer
	{
		// ��� ������ ��������� ������������
		friend class factory::BaseWidgetFactory<GridCtrl>;

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
		GridCtrl(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

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

		//! Redraw at a specified position
		void redrawItemAt(size_t _index);

		//! Redraw all items
		void redrawAllItems() { _updateAllVisible(true); }


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
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "GridCtrl::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}

		void setGridCtrlAlignVert(bool _vert);
		bool getGridCtrlAlignVert() { return mAlignVert; }

		// ���������� ������ ��������, �� ��������� �� ������ ������
		size_t getIndexByWidget(WidgetPtr _widget);

		// ���������� ������, ��������� ��� �����
		WidgetPtr getWidgetDrop() { return mItemDrag; }

		// ���������� ������ �������, ���� �� �����
		WidgetPtr getWidgetByIndex(size_t _index);

		void resetDrop() { endDrop(true); }

		//! @copydoc Widget::setPosition(const IntPoint & _point)
		virtual void setPosition(const IntPoint & _point);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize & _size);
		//! @copydoc Widget::setCoord(const IntCoord & _coord)
		virtual void setCoord(const IntCoord & _coord);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		// event : ������ �� �������� ������
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _item)
		EventInfo_WidgetWidget requestCreateWidgetItem;

		// event : ������ �� ������ ������
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _client, MyGUI::IntCoord & _coord, bool _drop)
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
		void notifyRootMouseChangeFocus(WidgetPtr _sender, bool _focus);
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

	}; // class GridCtrl

} // namespace MyGUI

#endif // __MYGUI_GRID_CTRL_H__
