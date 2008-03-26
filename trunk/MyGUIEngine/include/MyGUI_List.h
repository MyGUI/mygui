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

namespace MyGUI
{

	class _MyGUIExport List : public Widget
	{
		// ��� ������ ��������� ������������
		friend class factory::ListFactory;

	protected:
		List(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {static Ogre::String type("List"); return type;}
		//!	@copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		//------------------------------------------------------------------------------------//
		// ������ ��� ����������� ��������
		//! Get number of items
		inline size_t getItemCount() { return mStringArray.size();}

		//! Insert an item into a list at a specified position
		void insertItem(size_t _index, const Ogre::DisplayString & _item);
		//! Add an item to the end of a list
		inline void addItem(const Ogre::DisplayString & _item) {insertItem(ITEM_NONE, _item);}
		//! Replace an item at a specified position
		void setItem(size_t _index, const Ogre::DisplayString & _item);
		//! Get item from specified position
		const Ogre::DisplayString & getItem(size_t _index);

		//! Delete item at a specified position
		void deleteItem(size_t _index);
		//! Delete all items
		void deleteAllItems();

		//! Get number of selected item (ITEM_NONE if none selected)
		inline size_t getItemSelect() {return mIndexSelect;}
		//! Reset item selection
		inline void resetItemSelect() {setItemSelect(ITEM_NONE);}
		//! Set item selection at a specified position
		void setItemSelect(size_t _index);


		//------------------------------------------------------------------------------------//
		// ������ ��� ������ �����
		//! Move all elements so specified becomes visible
		void beginToIndex(size_t _index);
		//! Move all elements so first becomes visible
		inline void beginToStart() { beginToIndex(0); }
		//! Move all elements so last becomes visible
		inline void beginToEnd() { if (!mStringArray.empty()) beginToIndex(mStringArray.size()-1); }
		//! Move all elements so selected becomes visible
		inline void beginToSelect() { beginToIndex(mIndexSelect); }

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
		inline bool isItemSelectVisible(bool _fill = true) {return isItemVisible(mIndexSelect, _fill);}

		//! Set scroll visible when it needed
		void setScrollVisible(bool _visible);
		//! Set scroll position
		void setScrollPosition(size_t _position);

		//------------------------------------------------------------------------------------//
		// ��� ��������� � ���� �����
		/*virtual void setTextAlign(Align _align);
		virtual Align getTextAlign();

		virtual void setColour(const Ogre::ColourValue & _colour);
		virtual const Ogre::ColourValue & getColour();

		virtual void setFontName(const Ogre::String & _font);
		virtual const Ogre::String & getFontName();

		virtual void setFontHeight(uint16 _height);
		virtual uint16 getFontHeight();*/

		//------------------------------------------------------------------------------------//
		// ��������������� ������ ��� ��������� �������
		void _setItemFocus(size_t _position, bool _focus);
		void _sendEventChangeScroll(size_t _position);

		//------------------------------------------------------------------------------------//
		virtual void setSize(const IntSize& _size);
		virtual void setPosition(const IntCoord& _coord);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// ���������� ������������ ������ ��������� ��� ������ � ������������ ������
		//! Return optimal height to fit all items in List
		inline int getOptimalHeight() {return (mCoord.height - mWidgetClient->getHeight()) + ((int)mStringArray.size() * mHeightLine);}

		/** Event : Enter pressed or double click.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			_index of selected item
		*/
		EventInfo_WidgetSizeT eventListSelectAccept;

		/** Event : Selected item position changed.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			_index of new item
		*/
		EventInfo_WidgetSizeT eventListChangePosition;

		/** Event : Item was selected by mouse.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			_index of selected item
		*/
		EventInfo_WidgetSizeT eventListMouseItemActivate;

		/** Event : Mouse is over item.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			_index of focused item
		*/
		EventInfo_WidgetSizeT eventListMouseItemFocus;

		/** Event : Position of scroll changed.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _position)\n
			_position of scroll
		*/
		EventInfo_WidgetSizeT eventListChangeScroll;

	protected:

		void _onMouseWheel(int _rel);
		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);
		void _onKeyButtonPressed(int _key, Char _char);

		void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _rel);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel);
		void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);

		void updateScroll();
		void updateLine(bool _reset = false);
		void _setScrollView(size_t _position);

		// �������������� �� ������� �� ����
		void _redrawItemRange(size_t _start = 0);

		// �������������� ������
		void _redrawItem(size_t _index);

		// ������� ������ �� ������
		void _deleteString(size_t _index);
		// ��������� ������
		void _insertString(size_t _index, const Ogre::DisplayString & _item);

		// ���� � �������� �������
		inline void _selectIndex(size_t _index, bool _select);

		inline void _updateState() {setState(mIsFocus ? "select" : "normal");}

	private:
		std::string mSkinLine;
		VScrollPtr mWidgetScroll;
		WidgetPtr mWidgetClient;

		// ���� ���� � �������
		VectorWidgetPtr mWidgetLines;

		int mHeightLine; // ������ ����� ������
		int mTopIndex; // ������ ������ �������� ��������
		int mOffsetTop; // ������� ��������
		int mRangeIndex; // ����������� ������
		size_t mLastRedrawLine; // ��������� �������������� �����

		size_t mIndexSelect; // ������� ���������� ������� ��� ITEM_NONE
		size_t mLineActive; // ������� ������ ��� ������� ����

		std::vector<Ogre::DisplayString> mStringArray;

		// ����� �� �� ����� �����
		bool mIsFocus;
		bool mNeedVisibleScroll;

		IntSize mOldSize;

	}; // class List : public Widget

} // namespace MyGUI

#endif // __MYGUI_LIST_H__
