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

	typedef delegates::CDelegate2<WidgetPtr, size_t> EventListChangePositionInfo;

	class _MyGUIExport List : public Widget
	{
		// дл€ вызова закрытого конструктора
		friend factory::ListFactory;

	protected:
		List(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// тип данного виджета
		inline static const Ogre::String & getType() {static Ogre::String type("List"); return type;}

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

		// методы дл€ показа строк
		void beginToIndex(size_t _index);
		inline void beginToStart() {beginToIndex(0);}
		inline void beginToEnd()
		{
			if (!mStringArray.empty()) beginToIndex(mStringArray.size()-1);
		}
		inline void beginToSelect() {beginToIndex(mIndexSelect);}

		// видим ли мы элемент, полностью или нет
		bool isItemVisible(size_t _index, bool _fill = true);
		inline bool isItemSelectVisible(bool _fill = true) {return isItemVisible(mIndexSelect, _fill);}

		virtual void setSize(const IntSize& _size);
		virtual void setPosition(const IntCoord& _coord);

		// возвращает максимальную высоту вмещающую все строки и радительский бордюр
		inline int getListMaxHeight() {return (mCoord.height - mWidgetClient->getHeight()) + ((int)mStringArray.size() * mHeightLine);}


		// event : нажат ентер, или двойной щелчек
		// signature : void method(MyGUI::WidgetPtr _widget)
		EventSimple eventListSelectAccept;

		// event : нажат ентер, или двойной щелчек
		// signature : void method(MyGUI::WidgetPtr _widget)
		//EventSimple eventListSelectMouseAccept;

		// event : нажат ƒел на елементе
		// signature : void method(MyGUI::WidgetPtr _widget)
		EventSimple eventListPressedDelete;

		// event : изменилась позици€
		// signature : void method(MyGUI::WidgetPtr _widgetm size_t _position)
		EventListChangePositionInfo eventListChangePosition;

		// event : мышью выделен элемент
		// signature : void method(MyGUI::WidgetPtr _widgetm size_t _position)
		EventListChangePositionInfo eventListMouseChangePosition;

	protected:

		void _onMouseWheel(int _rel);
		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);
		void _onKeyButtonPressed(int _key, wchar_t _char);

		void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, int _rel);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel);

		void updateScroll();
		void updateLine(bool _reset = false);

		// перерисовывает от индекса до низа
		void _redrawItemRange(size_t _start = 0);

		// перерисовывает индекс
		void _redrawItem(size_t _index);

		// удал€ем строку из списка
		void _deleteString(size_t _index);
		// вставл€ем строку
		void _insertString(size_t _index, const Ogre::DisplayString & _item);

		// ищет и выдел€ет елемент
		inline void _selectIndex(size_t _index, bool _select);

		inline void _updateState() {setState(mIsFocus ? "select" : "normal");}

	private:
		std::string mSkinLine;
		VScrollPtr mWidgetScroll;
		WidgetPtr mWidgetClient;

		// наши дети в строках
		VectorWidgetPtr mWidgetLines;

		int mHeightLine; // высота одной строки
		int mTopIndex; // индекс самого верхнего элемента
		int mOffsetTop; // текущее смещение
		int mRangeIndex; // размерность скрола
		size_t mLastRedrawLine; // последн€€ перерисованна€ лини€

		size_t mIndexSelect; // текущий выделенный элемент или ITEM_NONE

		std::vector<Ogre::DisplayString> mStringArray;

		// имеем ли мы фокус ввода
		bool mIsFocus;

		IntSize mOldSize;

	}; // class List : public Widget

} // namespace MyGUI

#endif // __MYGUI_LIST_H__