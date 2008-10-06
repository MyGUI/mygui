/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LIST_H__
#define __MYGUI_LIST_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Any.h"

namespace MyGUI
{

	class _MyGUIExport List : public Widget
	{
		// ��� ������ ��������� ������������
		friend class factory::ListFactory;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		List(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

	public:
		//------------------------------------------------------------------------------------//
		// ������ ��� ����������� ��������
		//! Get number of items
		size_t getItemCount() { return mItemsInfo.size(); }

		//! Insert an item into a list at a specified position
		void insertItem(size_t _index, const Ogre::UTFString & _item, Any _data = Any::Null);
		//! Add an item to the end of a list
		void addItem(const Ogre::UTFString & _item, Any _data = Any::Null)
		{
			insertItem(ITEM_NONE, _item, _data);
		}

		//! Replace an item at a specified position
		void setItem(size_t _index, const Ogre::UTFString & _item);
		//! Replace an item data at a specified position
		void setItemData(size_t _index, Any _data);
		//! Reset an item data at a specified position
		void resetItemData(size_t _index) { setItemData(_index, Any::Null); }

		//! Get item from specified position
		const Ogre::UTFString & getItem(size_t _index);
		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemData(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT(_index < mItemsInfo.size(), "getItemData: index " << _index <<" out of range");
			return mItemsInfo[_index].second.castType<ValueType>(_throw);
		}

		//! Delete item at a specified position
		void deleteItem(size_t _index);
		//! Delete all items
		void deleteAllItems();

		//! Search item, returns the position of the first occurrence in list or ITEM_NONE if item not found
		size_t findItem(const Ogre::UTFString & _item);

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemSelect() { return mIndexSelect; }
		//! Reset item selection
		void resetItemSelect() { setItemSelect(ITEM_NONE); }
		//! Select specified _index
		void setItemSelect(size_t _index);


		//------------------------------------------------------------------------------------//
		// ������ ��� ������ �����
		//! Move all elements so specified becomes visible
		void beginToIndex(size_t _index);
		//! Move all elements so first becomes visible
		void beginToStart() { beginToIndex(0); }
		//! Move all elements so last becomes visible
		void beginToEnd() { if (!mItemsInfo.empty()) beginToIndex(mItemsInfo.size()-1); }
		//! Move all elements so selected becomes visible
		void beginToSelect() { beginToIndex(mIndexSelect); }

		// ����� �� �� �������, ��������� ��� ���
		/** Return true if item visible
			@param
				_index of item
			@param
				_fill if false function return true if only whole item is visible
				if true function return true even if only part of item is visible
		*/
		bool isItemVisible(size_t _index, bool _fill = true);
		//! Same as isItemVisible for selected item
		bool isItemSelectVisible(bool _fill = true) {return isItemVisible(mIndexSelect, _fill);}

		//! Set scroll visible when it needed
		void setScrollVisible(bool _visible);
		//! Set scroll position
		void setScrollPosition(size_t _position);

		//------------------------------------------------------------------------------------//
		// ��������������� ������ ��� ��������� �������
		void _setItemFocus(size_t _position, bool _focus);
		void _sendEventChangeScroll(size_t _position);

		//------------------------------------------------------------------------------------//
		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		//! @copydoc Widget::setSize(int _width, int _height)
		void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// ���������� ������������ ������ ��������� ��� ������ � ������������ ������
		//! Return optimal height to fit all items in List
		int getOptimalHeight() {return (mCoord.height - mWidgetClient->getHeight()) + ((int)mItemsInfo.size() * mHeightLine);}

		/** Event : Enter pressed or double click.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of selected item
		*/
		EventInfo_WidgetSizeT eventListSelectAccept;

		/** Event : Selected item position changed.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of new item
		*/
		EventInfo_WidgetSizeT eventListChangePosition;

		/** Event : Item was selected by mouse.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of selected item
		*/
		EventInfo_WidgetSizeT eventListMouseItemActivate;

		/** Event : Mouse is over item.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of focused item
		*/
		EventInfo_WidgetSizeT eventListMouseItemFocus;

		/** Event : Position of scroll changed.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _position)\n
			@param _position of scroll
		*/
		EventInfo_WidgetSizeT eventListChangeScroll;

	protected:

		void _onMouseWheel(int _rel);
		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);
		void _onKeyButtonPressed(KeyCode _key, Char _char);

		void notifyScrollChangePosition(WidgetPtr _sender, size_t _rel);
		void notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseDoubleClick(WidgetPtr _sender);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);
		void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old);
		void notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new);

		void updateScroll();
		void updateLine(bool _reset = false);
		void _setScrollView(size_t _position);

		// �������������� �� ������� �� ����
		void _redrawItemRange(size_t _start = 0);

		// �������������� ������
		void _redrawItem(size_t _index);

		// ������� ������ �� ������
		void _deleteString(size_t _index);

		// ���� � �������� �������
		void _selectIndex(size_t _index, bool _select);

		void _updateState() { setState(mIsFocus ? "select" : "normal"); }

	private:
		std::string mSkinLine;
		VScrollPtr mWidgetScroll;

		// ���� ���� � �������
		VectorWidgetPtr mWidgetLines;

		int mHeightLine; // ������ ����� ������
		int mTopIndex; // ������ ������ �������� ��������
		int mOffsetTop; // ������� ��������
		int mRangeIndex; // ����������� ������
		size_t mLastRedrawLine; // ��������� �������������� �����

		size_t mIndexSelect; // ������� ���������� ������� ��� ITEM_NONE
		size_t mLineActive; // ������� ������ ��� ������� ����

		typedef std::pair<Ogre::UTFString, Any> PairItem;
		typedef std::vector<PairItem> VectorItemInfo;
		VectorItemInfo mItemsInfo;

		// ����� �� �� ����� �����
		bool mIsFocus;
		bool mNeedVisibleScroll;

		IntSize mOldSize;

	}; // class List : public Widget

} // namespace MyGUI

#endif // __MYGUI_LIST_H__
