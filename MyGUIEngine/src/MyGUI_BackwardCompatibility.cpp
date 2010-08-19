/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_BackwardCompatibility.h"
#include "MyGUI_Button.h"
#include "MyGUI_ComboBox.h"
#include "MyGUI_Edit.h"
#include "MyGUI_ItemBox.h"
#include "MyGUI_List.h"
#include "MyGUI_MenuCtrl.h"
#include "MyGUI_MenuItem.h"
#include "MyGUI_MultiList.h"
#include "MyGUI_Progress.h"
#include "MyGUI_ScrollView.h"
#include "MyGUI_Tab.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Window.h"

namespace MyGUI
{

#ifndef MYGUI_DONT_USE_OBSOLETE

	void WidgetObsolete<Button>::setButtonPressed(bool _value) { static_cast<Button*>(this)->setStateSelected(_value); }
	bool WidgetObsolete<Button>::getButtonPressed() { return static_cast<Button*>(this)->getStateSelected(); }
	void WidgetObsolete<Button>::setStateCheck(bool _value) { static_cast<Button*>(this)->setStateSelected(_value); }
	bool WidgetObsolete<Button>::getStateCheck() { return static_cast<Button*>(this)->getStateSelected(); }


	size_t WidgetObsolete<ComboBox>::getItemIndexSelected() { return static_cast<ComboBox*>(this)->getIndexSelected(); }
	void WidgetObsolete<ComboBox>::setItemSelectedAt(size_t _index) { static_cast<ComboBox*>(this)->setIndexSelected(_index); }
	void WidgetObsolete<ComboBox>::clearItemSelected() { static_cast<ComboBox*>(this)->clearIndexSelected(); }

	void WidgetObsolete<ComboBox>::insertItem(size_t _index, const UString& _name) { static_cast<ComboBox*>(this)->insertItemAt(_index, _name); }
	void WidgetObsolete<ComboBox>::setItem(size_t _index, const UString& _item) { static_cast<ComboBox*>(this)->setItemNameAt(_index, _item); }
	const UString& WidgetObsolete<ComboBox>::getItem(size_t _index) { return static_cast<ComboBox*>(this)->getItemNameAt(_index); }
	void WidgetObsolete<ComboBox>::deleteItem(size_t _index) { static_cast<ComboBox*>(this)->removeItemAt(_index); }
	void WidgetObsolete<ComboBox>::deleteAllItems() { static_cast<ComboBox*>(this)->removeAllItems(); }
	size_t WidgetObsolete<ComboBox>::getItemSelect() { return static_cast<ComboBox*>(this)->getIndexSelected(); }
	void WidgetObsolete<ComboBox>::resetItemSelect() { static_cast<ComboBox*>(this)->clearIndexSelected(); }
	void WidgetObsolete<ComboBox>::setItemSelect(size_t _index) { static_cast<ComboBox*>(this)->setIndexSelected(_index); }

	void WidgetObsolete<ComboBox>::setMaxListHeight(int _value) { static_cast<ComboBox*>(this)->setMaxListLength(_value); }
	int WidgetObsolete<ComboBox>::getMaxListHeight() { return static_cast<ComboBox*>(this)->getMaxListLength(); }


	void WidgetObsolete<Edit>::showVScroll(bool _visible) { static_cast<Edit*>(this)->setVisibleVScroll(_visible); }
	bool WidgetObsolete<Edit>::isShowVScroll() { return static_cast<Edit*>(this)->isVisibleVScroll(); }
	void WidgetObsolete<Edit>::showHScroll(bool _visible) { static_cast<Edit*>(this)->setVisibleHScroll(_visible); }
	bool WidgetObsolete<Edit>::isShowHScroll() { return static_cast<Edit*>(this)->isVisibleHScroll(); }

	//void WidgetObsolete<Edit>::setPosition(const IntCoord& _coord) { static_cast<Edit*>(this)->setCoord(_coord); }
	//void WidgetObsolete<Edit>::setPosition(int _left, int _top, int _width, int _height) { static_cast<Edit*>(this)->setCoord(_left, _top, _width, _height); }

