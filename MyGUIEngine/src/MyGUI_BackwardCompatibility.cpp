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
#include "MyGUI_TabItem.h"
#include "MyGUI_Canvas.h"
#include "MyGUI_Message.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_PluginManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_FactoryManager.h"

namespace MyGUI
{

#ifndef MYGUI_DONT_USE_OBSOLETE

	void MemberObsolete<Button>::setButtonPressed(bool _value) { static_cast<Button*>(this)->setStateSelected(_value); }
	bool MemberObsolete<Button>::getButtonPressed() { return static_cast<Button*>(this)->getStateSelected(); }
	void MemberObsolete<Button>::setStateCheck(bool _value) { static_cast<Button*>(this)->setStateSelected(_value); }
	bool MemberObsolete<Button>::getStateCheck() { return static_cast<Button*>(this)->getStateSelected(); }


	size_t MemberObsolete<ComboBox>::getItemIndexSelected() { return static_cast<ComboBox*>(this)->getIndexSelected(); }
	void MemberObsolete<ComboBox>::setItemSelectedAt(size_t _index) { static_cast<ComboBox*>(this)->setIndexSelected(_index); }
	void MemberObsolete<ComboBox>::clearItemSelected() { static_cast<ComboBox*>(this)->clearIndexSelected(); }

	void MemberObsolete<ComboBox>::insertItem(size_t _index, const UString& _name) { static_cast<ComboBox*>(this)->insertItemAt(_index, _name); }
	void MemberObsolete<ComboBox>::setItem(size_t _index, const UString& _item) { static_cast<ComboBox*>(this)->setItemNameAt(_index, _item); }
	const UString& MemberObsolete<ComboBox>::getItem(size_t _index) { return static_cast<ComboBox*>(this)->getItemNameAt(_index); }
	void MemberObsolete<ComboBox>::deleteItem(size_t _index) { static_cast<ComboBox*>(this)->removeItemAt(_index); }
	void MemberObsolete<ComboBox>::deleteAllItems() { static_cast<ComboBox*>(this)->removeAllItems(); }
	size_t MemberObsolete<ComboBox>::getItemSelect() { return static_cast<ComboBox*>(this)->getIndexSelected(); }
	void MemberObsolete<ComboBox>::resetItemSelect() { static_cast<ComboBox*>(this)->clearIndexSelected(); }
	void MemberObsolete<ComboBox>::setItemSelect(size_t _index) { static_cast<ComboBox*>(this)->setIndexSelected(_index); }

	void MemberObsolete<ComboBox>::setMaxListHeight(int _value) { static_cast<ComboBox*>(this)->setMaxListLength(_value); }
	int MemberObsolete<ComboBox>::getMaxListHeight() { return static_cast<ComboBox*>(this)->getMaxListLength(); }


	void MemberObsolete<Edit>::showVScroll(bool _visible) { static_cast<Edit*>(this)->setVisibleVScroll(_visible); }
	bool MemberObsolete<Edit>::isShowVScroll() { return static_cast<Edit*>(this)->isVisibleVScroll(); }
	void MemberObsolete<Edit>::showHScroll(bool _visible) { static_cast<Edit*>(this)->setVisibleHScroll(_visible); }
	bool MemberObsolete<Edit>::isShowHScroll() { return static_cast<Edit*>(this)->isVisibleHScroll(); }

	//void MemberObsolete<Edit>::setPosition(const IntCoord& _coord) { static_cast<Edit*>(this)->setCoord(_coord); }
	//void MemberObsolete<Edit>::setPosition(int _left, int _top, int _width, int _height) { static_cast<Edit*>(this)->setCoord(_left, _top, _width, _height); }

	void MemberObsolete<Edit>::setTextColour(size_t _start, size_t _count, const Colour& _colour) { static_cast<Edit*>(this)->setTextIntervalColour(_start, _count, _colour); }
	void MemberObsolete<Edit>::getTextSelect(size_t& _start, size_t& _end) { _start = static_cast<Edit*>(this)->getTextSelectionStart(); _end = static_cast<Edit*>(this)->getTextSelectionEnd(); }
	UString MemberObsolete<Edit>::getText(size_t _start, size_t _count) { return static_cast<Edit*>(this)->getTextInterval(_start, _count); }
	void MemberObsolete<Edit>::setTextSelect(size_t _start, size_t _end) { static_cast<Edit*>(this)->setTextSelection(_start, _end); }
	void MemberObsolete<Edit>::deleteTextSelect() { static_cast<Edit*>(this)->deleteTextSelection(); }
	UString MemberObsolete<Edit>::getSelectedText() { return static_cast<Edit*>(this)->getTextSelection(); }
	bool MemberObsolete<Edit>::isTextSelect() { return static_cast<Edit*>(this)->isTextSelection(); }
	void MemberObsolete<Edit>::setTextSelectColour(const Colour& _colour) { static_cast<Edit*>(this)->setTextSelectionColour(_colour); }


