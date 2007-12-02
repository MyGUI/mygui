/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LIST_H__
#define __MYGUI_LIST_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ListFactory.h"
#include "MyGUI_Widget.h"
#include "MyGUI_VScroll.h"

namespace MyGUI
{

	class List;
	typedef List * ListPtr;

	class _MyGUIExport List : public Widget
	{
		// ��� ������ ��������� ������������
		friend factory::ListFactory;

	protected:
		List(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:

		inline size_t getItemCount() {return mStringArray.size();}
		inline const Ogre::DisplayString & getItemString(size_t _index)
		{
			MYGUI_ASSERT(_index < mStringArray.size());
			return mStringArray[_index];
		}
		inline void setItemString(size_t _index, const Ogre::DisplayString & _item)
		{
			MYGUI_ASSERT(_index < mStringArray.size());
			mStringArray[_index]=_item; _redrawItem(_index);
		}
		void insertItemString(size_t _index, const Ogre::DisplayString & _item);
		inline void addItemString(const Ogre::DisplayString & _item){insertItemString(ITEM_NONE, _item);}
		void deleteItemString(size_t _index);

		inline size_t getItemSelect() {return mIndexSelect;}
		inline void resetItemSelect() {setItemSelect(ITEM_NONE);}
		void setItemSelect(size_t _index);

		// ������ ��� ������ �����
		void beginToIndex(size_t _index);
		inline void beginToStart() {beginToIndex(0);}
		inline void beginToEnd()
		{
			if (!mStringArray.empty()) beginToIndex(mStringArray.size()-1);
		}
		inline void beginToSelect() {beginToIndex(mIndexSelect);}

		// ����� �� �� �������, ��������� ��� ���
		bool isItemVisible(size_t _index, bool _fill = true);
		inline bool isItemSelectVisible(bool _fill = true) {return isItemVisible(mIndexSelect, _fill);}

		virtual void setSize(int _width, int _height);
		virtual void setPosition(int _left, int _top, int _width, int _height);

	protected:

		void _onMouseSheel(int _rel);
		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);
		void _onKeyButtonPressed(int _key, wchar_t _char);

		void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, int _rel);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseSheel(MyGUI::WidgetPtr _sender, int _rel);

		void updateScroll();
		void updateLine(bool _reset = false);

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

		std::vector<Ogre::DisplayString> mStringArray;

		// ����� �� �� ����� �����
		bool mIsFocus;
		int mOldCx, mOldCy;

	}; // class List : public Widget

} // namespace MyGUI

#endif // __MYGUI_LIST_H__