	void WidgetObsolete<Edit>::setTextColour(size_t _start, size_t _count, const Colour& _colour) { static_cast<Edit*>(this)->setTextIntervalColour(_start, _count, _colour); }
	void WidgetObsolete<Edit>::getTextSelect(size_t& _start, size_t& _end) { _start = static_cast<Edit*>(this)->getTextSelectionStart(); _end = static_cast<Edit*>(this)->getTextSelectionEnd(); }
	UString WidgetObsolete<Edit>::getText(size_t _start, size_t _count) { return static_cast<Edit*>(this)->getTextInterval(_start, _count); }
	void WidgetObsolete<Edit>::setTextSelect(size_t _start, size_t _end) { static_cast<Edit*>(this)->setTextSelection(_start, _end); }
	void WidgetObsolete<Edit>::deleteTextSelect() { static_cast<Edit*>(this)->deleteTextSelection(); }
	UString WidgetObsolete<Edit>::getSelectedText() { return static_cast<Edit*>(this)->getTextSelection(); }
	bool WidgetObsolete<Edit>::isTextSelect() { return static_cast<Edit*>(this)->isTextSelection(); }
	void WidgetObsolete<Edit>::setTextSelectColour(const Colour& _colour) { static_cast<Edit*>(this)->setTextSelectionColour(_colour); }


	//void WidgetObsolete<ItemBox>::setPosition(const IntCoord& _coord) { static_cast<ItemBox*>(this)->setCoord(_coord); }
	//void WidgetObsolete<ItemBox>::setPosition(int _left, int _top, int _width, int _height) { static_cast<ItemBox*>(this)->setCoord(_left, _top, _width, _height); }

	size_t WidgetObsolete<ItemBox>::getItemIndexSelected() { return static_cast<ItemBox*>(this)->getIndexSelected(); }
	void WidgetObsolete<ItemBox>::setItemSelectedAt(size_t _index) { static_cast<ItemBox*>(this)->setIndexSelected(_index); }
	void WidgetObsolete<ItemBox>::clearItemSelected() { static_cast<ItemBox*>(this)->clearIndexSelected(); }

	void WidgetObsolete<ItemBox>::insertItem(size_t _index, Any _data) { static_cast<ItemBox*>(this)->insertItemAt(_index, _data); }
	void WidgetObsolete<ItemBox>::setItemData(size_t _index, Any _data) { static_cast<ItemBox*>(this)->setItemDataAt(_index, _data); }
	void WidgetObsolete<ItemBox>::deleteItem(size_t _index) { static_cast<ItemBox*>(this)->removeItemAt(_index); }
	void WidgetObsolete<ItemBox>::deleteAllItems() { static_cast<ItemBox*>(this)->removeAllItems(); }
	size_t WidgetObsolete<ItemBox>::getItemSelect() { return static_cast<ItemBox*>(this)->getIndexSelected(); }
	void WidgetObsolete<ItemBox>::resetItemSelect() { static_cast<ItemBox*>(this)->clearIndexSelected(); }
	void WidgetObsolete<ItemBox>::setItemSelect(size_t _index) { static_cast<ItemBox*>(this)->setIndexSelected(_index); }

	Widget* WidgetObsolete<ItemBox>::getWidgetDrop() { return static_cast<ItemBox*>(this)->getWidgetDrag(); }
	void WidgetObsolete<ItemBox>::resetDrop() { static_cast<ItemBox*>(this)->resetDrag(); }


	//void WidgetObsolete<List>::setPosition(const IntCoord& _coord) { static_cast<List*>(this)->setCoord(_coord); }
	//void WidgetObsolete<List>::setPosition(int _left, int _top, int _width, int _height) { static_cast<List*>(this)->setCoord(_left, _top, _width, _height); }

	size_t WidgetObsolete<List>::getItemIndexSelected() { return static_cast<List*>(this)->getIndexSelected(); }
	void WidgetObsolete<List>::setItemSelectedAt(size_t _index) { static_cast<List*>(this)->setIndexSelected(_index); }
	void WidgetObsolete<List>::clearItemSelected() { static_cast<List*>(this)->clearIndexSelected(); }