	//void MemberObsolete<ItemBox>::setPosition(const IntCoord& _coord) { static_cast<ItemBox*>(this)->setCoord(_coord); }
	//void MemberObsolete<ItemBox>::setPosition(int _left, int _top, int _width, int _height) { static_cast<ItemBox*>(this)->setCoord(_left, _top, _width, _height); }

	size_t MemberObsolete<ItemBox>::getItemIndexSelected() { return static_cast<ItemBox*>(this)->getIndexSelected(); }
	void MemberObsolete<ItemBox>::setItemSelectedAt(size_t _index) { static_cast<ItemBox*>(this)->setIndexSelected(_index); }
	void MemberObsolete<ItemBox>::clearItemSelected() { static_cast<ItemBox*>(this)->clearIndexSelected(); }

	void MemberObsolete<ItemBox>::insertItem(size_t _index, Any _data) { static_cast<ItemBox*>(this)->insertItemAt(_index, _data); }
	void MemberObsolete<ItemBox>::setItemData(size_t _index, Any _data) { static_cast<ItemBox*>(this)->setItemDataAt(_index, _data); }
	void MemberObsolete<ItemBox>::deleteItem(size_t _index) { static_cast<ItemBox*>(this)->removeItemAt(_index); }
	void MemberObsolete<ItemBox>::deleteAllItems() { static_cast<ItemBox*>(this)->removeAllItems(); }
	size_t MemberObsolete<ItemBox>::getItemSelect() { return static_cast<ItemBox*>(this)->getIndexSelected(); }
	void MemberObsolete<ItemBox>::resetItemSelect() { static_cast<ItemBox*>(this)->clearIndexSelected(); }
	void MemberObsolete<ItemBox>::setItemSelect(size_t _index) { static_cast<ItemBox*>(this)->setIndexSelected(_index); }

	Widget* MemberObsolete<ItemBox>::getWidgetDrop() { return static_cast<ItemBox*>(this)->getWidgetDrag(); }
	void MemberObsolete<ItemBox>::resetDrop() { static_cast<ItemBox*>(this)->resetDrag(); }


	//void MemberObsolete<List>::setPosition(const IntCoord& _coord) { static_cast<List*>(this)->setCoord(_coord); }
	//void MemberObsolete<List>::setPosition(int _left, int _top, int _width, int _height) { static_cast<List*>(this)->setCoord(_left, _top, _width, _height); }

	size_t MemberObsolete<List>::getItemIndexSelected() { return static_cast<List*>(this)->getIndexSelected(); }
	void MemberObsolete<List>::setItemSelectedAt(size_t _index) { static_cast<List*>(this)->setIndexSelected(_index); }
	void MemberObsolete<List>::clearItemSelected() { static_cast<List*>(this)->clearIndexSelected(); }

	void MemberObsolete<List>::insertItem(size_t _index, const UString& _item) { static_cast<List*>(this)->insertItemAt(_index, _item); }
	void MemberObsolete<List>::setItem(size_t _index, const UString& _item) { static_cast<List*>(this)->setItemNameAt(_index, _item); }
	const UString& MemberObsolete<List>::getItem(size_t _index) { return static_cast<List*>(this)->getItemNameAt(_index); }
	void MemberObsolete<List>::deleteItem(size_t _index) { static_cast<List*>(this)->removeItemAt(_index); }
	void MemberObsolete<List>::deleteAllItems() { static_cast<List*>(this)->removeAllItems(); }
	size_t MemberObsolete<List>::findItem(const UString& _item) { return static_cast<List*>(this)->findItemIndexWith(_item); }
	size_t MemberObsolete<List>::getItemSelect() { return static_cast<List*>(this)->getIndexSelected(); }
	void MemberObsolete<List>::resetItemSelect() { static_cast<List*>(this)->clearIndexSelected(); }
	void MemberObsolete<List>::setItemSelect(size_t _index) { static_cast<List*>(this)->setIndexSelected(_index); }
	void MemberObsolete<List>::beginToIndex(size_t _index) { static_cast<List*>(this)->beginToItemAt(_index); }
	void MemberObsolete<List>::beginToStart() { static_cast<List*>(this)->beginToItemFirst(); }
	void MemberObsolete<List>::beginToEnd() { static_cast<List*>(this)->beginToItemLast(); }
	void MemberObsolete<List>::beginToSelect() { static_cast<List*>(this)->beginToItemSelected(); }
	bool MemberObsolete<List>::isItemVisible(size_t _index, bool _fill) { return static_cast<List*>(this)->isItemVisibleAt(_index, _fill); }
	bool MemberObsolete<List>::isItemSelectVisible(bool _fill) { return static_cast<List*>(this)->isItemSelectedVisible(_fill); }


