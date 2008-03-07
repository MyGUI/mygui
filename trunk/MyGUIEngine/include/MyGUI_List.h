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
		List(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// ��� ������� �������
		inline static const Ogre::String & _getType() {static Ogre::String type("List"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		//------------------------------------------------------------------------------------//
		// ������ ��� ����������� ��������
		inline size_t getItemCount() { return mStringArray.size();}

		void insertItem(size_t _index, const Ogre::DisplayString & _item);
		inline void addItem(const Ogre::DisplayString & _item) {insertItem(ITEM_NONE, _item);}
		void setItem(size_t _index, const Ogre::DisplayString & _item);
		const Ogre::DisplayString & getItem(size_t _index);

		void deleteItem(size_t _index);
		void deleteAllItems();

		inline size_t getItemSelect() {return mIndexSelect;}
		inline void resetItemSelect() {setItemSelect(ITEM_NONE);}
		void setItemSelect(size_t _index);


		//------------------------------------------------------------------------------------//
		// ������ ��� ������ �����
		void beginToIndex(size_t _index);
		inline void beginToStart() { beginToIndex(0); }
		inline void beginToEnd() { if (!mStringArray.empty()) beginToIndex(mStringArray.size()-1); }
		inline void beginToSelect() { beginToIndex(mIndexSelect); }

		// ����� �� �� �������, ��������� ��� ���
		bool isItemVisible(size_t _index, bool _fill = true);
		inline bool isItemSelectVisible(bool _fill = true) {return isItemVisible(mIndexSelect, _fill);}

		void needVisibleScroll(bool _visible);
		void setScrollPosition(size_t _position);

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
		inline int getListMaxHeight() {return (mCoord.height - mWidgetClient->getHeight()) + ((int)mStringArray.size() * mHeightLine);}

		// event : ����� �����, ��� ������� ������
		// signature : void method(MyGUI::WidgetPtr _widget)
		EventInfo_WidgetVoid eventListSelectAccept;

		// event : ���������� �������
		// signature : void method(MyGUI::WidgetPtr _widget, size_t _position)
		EventInfo_WidgetSizeT eventListChangePosition;

		// event : ����� ������� �������
		// signature : void method(MyGUI::WidgetPtr _widget, size_t _position)
		EventInfo_WidgetSizeT eventListMouseItemActivate;

		// event : ��� ��������� ���������� ����
		// signature : void method(MyGUI::WidgetPtr _widget, size_t _position)
		EventInfo_WidgetSizeT eventListMouseItemFocus;

		// event : ���������� ��������� ������� ���
		// signature : void method(MyGUI::WidgetPtr _widget, size_t _position)
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