	void WidgetObsolete<List>::insertItem(size_t _index, const UString& _item) { static_cast<List*>(this)->insertItemAt(_index, _item); }
	void WidgetObsolete<List>::setItem(size_t _index, const UString& _item) { static_cast<List*>(this)->setItemNameAt(_index, _item); }
	const UString& WidgetObsolete<List>::getItem(size_t _index) { return static_cast<List*>(this)->getItemNameAt(_index); }
	void WidgetObsolete<List>::deleteItem(size_t _index) { static_cast<List*>(this)->removeItemAt(_index); }
	void WidgetObsolete<List>::deleteAllItems() { static_cast<List*>(this)->removeAllItems(); }
	size_t WidgetObsolete<List>::findItem(const UString& _item) { return static_cast<List*>(this)->findItemIndexWith(_item); }
	size_t WidgetObsolete<List>::getItemSelect() { return static_cast<List*>(this)->getIndexSelected(); }
	void WidgetObsolete<List>::resetItemSelect() { static_cast<List*>(this)->clearIndexSelected(); }
	void WidgetObsolete<List>::setItemSelect(size_t _index) { static_cast<List*>(this)->setIndexSelected(_index); }
	void WidgetObsolete<List>::beginToIndex(size_t _index) { static_cast<List*>(this)->beginToItemAt(_index); }
	void WidgetObsolete<List>::beginToStart() { static_cast<List*>(this)->beginToItemFirst(); }
	void WidgetObsolete<List>::beginToEnd() { static_cast<List*>(this)->beginToItemLast(); }
	void WidgetObsolete<List>::beginToSelect() { static_cast<List*>(this)->beginToItemSelected(); }
	bool WidgetObsolete<List>::isItemVisible(size_t _index, bool _fill) { return static_cast<List*>(this)->isItemVisibleAt(_index, _fill); }
	bool WidgetObsolete<List>::isItemSelectVisible(bool _fill) { return static_cast<List*>(this)->isItemSelectedVisible(_fill); }


	void WidgetObsolete<MenuCtrl>::showMenu() { static_cast<MenuCtrl*>(this)->setVisible(true); }
	void WidgetObsolete<MenuCtrl>::hideMenu() { static_cast<MenuCtrl*>(this)->setVisible(false); }
	bool WidgetObsolete<MenuCtrl>::isShowMenu() { return static_cast<MenuCtrl*>(this)->getVisible(); }

	void WidgetObsolete<MenuCtrl>::showItemChildAt(size_t _index) { static_cast<MenuCtrl*>(this)->setItemChildVisibleAt(_index, true); }
	void WidgetObsolete<MenuCtrl>::showItemChild(MenuItem* _item) { static_cast<MenuCtrl*>(this)->setItemChildVisible(_item, true); }
	void WidgetObsolete<MenuCtrl>::hideItemChildAt(size_t _index) { static_cast<MenuCtrl*>(this)->setItemChildVisibleAt(_index, false); }
	void WidgetObsolete<MenuCtrl>::hideItemChild(MenuItem* _item) { static_cast<MenuCtrl*>(this)->setItemChildVisible(_item, false); }


	void WidgetObsolete<MenuItem>::showItemChild() { static_cast<MenuItem*>(this)->setItemChildVisible(true); }
	void WidgetObsolete<MenuItem>::hideItemChild() { static_cast<MenuItem*>(this)->setItemChildVisible(false); }


	size_t WidgetObsolete<MultiList>::getItemIndexSelected() { return static_cast<MultiList*>(this)->getIndexSelected(); }
	void WidgetObsolete<MultiList>::setItemSelectedAt(size_t _index) { static_cast<MultiList*>(this)->setIndexSelected(_index); }
	void WidgetObsolete<MultiList>::clearItemSelected() { static_cast<MultiList*>(this)->clearIndexSelected(); }