	void MemberObsolete<MenuCtrl>::showMenu() { static_cast<MenuCtrl*>(this)->setVisible(true); }
	void MemberObsolete<MenuCtrl>::hideMenu() { static_cast<MenuCtrl*>(this)->setVisible(false); }
	bool MemberObsolete<MenuCtrl>::isShowMenu() { return static_cast<MenuCtrl*>(this)->getVisible(); }

	void MemberObsolete<MenuCtrl>::showItemChildAt(size_t _index) { static_cast<MenuCtrl*>(this)->setItemChildVisibleAt(_index, true); }
	void MemberObsolete<MenuCtrl>::showItemChild(MenuItem* _item) { static_cast<MenuCtrl*>(this)->setItemChildVisible(_item, true); }
	void MemberObsolete<MenuCtrl>::hideItemChildAt(size_t _index) { static_cast<MenuCtrl*>(this)->setItemChildVisibleAt(_index, false); }
	void MemberObsolete<MenuCtrl>::hideItemChild(MenuItem* _item) { static_cast<MenuCtrl*>(this)->setItemChildVisible(_item, false); }


	void MemberObsolete<MenuItem>::showItemChild() { static_cast<MenuItem*>(this)->setItemChildVisible(true); }
	void MemberObsolete<MenuItem>::hideItemChild() { static_cast<MenuItem*>(this)->setItemChildVisible(false); }


	size_t MemberObsolete<MultiList>::getItemIndexSelected() { return static_cast<MultiList*>(this)->getIndexSelected(); }
	void MemberObsolete<MultiList>::setItemSelectedAt(size_t _index) { static_cast<MultiList*>(this)->setIndexSelected(_index); }
	void MemberObsolete<MultiList>::clearItemSelected() { static_cast<MultiList*>(this)->clearIndexSelected(); }

	size_t MemberObsolete<MultiList>::findItem(size_t _column, const UString& _name) { return static_cast<MultiList*>(this)->findSubItemWith(_column, _name); }
	const UString& MemberObsolete<MultiList>::getSubItem(size_t _column, size_t _index) { return static_cast<MultiList*>(this)->getSubItemNameAt(_column, _index); }
	void MemberObsolete<MultiList>::setSubItem(size_t _column, size_t _index, const UString& _name) { static_cast<MultiList*>(this)->setSubItemNameAt(_column, _index, _name); }
	void MemberObsolete<MultiList>::deleteColumn(size_t _column) { static_cast<MultiList*>(this)->removeColumnAt(_column); }
	void MemberObsolete<MultiList>::deleteAllColumns() { static_cast<MultiList*>(this)->removeAllColumns(); }
	int MemberObsolete<MultiList>::getColumnWidth(size_t _column) { return static_cast<MultiList*>(this)->getColumnWidthAt(_column); }
	const UString& MemberObsolete<MultiList>::getColumnName(size_t _column) { return static_cast<MultiList*>(this)->getColumnNameAt(_column); }
	void MemberObsolete<MultiList>::setColumnWidth(size_t _column, int _width) { static_cast<MultiList*>(this)->setColumnWidthAt(_column, _width); }
	void MemberObsolete<MultiList>::addColumn(int _width, const UString& _name) { static_cast<MultiList*>(this)->addColumn(_name, _width); }
	void MemberObsolete<MultiList>::setColumnName(size_t _column, const UString& _name) { static_cast<MultiList*>(this)->setColumnNameAt(_column, _name); }
	void MemberObsolete<MultiList>::insertColumn(size_t _column, int _width, const UString& _name) { static_cast<MultiList*>(this)->insertColumnAt(_column, _name, _width); }
	size_t MemberObsolete<MultiList>::getItemSelect() { return static_cast<MultiList*>(this)->getIndexSelected(); }
	void MemberObsolete<MultiList>::resetItemSelect() { static_cast<MultiList*>(this)->clearIndexSelected(); }
	void MemberObsolete<MultiList>::setItemSelect(size_t _index) { static_cast<MultiList*>(this)->setIndexSelected(_index); }
	void MemberObsolete<MultiList>::insertItem(size_t _index, const UString& _name) { static_cast<MultiList*>(this)->insertItemAt(_index, _name); }
	void MemberObsolete<MultiList>::setItem(size_t _index, const UString& _name) { static_cast<MultiList*>(this)->setItemNameAt(_index, _name); }
	const UString& MemberObsolete<MultiList>::getItem(size_t _index) { return static_cast<MultiList*>(this)->getItemNameAt(_index); }
	void MemberObsolete<MultiList>::deleteItem(size_t _index) { static_cast<MultiList*>(this)->removeItemAt(_index); }
	void MemberObsolete<MultiList>::deleteAllItems() { static_cast<MultiList*>(this)->removeAllItems(); }


