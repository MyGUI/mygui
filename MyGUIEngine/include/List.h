#ifndef _LIST_H_
#define _LIST_H_

#include "Widget.h"
#include "VScroll.h"
#include "Button.h"

namespace MyGUI
{

	class _MyGUIExport List : public Widget
	{
		// для вызова закрытого конструктора
		friend WidgetFactory<List>;

	protected:
		List(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("List"); return type;};

		inline size_t getItemCount() {return mStringArray.size();}
		inline const Ogre::DisplayString & getItemString(size_t _index) {assert(_index < mStringArray.size()); return mStringArray[_index];}
		inline void setItemString(size_t _index, const Ogre::DisplayString & _item) {assert(_index < mStringArray.size()); mStringArray[_index]=_item; _redrawItem(_index);}
		void insertItemString(size_t _index, const Ogre::DisplayString & _item);
		inline void addItemString(const Ogre::DisplayString & _item) {insertItemString(ITEM_NONE, _item);}
		void deleteItemString(size_t _index);

		inline size_t getItemSelect() {return mIndexSelect;}
		inline void resetItemSelect() {setItemSelect(ITEM_NONE);}
		void setItemSelect(size_t _index);


	protected:
		void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, int _rel);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);

		virtual void size(int _cx, int _cy);
		virtual void move(int _x, int _y, int _cx, int _cy);

		void updateScroll();
		void updateLine(bool _reset = false);

		// перерисовывает от индекса до низа
		void _redrawItemRange(size_t _start = 0);

		// перерисовывает индекс
		void _redrawItem(size_t _index);

		// удаляем строку из списка
		void _deleteString(size_t _index);
		// вставляем строку
		void _insertString(size_t _index, const Ogre::DisplayString & _item);

		// ищет и выделяет елемент
		inline void _selectIndex(size_t _index, bool _select);

	private:
		std::string mSkinLine;
		VScrollPtr mWidgetScroll;
		WidgetPtr mWidgetClient;

		// наши дети в строках
		WidgetChild mWidgetLines;

		int mHeightLine; // высота одной строки
		int mTopIndex; // индекс самого верхнего элемента
		int mOffsetTop; // текущее смещение
		int mRangeIndex; // размерность скрола
		size_t mLastRedrawLine; // последняя перерисованная линия

		size_t mIndexSelect; // текущий выделенный элемент или ITEM_NONE

		std::vector<Ogre::DisplayString> mStringArray;

	}; // class List : public Widget

	typedef List * ListPtr;

} // namespace MyGUI

#endif // _LIST_H_