	size_t WidgetObsolete<MultiList>::findItem(size_t _column, const UString& _name) { return static_cast<MultiList*>(this)->findSubItemWith(_column, _name); }
	const UString& WidgetObsolete<MultiList>::getSubItem(size_t _column, size_t _index) { return static_cast<MultiList*>(this)->getSubItemNameAt(_column, _index); }
	void WidgetObsolete<MultiList>::setSubItem(size_t _column, size_t _index, const UString& _name) { static_cast<MultiList*>(this)->setSubItemNameAt(_column, _index, _name); }
	void WidgetObsolete<MultiList>::deleteColumn(size_t _column) { static_cast<MultiList*>(this)->removeColumnAt(_column); }
	void WidgetObsolete<MultiList>::deleteAllColumns() { static_cast<MultiList*>(this)->removeAllColumns(); }
	int WidgetObsolete<MultiList>::getColumnWidth(size_t _column) { return static_cast<MultiList*>(this)->getColumnWidthAt(_column); }
	const UString& WidgetObsolete<MultiList>::getColumnName(size_t _column) { return static_cast<MultiList*>(this)->getColumnNameAt(_column); }
	void WidgetObsolete<MultiList>::setColumnWidth(size_t _column, int _width) { static_cast<MultiList*>(this)->setColumnWidthAt(_column, _width); }
	void WidgetObsolete<MultiList>::addColumn(int _width, const UString& _name) { static_cast<MultiList*>(this)->addColumn(_name, _width); }
	void WidgetObsolete<MultiList>::setColumnName(size_t _column, const UString& _name) { static_cast<MultiList*>(this)->setColumnNameAt(_column, _name); }
	void WidgetObsolete<MultiList>::insertColumn(size_t _column, int _width, const UString& _name) { static_cast<MultiList*>(this)->insertColumnAt(_column, _name, _width); }
	size_t WidgetObsolete<MultiList>::getItemSelect() { return static_cast<MultiList*>(this)->getIndexSelected(); }
	void WidgetObsolete<MultiList>::resetItemSelect() { static_cast<MultiList*>(this)->clearIndexSelected(); }
	void WidgetObsolete<MultiList>::setItemSelect(size_t _index) { static_cast<MultiList*>(this)->setIndexSelected(_index); }
	void WidgetObsolete<MultiList>::insertItem(size_t _index, const UString& _name) { static_cast<MultiList*>(this)->insertItemAt(_index, _name); }
	void WidgetObsolete<MultiList>::setItem(size_t _index, const UString& _name) { static_cast<MultiList*>(this)->setItemNameAt(_index, _name); }
	const UString& WidgetObsolete<MultiList>::getItem(size_t _index) { return static_cast<MultiList*>(this)->getItemNameAt(_index); }
	void WidgetObsolete<MultiList>::deleteItem(size_t _index) { static_cast<MultiList*>(this)->removeItemAt(_index); }
	void WidgetObsolete<MultiList>::deleteAllItems() { static_cast<MultiList*>(this)->removeAllItems(); }


	//void WidgetObsolete<Progress>::setPosition(const IntCoord& _coord) { static_cast<Progress*>(this)->setCoord(_coord); }
	//void WidgetObsolete<Progress>::setPosition(int _left, int _top, int _width, int _height) { static_cast<Progress*>(this)->setCoord(_left, _top, _width, _height); }

	void WidgetObsolete<Progress>::setProgressStartPoint(Align _value)
	{
		if (_value == Align::Right)
			static_cast<Progress*>(this)->setFlowDirection(FlowDirection::RightToLeft);
		else if (_value == Align::Top)
			static_cast<Progress*>(this)->setFlowDirection(FlowDirection::TopToBottom);
		else if (_value == Align::Bottom)
			static_cast<Progress*>(this)->setFlowDirection(FlowDirection::BottomToTop);
		else
			static_cast<Progress*>(this)->setFlowDirection(FlowDirection::LeftToRight);
	}