	//void MemberObsolete<Progress>::setPosition(const IntCoord& _coord) { static_cast<Progress*>(this)->setCoord(_coord); }
	//void MemberObsolete<Progress>::setPosition(int _left, int _top, int _width, int _height) { static_cast<Progress*>(this)->setCoord(_left, _top, _width, _height); }

	void MemberObsolete<Progress>::setProgressStartPoint(Align _value)
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

	Align MemberObsolete<Progress>::getProgressStartPoint()
	{
		if (static_cast<Progress*>(this)->getFlowDirection() == FlowDirection::RightToLeft)
			return Align::Right;
		else if (static_cast<Progress*>(this)->getFlowDirection() == FlowDirection::TopToBottom)
			return Align::Top;
		else if (static_cast<Progress*>(this)->getFlowDirection() == FlowDirection::BottomToTop)
			return Align::Bottom;

		return Align::Left;
	}


	//void MemberObsolete<ScrollView>::setPosition(const IntCoord& _coord) { static_cast<ScrollView*>(this)->setCoord(_coord); }
	//void MemberObsolete<ScrollView>::setPosition(int _left, int _top, int _width, int _height) { static_cast<ScrollView*>(this)->setCoord(_left, _top, _width, _height); }

	void MemberObsolete<ScrollView>::showVScroll(bool _visible) { static_cast<ScrollView*>(this)->setVisibleVScroll(_visible); }
	bool MemberObsolete<ScrollView>::isShowVScroll() { return static_cast<ScrollView*>(this)->isVisibleVScroll(); }
	void MemberObsolete<ScrollView>::showHScroll(bool _visible) { static_cast<ScrollView*>(this)->setVisibleHScroll(_visible); }
	bool MemberObsolete<ScrollView>::isShowHScroll() { return static_cast<ScrollView*>(this)->isVisibleHScroll(); }


	//void MemberObsolete<Tab>::setPosition(const IntCoord& _coord) { static_cast<Tab*>(this)->setCoord(_coord); }
	//void MemberObsolete<Tab>::setPosition(int _left, int _top, int _width, int _height) { static_cast<Tab*>(this)->setCoord(_left, _top, _width, _height); }

