/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_ITEM_BOX_H__
#define __MYGUI_ITEM_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	struct ItemInfo
	{
		ItemInfo(size_t _index) :
			index(_index),
			data(null),
			only_state(false),
			select(false),
			active(false),
			drag(false),
			drag_result(false),
			drag_accept(false),
			drag_refuse(false)
		{
		}

		// ������ ����� ��������
		size_t index;
		// ���������������� ������
		void * data;
		// ���������� ������ ���������, � �� ����������
		bool only_state;
		// ����� �� ������
		bool select;
		// ������� �� ������
		bool active;
		// ������ ��� ��������������
		bool drag;
		// ����������� ������� ����� �� ������
		bool drag_result;
		// ����� ���������� �� ��� �� ���� �������
		bool drag_accept;
		// ����� �� ����� �� ��� �� ���� �������
		bool drag_refuse;
	};
	typedef std::vector<ItemInfo> VectorItemInfo;

	struct ItemDropInfo
	{
		ItemDropInfo() :
			index(ITEM_NONE),
			reseiver(null),
			index_reseiver(ITEM_NONE)
		{
		}

		ItemDropInfo(size_t _index, WidgetPtr _reseiver, size_t _index_reseiver) :
			index(_index),
			reseiver(_reseiver),
			index_reseiver(_index_reseiver)
		{
		}

		void reset()
		{
			index = ITEM_NONE;
			reseiver = null;
			index_reseiver = ITEM_NONE;
		}

		// ������ �������������� �������
		size_t index;
		// ����������� �������
		WidgetPtr reseiver;
		// ������ ������������ �������
		size_t index_reseiver;
	};

	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr, WidgetPtr&> EventInfo_WidgetWidgetRefWidget;
	typedef delegates::CDelegate4<WidgetPtr, WidgetPtr, IntCoord&, bool> EventInfo_WidgetWidgetRefCoordBool;
	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr, const ItemInfo&> EventInfo_WidgetWidgetItemInfo;
	typedef delegates::CDelegate3<WidgetPtr, const ItemDropInfo&, bool&> EventInfo_WidgetItemDropInfoBoolRef;
	typedef delegates::CDelegate2<WidgetPtr, ItemDropInfo> EventInfo_WidgetItemDropInfo;

	class _MyGUIExport ItemBox : public Widget
	{
		// ��� ������ ��������� ������������
		friend class factory::ItemBoxFactory;

	protected:
		ItemBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//!	@copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		void addItem();

		virtual void setSize(const IntSize& _size);
		virtual void setPosition(const IntCoord& _coord);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// event : ������ �� �������� ������
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _parent, MyGUI::WidgetPtr & _item)
		EventInfo_WidgetWidgetRefWidget requestCreateItem;

		// event : ������ �� ������ ������
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _client, MyGUI::IntCoord & _coord, bool _drop)
		EventInfo_WidgetWidgetRefCoordBool requestCoordItem;

		// event : ������ �� ���������� ������
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _item, size_t _index)
		EventInfo_WidgetWidgetItemInfo requestUpdateItem;

		// event : ������ �� ���� ������
		// signature : void method(MyGUI::WidgetPtr _sender, const ItemDropInfo& _info, bool & _result)
		EventInfo_WidgetItemDropInfoBoolRef requestDropItem;

		// event : ������ �� ���� ������
		// signature : void method(MyGUI::WidgetPtr _sender, ItemDropInfo _info)
		EventInfo_WidgetItemDropInfo eventDropAccept;

	protected:

		void _onMouseWheel(int _rel);
		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);

		void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _index);
		void notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel);
		void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);
		void requestGetDragItemInfo(WidgetPtr _sender, WidgetPtr & _list, size_t & _index);

		// ��������� ������ � �������, ��� ��������� �������� 
		void updateMetrics();

		// ��������� ������, �� ������� ��������
		void updateScroll();

		// ������ ��������� ��� ������� ��� �����
		void _updateAllVisible(bool _redraw);

		void updateFromResize(const IntSize& _size);

		WidgetPtr getItemWidget(size_t _index);

		void _updateScrollWidget();

		void _setDragItemInfo(size_t _index, bool _set, bool _accept);

	private:
		VScrollPtr mWidgetScroll;
		WidgetPtr mWidgetClient;

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

		// ����� �� �� ����� �����
		bool mIsFocus;

		// ��������� ������ �� ������
		VectorItemInfo mItemsInfo;

		WidgetPtr mItemDrag;
		WidgetPtr mOldDrop;
		bool mDropResult;
		ItemDropInfo mDropInfo;
		IntPoint mPointDragOffset;

	}; // class ItemBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_ITEM_BOX_H__