	Align WidgetObsolete<Progress>::getProgressStartPoint()
	{
		if (static_cast<Progress*>(this)->getFlowDirection() == FlowDirection::RightToLeft)
			return Align::Right;
		else if (static_cast<Progress*>(this)->getFlowDirection() == FlowDirection::TopToBottom)
			return Align::Top;
		else if (static_cast<Progress*>(this)->getFlowDirection() == FlowDirection::BottomToTop)
			return Align::Bottom;

		return Align::Left;
	}


	//void WidgetObsolete<ScrollView>::setPosition(const IntCoord& _coord) { static_cast<ScrollView*>(this)->setCoord(_coord); }
	//void WidgetObsolete<ScrollView>::setPosition(int _left, int _top, int _width, int _height) { static_cast<ScrollView*>(this)->setCoord(_left, _top, _width, _height); }

	void WidgetObsolete<ScrollView>::showVScroll(bool _visible) { static_cast<ScrollView*>(this)->setVisibleVScroll(_visible); }
	bool WidgetObsolete<ScrollView>::isShowVScroll() { return static_cast<ScrollView*>(this)->isVisibleVScroll(); }
	void WidgetObsolete<ScrollView>::showHScroll(bool _visible) { static_cast<ScrollView*>(this)->setVisibleHScroll(_visible); }
	bool WidgetObsolete<ScrollView>::isShowHScroll() { return static_cast<ScrollView*>(this)->isVisibleHScroll(); }


	//void WidgetObsolete<Tab>::setPosition(const IntCoord& _coord) { static_cast<Tab*>(this)->setCoord(_coord); }
	//void WidgetObsolete<Tab>::setPosition(int _left, int _top, int _width, int _height) { static_cast<Tab*>(this)->setCoord(_left, _top, _width, _height); }

	int WidgetObsolete<Tab>::getSheetButtonWidthIndex(size_t _index) { return static_cast<Tab*>(this)->getButtonWidthAt(_index); }
	int WidgetObsolete<Tab>::getSheetButtonWidth(TabItem* _sheet) { return static_cast<Tab*>(this)->getButtonWidth(_sheet); }
	void WidgetObsolete<Tab>::setSheetButtonWidthIndex(size_t _index, int _width) { static_cast<Tab*>(this)->setButtonWidthAt(_index, _width); }
	void WidgetObsolete<Tab>::setSheetButtonWidth(TabItem* _sheet, int _width) { static_cast<Tab*>(this)->setButtonWidth(_sheet, _width); }
	void WidgetObsolete<Tab>::showBarButton(size_t _index) { static_cast<Tab*>(this)->beginToItemAt(_index); }
	void WidgetObsolete<Tab>::showBarSelectButton() { static_cast<Tab*>(this)->beginToItemSelected(); }
	size_t WidgetObsolete<Tab>::getSheetCount() { return static_cast<Tab*>(this)->getItemCount(); }
	const UString& WidgetObsolete<Tab>::getSheetName(TabItem* _sheet) { return static_cast<Tab*>(this)->getItemName(_sheet); }
	const UString& WidgetObsolete<Tab>::getSheetNameIndex(size_t _index) { return static_cast<Tab*>(this)->getItemNameAt(_index); }
	TabItem* WidgetObsolete<Tab>::getSheet(size_t _index) { return static_cast<Tab*>(this)->getItemAt(_index); }
	void WidgetObsolete<Tab>::setSheetNameIndex(size_t _index, const UString& _name, int _width) { static_cast<Tab*>(this)->setItemNameAt(_index, _name); }
	void WidgetObsolete<Tab>::setSheetName(TabItem* _sheet, const UString& _name, int _width) { static_cast<Tab*>(this)->setItemName(_sheet, _name); }
	TabItem* WidgetObsolete<Tab>::addSheet(const UString& _name, int _width) { return static_cast<Tab*>(this)->addItem(_name, _width); }
	TabItem* WidgetObsolete<Tab>::insertSheet(size_t _index, const UString& _name, int _width) { return static_cast<Tab*>(this)->insertItemAt(_index, _name); }
	void WidgetObsolete<Tab>::removeSheetIndex(size_t _index) { static_cast<Tab*>(this)->removeItemAt(_index); }
	void WidgetObsolete<Tab>::removeSheet(TabItem* _sheet) { static_cast<Tab*>(this)->removeItem(_sheet); }
	void WidgetObsolete<Tab>::selectSheetIndex(size_t _index, bool _smooth) { static_cast<Tab*>(this)->setIndexSelected(_index); }
	void WidgetObsolete<Tab>::selectSheet(TabItem* _sheet, bool _smooth) { static_cast<Tab*>(this)->setItemSelected(_sheet); }
	size_t WidgetObsolete<Tab>::getSelectSheetIndex() { return static_cast<Tab*>(this)->getIndexSelected(); }