	int MemberObsolete<Tab>::getSheetButtonWidthIndex(size_t _index) { return static_cast<Tab*>(this)->getButtonWidthAt(_index); }
	int MemberObsolete<Tab>::getSheetButtonWidth(TabItem* _sheet) { return static_cast<Tab*>(this)->getButtonWidth(_sheet); }
	void MemberObsolete<Tab>::setSheetButtonWidthIndex(size_t _index, int _width) { static_cast<Tab*>(this)->setButtonWidthAt(_index, _width); }
	void MemberObsolete<Tab>::setSheetButtonWidth(TabItem* _sheet, int _width) { static_cast<Tab*>(this)->setButtonWidth(_sheet, _width); }
	void MemberObsolete<Tab>::showBarButton(size_t _index) { static_cast<Tab*>(this)->beginToItemAt(_index); }
	void MemberObsolete<Tab>::showBarSelectButton() { static_cast<Tab*>(this)->beginToItemSelected(); }
	size_t MemberObsolete<Tab>::getSheetCount() { return static_cast<Tab*>(this)->getItemCount(); }
	const UString& MemberObsolete<Tab>::getSheetName(TabItem* _sheet) { return static_cast<Tab*>(this)->getItemName(_sheet); }
	const UString& MemberObsolete<Tab>::getSheetNameIndex(size_t _index) { return static_cast<Tab*>(this)->getItemNameAt(_index); }
	TabItem* MemberObsolete<Tab>::getSheet(size_t _index) { return static_cast<Tab*>(this)->getItemAt(_index); }
	void MemberObsolete<Tab>::setSheetNameIndex(size_t _index, const UString& _name, int _width) { static_cast<Tab*>(this)->setItemNameAt(_index, _name); }
	void MemberObsolete<Tab>::setSheetName(TabItem* _sheet, const UString& _name, int _width) { static_cast<Tab*>(this)->setItemName(_sheet, _name); }
	TabItem* MemberObsolete<Tab>::addSheet(const UString& _name, int _width) { return static_cast<Tab*>(this)->addItem(_name, _width); }
	TabItem* MemberObsolete<Tab>::insertSheet(size_t _index, const UString& _name, int _width) { return static_cast<Tab*>(this)->insertItemAt(_index, _name); }
	void MemberObsolete<Tab>::removeSheetIndex(size_t _index) { static_cast<Tab*>(this)->removeItemAt(_index); }
	void MemberObsolete<Tab>::removeSheet(TabItem* _sheet) { static_cast<Tab*>(this)->removeItem(_sheet); }
	void MemberObsolete<Tab>::selectSheetIndex(size_t _index, bool _smooth) { static_cast<Tab*>(this)->setIndexSelected(_index); }
	void MemberObsolete<Tab>::selectSheet(TabItem* _sheet, bool _smooth) { static_cast<Tab*>(this)->setItemSelected(_sheet); }
	size_t MemberObsolete<Tab>::getSelectSheetIndex() { return static_cast<Tab*>(this)->getIndexSelected(); }

	size_t MemberObsolete<Tab>::getItemIndexSelected() { return static_cast<Tab*>(this)->getIndexSelected(); }
	void MemberObsolete<Tab>::setItemSelectedAt(size_t _index) { static_cast<Tab*>(this)->setIndexSelected(_index); }

	//void MemberObsolete<VScroll>::setPosition(const IntCoord& _coord) { static_cast<VScroll*>(this)->setCoord(_coord); }
	//void MemberObsolete<VScroll>::setPosition(int _left, int _top, int _width, int _height) { static_cast<VScroll*>(this)->setCoord(_left, _top, _width, _height); }


	//void MemberObsolete<Widget>::setPosition(const IntCoord& _coord) { static_cast<Widget*>(this)->setCoord(_coord); }
	//void MemberObsolete<Widget>::setPosition(int _left, int _top, int _width, int _height) { static_cast<Widget*>(this)->setCoord(_left, _top, _width, _height); }

	bool MemberObsolete<Widget>::isEnabled() { return static_cast<Widget*>(this)->getEnabled(); }
	bool MemberObsolete<Widget>::isInheritsAlpha() { return static_cast<Widget*>(this)->getInheritsAlpha(); }
	bool MemberObsolete<Widget>::isNeedKeyFocus() { return static_cast<Widget*>(this)->getNeedKeyFocus(); }
	bool MemberObsolete<Widget>::isNeedMouseFocus() { return static_cast<Widget*>(this)->getNeedMouseFocus(); }
	bool MemberObsolete<Widget>::isInheritsPick() { return static_cast<Widget*>(this)->getInheritsPick(); }
	bool MemberObsolete<Widget>::isVisible() { return static_cast<Widget*>(this)->getVisible(); }


	//void MemberObsolete<Window>::setPosition(const IntCoord& _coord) { static_cast<Window*>(this)->setCoord(_coord); }
	//void MemberObsolete<Window>::setPosition(int _left, int _top, int _width, int _height) { static_cast<Window*>(this)->setCoord(_left, _top, _width, _height); }

	void MemberObsolete<Window>::showSmooth(bool _reset) { static_cast<Window*>(this)->setVisibleSmooth(true); }
	void MemberObsolete<Window>::hideSmooth() { static_cast<Window*>(this)->setVisibleSmooth(false); }
	void MemberObsolete<Window>::setMinMax(const IntRect& _minmax) { static_cast<Window*>(this)->setMinSize(_minmax.left, _minmax.top); static_cast<Window*>(this)->setMaxSize(_minmax.right, _minmax.bottom); }
	void MemberObsolete<Window>::setMinMax(int _min_w, int _min_h, int _max_w, int _max_h) { static_cast<Window*>(this)->setMinSize(_min_w, _min_h); static_cast<Window*>(this)->setMaxSize(_max_w, _max_h); }
	IntRect MemberObsolete<Window>::getMinMax() { return IntRect(static_cast<Window*>(this)->getMinSize().width, static_cast<Window*>(this)->getMinSize().height, static_cast<Window*>(this)->getMaxSize().width, static_cast<Window*>(this)->getMaxSize().height); }