	size_t WidgetObsolete<Tab>::getItemIndexSelected() { return static_cast<Tab*>(this)->getIndexSelected(); }
	void WidgetObsolete<Tab>::setItemSelectedAt(size_t _index) { static_cast<Tab*>(this)->setIndexSelected(_index); }

	//void WidgetObsolete<VScroll>::setPosition(const IntCoord& _coord) { static_cast<VScroll*>(this)->setCoord(_coord); }
	//void WidgetObsolete<VScroll>::setPosition(int _left, int _top, int _width, int _height) { static_cast<VScroll*>(this)->setCoord(_left, _top, _width, _height); }


	//void WidgetObsolete<Widget>::setPosition(const IntCoord& _coord) { static_cast<Widget*>(this)->setCoord(_coord); }
	//void WidgetObsolete<Widget>::setPosition(int _left, int _top, int _width, int _height) { static_cast<Widget*>(this)->setCoord(_left, _top, _width, _height); }

	bool WidgetObsolete<Widget>::isEnabled() { return static_cast<Widget*>(this)->getEnabled(); }
	bool WidgetObsolete<Widget>::isInheritsAlpha() { return static_cast<Widget*>(this)->getInheritsAlpha(); }
	bool WidgetObsolete<Widget>::isNeedKeyFocus() { return static_cast<Widget*>(this)->getNeedKeyFocus(); }
	bool WidgetObsolete<Widget>::isNeedMouseFocus() { return static_cast<Widget*>(this)->getNeedMouseFocus(); }
	bool WidgetObsolete<Widget>::isInheritsPick() { return static_cast<Widget*>(this)->getInheritsPick(); }
	bool WidgetObsolete<Widget>::isVisible() { return static_cast<Widget*>(this)->getVisible(); }


	//void WidgetObsolete<Window>::setPosition(const IntCoord& _coord) { static_cast<Window*>(this)->setCoord(_coord); }
	//void WidgetObsolete<Window>::setPosition(int _left, int _top, int _width, int _height) { static_cast<Window*>(this)->setCoord(_left, _top, _width, _height); }

	void WidgetObsolete<Window>::showSmooth(bool _reset) { static_cast<Window*>(this)->setVisibleSmooth(true); }
	void WidgetObsolete<Window>::hideSmooth() { static_cast<Window*>(this)->setVisibleSmooth(false); }
	void WidgetObsolete<Window>::setMinMax(const IntRect& _minmax) { static_cast<Window*>(this)->setMinSize(_minmax.left, _minmax.top); static_cast<Window*>(this)->setMaxSize(_minmax.right, _minmax.bottom); }
	void WidgetObsolete<Window>::setMinMax(int _min_w, int _min_h, int _max_w, int _max_h) { static_cast<Window*>(this)->setMinSize(_min_w, _min_h); static_cast<Window*>(this)->setMaxSize(_max_w, _max_h); }
	IntRect WidgetObsolete<Window>::getMinMax() { return IntRect(static_cast<Window*>(this)->getMinSize().width, static_cast<Window*>(this)->getMinSize().height, static_cast<Window*>(this)->getMaxSize().width, static_cast<Window*>(this)->getMaxSize().height); }

#endif // MYGUI_DONT_USE_OBSOLETE

} // namespace MyGUI