	bool MemberObsolete<FontManager>::load(const std::string& _file) { return ResourceManager::getInstance().load(_file); }

	void MemberObsolete<Gui>::destroyWidgetsVector(VectorWidgetPtr& _widgets) { static_cast<Gui*>(this)->destroyWidgets(_widgets); }

	bool MemberObsolete<Gui>::injectMouseMove( int _absx, int _absy, int _absz) { return InputManager::getInstance().injectMouseMove(_absx, _absy, _absz); }
	bool MemberObsolete<Gui>::injectMousePress( int _absx, int _absy, MouseButton _id ) { return InputManager::getInstance().injectMousePress(_absx, _absy, _id); }
	bool MemberObsolete<Gui>::injectMouseRelease( int _absx, int _absy, MouseButton _id ) { return InputManager::getInstance().injectMouseRelease(_absx, _absy, _id); }
	bool MemberObsolete<Gui>::injectKeyPress(KeyCode _key, Char _text) { return InputManager::getInstance().injectKeyPress(_key, _text); }
	bool MemberObsolete<Gui>::injectKeyRelease(KeyCode _key) { return InputManager::getInstance().injectKeyRelease(_key); }

	void MemberObsolete<Gui>::hidePointer() { PointerManager::getInstance().setVisible(false); }
	void MemberObsolete<Gui>::showPointer() { PointerManager::getInstance().setVisible(true); }
	bool MemberObsolete<Gui>::isShowPointer() { return PointerManager::getInstance().isVisible(); }

	bool MemberObsolete<Gui>::load(const std::string& _file) { return ResourceManager::getInstance().load(_file); }

	const IntSize& MemberObsolete<Gui>::getViewSize() { return RenderManager::getInstance().getViewSize(); }
	int MemberObsolete<Gui>::getViewWidth() { return RenderManager::getInstance().getViewSize().width; }
	int MemberObsolete<Gui>::getViewHeight() { return RenderManager::getInstance().getViewSize().height; }

	void MemberObsolete<Gui>::setVisiblePointer(bool _value) { PointerManager::getInstance().setVisible(_value); }
	bool MemberObsolete<Gui>::isVisiblePointer() { return PointerManager::getInstance().isVisible(); }

	bool MemberObsolete<LanguageManager>::load(const std::string& _file) { return ResourceManager::getInstance().load(_file); }

	bool MemberObsolete<LayerManager>::load(const std::string& _file) { return ResourceManager::getInstance().load(_file); }

	VectorWidgetPtr MemberObsolete<LayoutManager>::load(const std::string& _file) { return static_cast<LayoutManager*>(this)->loadLayout(_file); }

	bool MemberObsolete<PluginManager>::load(const std::string& _file) { return ResourceManager::getInstance().load(_file); }

	void MemberObsolete<PointerManager>::show() { static_cast<PointerManager*>(this)->setVisible(true); }
	void MemberObsolete<PointerManager>::hide() { static_cast<PointerManager*>(this)->setVisible(false); }
	bool MemberObsolete<PointerManager>::isShow() { return static_cast<PointerManager*>(this)->isVisible(); }
	bool MemberObsolete<PointerManager>::load(const std::string& _file) { return ResourceManager::getInstance().load(_file); }

	size_t MemberObsolete<ResourceManager>::getResourceCount() { return static_cast<ResourceManager*>(this)->getCount(); }
	IResourcePtr MemberObsolete<ResourceManager>::getResource(const std::string& _name, bool _throw) { return static_cast<ResourceManager*>(this)->getByName(_name, _throw); }

	ResourceSkin* MemberObsolete<SkinManager>::getSkin(const std::string& _name) { return static_cast<SkinManager*>(this)->getByName(_name); }
	bool MemberObsolete<SkinManager>::load(const std::string& _file) { return ResourceManager::getInstance().load(_file); }


	void MemberObsolete<WidgetManager>::destroyWidgetsVector(VectorWidgetPtr &_widgets) { static_cast<WidgetManager*>(this)->destroyWidgets(_widgets); }
	Widget* MemberObsolete<WidgetManager>::findWidgetT(const std::string& _name, bool _throw) { return Gui::getInstance().findWidgetT(_name, _throw); }
	Widget* MemberObsolete<WidgetManager>::findWidgetT(const std::string& _name, const std::string& _prefix, bool _throw) { return Gui::getInstance().findWidgetT(_name, _prefix, _throw); }
	void MemberObsolete<WidgetManager>::parse(Widget* _widget, const std::string &_key, const std::string &_value) { _widget->setProperty(_key, _value); }


#endif // MYGUI_DONT_USE_OBSOLETE

#ifndef MYGUI_DONT_USE_OBSOLETE

	std::string convertAlignToDirection(const std::string& _value)
	{
		Align align = utility::parseValue<Align>(_value);
		if (align == Align::Right)
			return FlowDirection(FlowDirection::RightToLeft).print();
		else if (align == Align::Top)
			return FlowDirection(FlowDirection::TopToBottom).print();
		else if (align == Align::Bottom)
			return FlowDirection(FlowDirection::BottomToTop).print();
		return FlowDirection(FlowDirection::LeftToRight).print();
	}

	std::string convertRectToCoord(const std::string& _value)
	{
		IntRect rect = IntRect::parse(_value);
		IntCoord coord(rect.left, rect.top, rect.width(), rect.height());
		return coord.print();
	}

	BackwardCompatibility::MapString BackwardCompatibility::mPropertyRename;
	BackwardCompatibility::SetString BackwardCompatibility::mPropertyIgnore;

#endif // MYGUI_DONT_USE_OBSOLETE

	bool BackwardCompatibility::checkProperty(Widget* _owner, std::string& _key, std::string& _value)
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		if (mPropertyIgnore.find(_key) != mPropertyIgnore.end())
			return false;

		MapString::iterator item = mPropertyRename.find(_key);
		if (item != mPropertyRename.end())
		{
			MYGUI_LOG(Warning, (*item).first << " is deprecated, use " << (*item).second << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			_key = (*item).second;
		}
		else if (_key == "Progress_StartPoint")
		{
			MYGUI_LOG(Warning, "Progress_StartPoint is deprecated, use Progress_FlowDirection" << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			_key = "Progress_FlowDirection";
			_value = convertAlignToDirection(_value);
		}
		else if (_key == "ComboBox_AddItem")
		{
			MYGUI_LOG(Warning, "ComboBox_AddItem is deprecated" << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			ComboBox* widget = _owner->castType<ComboBox>(false);
			if (widget != nullptr)
				widget->addItem(_value);
			return false;
		}
		else if (_key == "List_AddItem")
		{
			MYGUI_LOG(Warning, "List_AddItem is deprecated" << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			List* widget = _owner->castType<List>(false);
			if (widget != nullptr)
				widget->addItem(_value);
			return false;
		}
		else if (_key == "Tab_AddSheet")
		{
			MYGUI_LOG(Warning, "Tab_AddSheet is deprecated" << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			Tab* widget = _owner->castType<Tab>(false);
			if (widget != nullptr)
				widget->addItem(_value);
			return false;
		}
		else if (_key == "Tab_AddItem")
		{
			MYGUI_LOG(Warning, "Tab_AddItem is deprecated" << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			Tab* widget = _owner->castType<Tab>(false);
			if (widget != nullptr)
				widget->addItem(_value);
			return false;
		}
		else if (_key == "Window_MinMax")
		{
			MYGUI_LOG(Warning, "Window_MinMax is deprecated, use Window_MinSize or Window_MaxSize" << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			Window* widget = _owner->castType<Window>(false);
			if (widget != nullptr)
			{
				IntRect rect = IntRect::parse(_value);
				widget->setMinSize(rect.left, rect.top);
				widget->setMaxSize(rect.right, rect.bottom);
			}
			return false;
		}
		else if (_key == "Message_AddButton")
		{
			MYGUI_LOG(Warning, "Message_AddButton is deprecated" << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			Message* widget = _owner->castType<Message>(false);
			if (widget != nullptr)
				widget->addButtonName(_value);
			return false;
		}
		else if (_key == "ImageRect")
		{
			MYGUI_LOG(Warning, "ImageRect is deprecated, use ImageCoord" << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			_key = "ImageCoord";
			_value = convertRectToCoord(_value);
		}
		else if (_key == "StartPoint")
		{
			MYGUI_LOG(Warning, "StartPoint is deprecated, use FlowDirection" << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			_key = "FlowDirection";
			_value = convertAlignToDirection(_value);
		}

#endif // MYGUI_DONT_USE_OBSOLETE
		return true;
	}

	void BackwardCompatibility::initialise()
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		FactoryManager& factory = FactoryManager::getInstance();

		factory.registerFactory<RenderBox>("Widget");
		factory.registerFactory<Sheet>("Widget");

		mPropertyRename["Button_Pressed"] = "StateSelected";
		mPropertyRename["ButtonPressed"] = "StateSelected";
		mPropertyRename["StateCheck"] = "StateSelected";
		mPropertyRename["Edit_ShowVScroll"] = "VisibleVScroll";
		mPropertyRename["ScrollView_VScroll"] = "VisibleVScroll";
		mPropertyRename["Edit_ShowHScroll"] = "VisibleHScroll";
		mPropertyRename["ScrollView_HScroll"] = "VisibleHScroll";
		mPropertyRename["Widget_Caption"] = "Caption";
		mPropertyRename["Message_Caption"] = "Caption";
		mPropertyRename["Progress_Position"] = "RangePosition";
		mPropertyRename["Scroll_Position"] = "RangePosition";
		mPropertyRename["Tab_SelectSheet"] = "SelectItem";
		mPropertyRename["Message_Message"] = "MessageText";
		mPropertyRename["Message_Button"] = "MessageButton";
		mPropertyRename["Image_Texture"] = "ImageTexture";
		mPropertyRename["Image_Coord"] = "ImageRegion";
		mPropertyRename["Image_Tile"] = "ImageTile";
		mPropertyRename["Image_Index"] = "ImageIndex";
		mPropertyRename["Image_Resource"] = "ImageResource";
		mPropertyRename["Image_Group"] = "ImageGroup";
		mPropertyRename["Image_Name"] = "ImageName";
		mPropertyRename["MenuItem_Id"] = "MenuItemId";
		mPropertyRename["MenuItem_Type"] = "MenuItemType";
		mPropertyRename["AlignText"] = "TextAlign";
		mPropertyRename["ToStick"] = "Snap";

		mPropertyIgnore.insert("MaxListLength");
		mPropertyIgnore.insert("ListSmoothShow");
		mPropertyIgnore.insert("HeightList");
		mPropertyIgnore.insert("AlignVert");
		mPropertyIgnore.insert("DragLayer");
		mPropertyIgnore.insert("SkinLine");
		mPropertyIgnore.insert("HeightLine");
		mPropertyIgnore.insert("SkinLine");
		mPropertyIgnore.insert("HeightLine");
		mPropertyIgnore.insert("SeparatorHeight");
		mPropertyIgnore.insert("SeparatorSkin");
		mPropertyIgnore.insert("SubmenuImageSize");
		mPropertyIgnore.insert("SubMenuSkin");
		mPropertyIgnore.insert("SubMenuLayer");
		mPropertyIgnore.insert("AlignVert");
		mPropertyIgnore.insert("DistanceButton");
		mPropertyIgnore.insert("ButtonSkin");
		mPropertyIgnore.insert("ButtonType");
		mPropertyIgnore.insert("ButtonSize");
		mPropertyIgnore.insert("ButtonOffset");
		mPropertyIgnore.insert("DefaultLayer");
		mPropertyIgnore.insert("FadeSkin");
		mPropertyIgnore.insert("FadeLayer");
		mPropertyIgnore.insert("SkinButton");
		mPropertyIgnore.insert("HeightButton");
		mPropertyIgnore.insert("SkinList");
		mPropertyIgnore.insert("SkinButtonEmpty");
		mPropertyIgnore.insert("WidthSeparator");
		mPropertyIgnore.insert("SkinSeparator");
		mPropertyIgnore.insert("TrackSkin");
		mPropertyIgnore.insert("TrackWidth");
		mPropertyIgnore.insert("TrackMin");
		mPropertyIgnore.insert("TrackStep");
		mPropertyIgnore.insert("TrackFill");
		mPropertyIgnore.insert("OffsetBar");
		mPropertyIgnore.insert("ButtonSkin");
		mPropertyIgnore.insert("EmptyBarSkin");
		mPropertyIgnore.insert("TrackRangeMargins");
		mPropertyIgnore.insert("MinTrackSize");
		mPropertyIgnore.insert("MainMove");
#endif // MYGUI_DONT_USE_OBSOLETE
	}

	void BackwardCompatibility::shutdown()
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		mPropertyRename.clear();
#endif // MYGUI_DONT_USE_OBSOLETE
	}

} // namespace MyGUI
