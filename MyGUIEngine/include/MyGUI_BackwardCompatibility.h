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
#ifndef __MYGUI_BACKWARD_COMPATIBILITY_H__
#define __MYGUI_BACKWARD_COMPATIBILITY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Colour.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_Any.h"
#include "MyGUI_MouseButton.h"
#include "MyGUI_KeyCode.h"
#include "MyGUI_Macros.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_IResource.h"
#include <map>
#include <set>

namespace MyGUI
{

#ifndef MYGUI_DONT_USE_OBSOLETE

	template <>
	class MemberObsolete<Button>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : void Button::setStateSelected(bool _value)")
		void setButtonPressed(bool _value);
		MYGUI_OBSOLETE(" is deprecated, use : bool Button::getStateSelected()")
		bool getButtonPressed();
		MYGUI_OBSOLETE(" is deprecated, use : void Button::setStateSelected(bool _value)")
		void setStateCheck(bool _value);
		MYGUI_OBSOLETE(" is deprecated, use : bool Button::getStateSelected()")
		bool getStateCheck();
	};

	template <>
	class MemberObsolete<ComboBox>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : size_t ComboBox::getIndexSelected()")
		size_t getItemIndexSelected();
		MYGUI_OBSOLETE(" is deprecated, use : void ComboBox::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void ComboBox::clearIndexSelected()")
		void clearItemSelected();

		MYGUI_OBSOLETE(" is deprecated, use : void ComboBox::insertItemAt(size_t _index, const UString& _name)")
		void insertItem(size_t _index, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : void ComboBox::setItemNameAt(size_t _index, const UString& _name)")
		void setItem(size_t _index, const UString& _item);
		MYGUI_OBSOLETE(" is deprecated, use : const UString& ComboBox::getItemNameAt(size_t _index)")
		const UString& getItem(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void ComboBox::removeItemAt(size_t _index)")
		void deleteItem(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void ComboBox::removeAllItems()")
		void deleteAllItems();
		MYGUI_OBSOLETE(" is deprecated, use : size_t ComboBox::getIndexSelected()")
		size_t getItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void void ComboBox::clearIndexSelected()")
		void resetItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void ComboBox::setIndexSelected(size_t _index)")
		void setItemSelect(size_t _index);

		MYGUI_OBSOLETE(" is deprecated, use : void ComboBox::setMaxListLength(int _value)")
		void setMaxListHeight(int _value);
		MYGUI_OBSOLETE(" is deprecated, use : int ComboBox::getMaxListLength()")
		int getMaxListHeight();
	};

	template <>
	class MemberObsolete<Edit>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : void Edit::setVisibleVScroll(bool _visible)")
		void showVScroll(bool _visible);
		MYGUI_OBSOLETE(" is deprecated, use : bool Edit::isVisibleVScroll()")
		bool isShowVScroll();
		MYGUI_OBSOLETE(" is deprecated, use : void Edit::setVisibleHScroll(bool _visible)")
		void showHScroll(bool _visible);
		MYGUI_OBSOLETE(" is deprecated, use : bool Edit::isVisibleHScroll()")
		bool isShowHScroll();

		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(const IntCoord& _coord)")
		//void setPosition(const IntCoord& _coord);
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		//void setPosition(int _left, int _top, int _width, int _height);

		MYGUI_OBSOLETE(" is deprecated, use : void Edit::setTextIntervalColour(size_t _start, size_t _count, const Colour& _colour)")
		void setTextColour(size_t _start, size_t _count, const Colour& _colour);
		MYGUI_OBSOLETE(" is deprecated, use : size_t Edit::getTextSelectionStart() , size_t getTextSelectionEnd()")
		void getTextSelect(size_t& _start, size_t& _end);
		MYGUI_OBSOLETE(" is deprecated, use : UString Edit::getTextInterval(size_t _start, size_t _count)")
		UString getText(size_t _start, size_t _count);
		MYGUI_OBSOLETE(" is deprecated, use : void Edit::setTextSelection(size_t _start, size_t _end)")
		void setTextSelect(size_t _start, size_t _end);
		MYGUI_OBSOLETE(" is deprecated, use : void Edit::deleteTextSelection()")
		void deleteTextSelect();
		MYGUI_OBSOLETE(" is deprecated, use : UString Edit::getTextSelection()")
		UString getSelectedText();
		MYGUI_OBSOLETE(" is deprecated, use : bool Edit::isTextSelection()")
		bool isTextSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void Edit::setTextSelectionColour(const Colour& _colour)")
		void setTextSelectColour(const Colour& _colour);
	};

	template <>
	class MemberObsolete<ItemBox>
	{
	public:
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(const IntCoord& _coord)")
		//void setPosition(const IntCoord& _coord);
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		//void setPosition(int _left, int _top, int _width, int _height);

		MYGUI_OBSOLETE(" is deprecated, use : size_t ItemBox::getItemIndexSelected()")
		size_t getItemIndexSelected();
		MYGUI_OBSOLETE(" is deprecated, use : void ItemBox::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void ItemBox::clearIndexSelected()")
		void clearItemSelected();

		MYGUI_OBSOLETE(" is deprecated, use : void ItemBox::insertItemAt(size_t _index, Any _data)")
		void insertItem(size_t _index, Any _data = Any::Null);
		MYGUI_OBSOLETE(" is deprecated, use : void ItemBox::setItemDataAt(size_t _index, Any _data)")
		void setItemData(size_t _index, Any _data);
		MYGUI_OBSOLETE(" is deprecated, use : void ItemBox::removeItemAt(size_t _index)")
		void deleteItem(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void ItemBox::removeAllItems()")
		void deleteAllItems();
		MYGUI_OBSOLETE(" is deprecated, use : size_t ItemBox::getIndexSelected()")
		size_t getItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void ItemBox::clearIndexSelected()")
		void resetItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void ItemBox::setIndexSelected(size_t _index)")
		void setItemSelect(size_t _index);

		MYGUI_OBSOLETE(" is deprecated, use : Widget* ItemBox::getWidgetDrag()")
		Widget* getWidgetDrop();
		MYGUI_OBSOLETE(" is deprecated, use : void ItemBox::resetDrag()")
		void resetDrop();
	};

	template <>
	class MemberObsolete<List>
	{
	public:
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(const IntCoord& _coord)")
		//void setPosition(const IntCoord& _coord);
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		//void setPosition(int _left, int _top, int _width, int _height);

		MYGUI_OBSOLETE(" is deprecated, use : size_t List::getIndexSelected()")
		size_t getItemIndexSelected();
		MYGUI_OBSOLETE(" is deprecated, use : void List::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void List::clearIndexSelected()")
		void clearItemSelected();

		MYGUI_OBSOLETE(" is deprecated, use : void List::insertItemAt(size_t _index, const UString& _name)")
		void insertItem(size_t _index, const UString& _item);
		MYGUI_OBSOLETE(" is deprecated, use : void List::setItemNameAt(size_t _index, const UString& _name)")
		void setItem(size_t _index, const UString& _item);
		MYGUI_OBSOLETE(" is deprecated, use : const UString& List::getItemNameAt(size_t _index)")
		const UString& getItem(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void List::removeItemAt(size_t _index)")
		void deleteItem(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void List::removeAllItems()")
		void deleteAllItems();
		MYGUI_OBSOLETE(" is deprecated, use : size_t List::findItemIndexWith(const UString& _name)")
		size_t findItem(const UString& _item);
		MYGUI_OBSOLETE(" is deprecated, use : size_t List::getIndexSelected()")
		size_t getItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void List::clearIndexSelected()")
		void resetItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void List::setIndexSelected(size_t _index)")
		void setItemSelect(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void List::beginToItemAt(size_t _index)")
		void beginToIndex(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void List::beginToItemFirst()")
		void beginToStart();
		MYGUI_OBSOLETE(" is deprecated, use : void List::beginToItemLast()")
		void beginToEnd();
		MYGUI_OBSOLETE(" is deprecated, use : void List::beginToItemSelected()")
		void beginToSelect();
		MYGUI_OBSOLETE(" is deprecated, use : bool List::isItemVisibleAt(size_t _index, bool _fill)")
		bool isItemVisible(size_t _index, bool _fill = true);
		MYGUI_OBSOLETE(" is deprecated, use : bool List::isItemSelectedVisible(bool _fill)")
		bool isItemSelectVisible(bool _fill = true);
	};

	template <>
	class MemberObsolete<MenuCtrl>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : void Widget::setVisible(bool _value)")
		void showMenu();
		MYGUI_OBSOLETE(" is deprecated, use : void Widget::setVisible(bool _value)")
		void hideMenu();
		MYGUI_OBSOLETE(" is deprecated, use : bool Widget::getVisible()")
		bool isShowMenu();

		MYGUI_OBSOLETE(" is deprecated, use : void setItemChildVisibleAt(size_t _index, bool _visible)")
		void showItemChildAt(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void setItemChildVisible(MenuItem* _item, bool _visible)")
		void showItemChild(MenuItem* _item);
		MYGUI_OBSOLETE(" is deprecated, use : void setItemChildVisibleAt(size_t _index, bool _visible)")
		void hideItemChildAt(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void setItemChildVisible(MenuItem* _item, bool _visible)")
		void hideItemChild(MenuItem* _item);
	};

	template <>
	class MemberObsolete<MenuItem>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : void setItemChildVisible(bool _visible)")
		void showItemChild();
		MYGUI_OBSOLETE(" is deprecated, use : void setItemChildVisible(bool _visible)")
		void hideItemChild();
	};

	template <>
	class MemberObsolete<MultiList>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : size_t MultiList::getIndexSelected()")
		size_t getItemIndexSelected();
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::clearIndexSelected()")
		void clearItemSelected();

		MYGUI_OBSOLETE(" is deprecated, use : size_t MultiList::findSubItemWith(size_t _column, const UString& _name)")
		size_t findItem(size_t _column, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : const UString& MultiList::getSubItemNameAt(size_t _column, size_t _index)")
		const UString& getSubItem(size_t _column, size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::setSubItemNameAt(size_t _column, size_t _index, const UString& _name)")
		void setSubItem(size_t _column, size_t _index, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::removeColumnAt(size_t _column)")
		void deleteColumn(size_t _column);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::removeAllColumns()")
		void deleteAllColumns();
		MYGUI_OBSOLETE(" is deprecated, use : int MultiList::getColumnWidthAt(size_t _column)")
		int getColumnWidth(size_t _column);
		MYGUI_OBSOLETE(" is deprecated, use : const UString& MultiList::getColumnNameAt(size_t _column)")
		const UString& getColumnName(size_t _column);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::setColumnWidthAt(size_t _column, int _width)")
		void setColumnWidth(size_t _column, int _width);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::addColumn(const UString& _name, int _width, Any _data)")
		void addColumn(int _width, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::setColumnNameAt(size_t _column, const UString& _name)")
		void setColumnName(size_t _column, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::insertColumnAt(size_t _column, const UString& _name, int _width, Any _data)")
		void insertColumn(size_t _column, int _width, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : size_t MultiList::getIndexSelected()")
		size_t getItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::clearIndexSelected()")
		void resetItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::setIndexSelected(size_t _index)")
		void setItemSelect(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::insertItemAt(size_t _index, const UString& _name, Any _data)")
		void insertItem(size_t _index, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::setItemNameAt(size_t _index, const UString& _name)")
		void setItem(size_t _index, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : const UString& MultiList::getItemNameAt(size_t _index)")
		const UString& getItem(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::removeItemAt(size_t _index)")
		void deleteItem(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiList::removeAllItems()")
		void deleteAllItems();
	};

	template <>
	class MemberObsolete<Progress>
	{
	public:
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(const IntCoord& _coord)")
		//void setPosition(const IntCoord& _coord);
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		//void setPosition(int _left, int _top, int _width, int _height);

		MYGUI_OBSOLETE(" is deprecated, use : void Progress::setFlowDirection(FlowDirection _value)")
		void setProgressStartPoint(Align _value);
		MYGUI_OBSOLETE(" is deprecated, use : FlowDirection Progress::getFlowDirection()")
		Align getProgressStartPoint();
	};

	template <>
	class MemberObsolete<ScrollView>
	{
	public:
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(const IntCoord& _coord)")
		//void setPosition(const IntCoord& _coord);
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		//void setPosition(int _left, int _top, int _width, int _height);

		MYGUI_OBSOLETE(" is deprecated, use : void ScrollView::setVisibleVScroll(bool _visible)")
		void showVScroll(bool _visible);
		MYGUI_OBSOLETE(" is deprecated, use : bool ScrollView::isVisibleVScroll()")
		bool isShowVScroll();
		MYGUI_OBSOLETE(" is deprecated, use : void ScrollView::setVisibleHScroll(bool _visible)")
		void showHScroll(bool _visible);
		MYGUI_OBSOLETE(" is deprecated, use : bool ScrollView::isVisibleHScroll()")
		bool isShowHScroll();
	};

	template <>
	class MemberObsolete<Tab>
	{
	public:
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(const IntCoord& _coord)")
		//void setPosition(const IntCoord& _coord);
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		//void setPosition(int _left, int _top, int _width, int _height);

		MYGUI_OBSOLETE(" is deprecated, use : int Tab::getButtonWidthAt(size_t _index)")
		int getSheetButtonWidthIndex(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : int Tab::getButtonWidth(TabItem* _item)")
		int getSheetButtonWidth(TabItem* _sheet);
		MYGUI_OBSOLETE(" is deprecated, use : void Tab::setButtonWidthAt(size_t _index, int _width)")
		void setSheetButtonWidthIndex(size_t _index, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : void Tab::setButtonWidth(TabItem* _item, int _width)")
		void setSheetButtonWidth(TabItem* _sheet, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : void Tab::beginToItemAt(size_t _index)")
		void showBarButton(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void Tab::beginToItemSelected()")
		void showBarSelectButton();
		MYGUI_OBSOLETE(" is deprecated, use : size_t Tab::getItemCount()")
		size_t getSheetCount();
		MYGUI_OBSOLETE(" is deprecated, use : const UString& Tab::getItemName(TabItem* _item)")
		const UString& getSheetName(TabItem* _sheet);
		MYGUI_OBSOLETE(" is deprecated, use : const UString& Tab::getItemNameAt(size_t _index)")
		const UString& getSheetNameIndex(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : TabItem* Tab::getItemAt(size_t _index)")
		TabItem* getSheet(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void Tab::setItemNameAt(size_t _index, const UString& _name)")
		void setSheetNameIndex(size_t _index, const UString& _name, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : void Tab::setItemName(TabItem* _item, const UString& _name)")
		void setSheetName(TabItem* _sheet, const UString& _name, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : TabItem* Tab::addItem(const UString& _name, Any _data)")
		TabItem* addSheet(const UString& _name, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : TabItem* Tab::insertItemAt(size_t _index, const UString& _name, Any _data)")
		TabItem* insertSheet(size_t _index, const UString& _name, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : void Tab::removeItemAt(size_t _index)")
		void removeSheetIndex(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void Tab::removeItem(TabItem* _item)")
		void removeSheet(TabItem* _sheet);
		MYGUI_OBSOLETE(" is deprecated, use : void Tab::setIndexSelected(size_t _index)")
		void selectSheetIndex(size_t _index, bool _smooth = true);
		MYGUI_OBSOLETE(" is deprecated, use : void Tab::setItemSelected(TabItem* _item)")
		void selectSheet(TabItem* _sheet, bool _smooth = true);
		MYGUI_OBSOLETE(" is deprecated, use : size_t Tab::getIndexSelected()")
		size_t getSelectSheetIndex();

		MYGUI_OBSOLETE(" is deprecated, use : size_t Widget::getIndexSelected()")
		size_t getItemIndexSelected();
		MYGUI_OBSOLETE(" is deprecated, use : void Widget::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index);
	};

	template <>
	class MemberObsolete<VScroll>
	{
	public:
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(const IntCoord& _coord)")
		//void setPosition(const IntCoord& _coord);
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		//void setPosition(int _left, int _top, int _width, int _height);
	};

	template <>
	class MemberObsolete<Widget>
	{
	public:
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(const IntCoord& _coord)")
		//void setPosition(const IntCoord& _coord);
		//MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		//void setPosition(int _left, int _top, int _width, int _height):

		MYGUI_OBSOLETE(" is deprecated, use : bool Widget::getEnabled() const")
		bool isEnabled();
		MYGUI_OBSOLETE(" is deprecated, use : bool Widget::getInheritsAlpha() const")
		bool isInheritsAlpha();
		MYGUI_OBSOLETE(" is deprecated, use : bool Widget::getNeedKeyFocus() const")
		bool isNeedKeyFocus();
		MYGUI_OBSOLETE(" is deprecated, use : bool Widget::getNeedMouseFocus() const")
		bool isNeedMouseFocus();
		MYGUI_OBSOLETE(" is deprecated, use : bool Widget::getInheritsPick() const")
		bool isInheritsPick();
		MYGUI_OBSOLETE(" is deprecated, use : bool Widget::getVisible() const")
		bool isVisible();
	};

	template <>
	class MemberObsolete<Window>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord& _coord);
		MYGUI_OBSOLETE(" is deprecated, use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height);

		MYGUI_OBSOLETE(" is deprecated, use : void setVisibleSmooth(bool _visible)")
		void showSmooth(bool _reset = false);
		MYGUI_OBSOLETE(" is deprecated, use : void setVisibleSmooth(bool _visible)")
		void hideSmooth();
		MYGUI_OBSOLETE(" is deprecated, use : void setMinSize(const IntSize& _min) , void setMaxSize(const IntSize& _min)")
		void setMinMax(const IntRect& _minmax);
		MYGUI_OBSOLETE(" is deprecated, use : void setMinSize(const IntSize& _min) , void setMaxSize(const IntSize& _min)")
		void setMinMax(int _min_w, int _min_h, int _max_w, int _max_h);
		MYGUI_OBSOLETE(" is deprecated, use : IntSize getMinSize() , IntSize getMaxSize()")
		IntRect getMinMax();
	};

	template <>
	class MemberObsolete<FontManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);
	};

	template <>
	class MemberObsolete<Gui>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : void Gui::destroyWidgets(VectorWidgetPtr &_widgets)")
		void destroyWidgetsVector(VectorWidgetPtr& _widgets);

		MYGUI_OBSOLETE(" is deprecated, use : void Gui::setVisiblePointer(bool _value)")
		void hidePointer();
		MYGUI_OBSOLETE(" is deprecated, use : void Gui::setVisiblePointer(bool _value)")
		void showPointer();
		MYGUI_OBSOLETE(" is deprecated, use : bool Gui::isVisiblePointer()")
		bool isShowPointer();
		MYGUI_OBSOLETE("called be renderer, do not call it manually")
		void injectFrameEntered(float _time) { }

		MYGUI_OBSOLETE(" is deprecated, use : void Gui::getViewSize().width")
		int getViewWidth();
		MYGUI_OBSOLETE(" is deprecated, use : void Gui::getViewSize().height")
		int getViewHeight();

		MYGUI_OBSOLETE(" is deprecated, use : bool InputManager::injectMouseMove(int _absx, int _absy, int _absz)")
		bool injectMouseMove(int _absx, int _absy, int _absz);
		MYGUI_OBSOLETE(" is deprecated, use : bool InputManager::injectMousePress(int _absx, int _absy, MouseButton _id)")
		bool injectMousePress(int _absx, int _absy, MouseButton _id);
		MYGUI_OBSOLETE(" is deprecated, use : bool InputManager::injectMouseRelease(int _absx, int _absy, MouseButton _id)")
		bool injectMouseRelease(int _absx, int _absy, MouseButton _id);
		MYGUI_OBSOLETE(" is deprecated, use : bool InputManager::injectKeyPress(KeyCode _key, Char _text = 0)")
		bool injectKeyPress(KeyCode _key, Char _text = 0);
		MYGUI_OBSOLETE(" is deprecated, use : bool InputManager::injectKeyRelease(KeyCode _key)")
		bool injectKeyRelease(KeyCode _key);

		MYGUI_OBSOLETE(" is deprecated, use : void PointerManager::setVisible(_value)")
		void setVisiblePointer(bool _value);
		MYGUI_OBSOLETE(" is deprecated, use : bool PointerManager::isVisible()")
		bool isVisiblePointer();

		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);

		MYGUI_OBSOLETE(" is deprecated, use : const IntSize& RenderManager::getViewSize() const")
		const IntSize& getViewSize();
	};

	template <>
	class MemberObsolete<LanguageManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);
	};

	template <>
	class MemberObsolete<LayerManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);
	};

	template <>
	class MemberObsolete<LayoutManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : VectorWidgetPtr& LayoutManager::loadLayout(const std::string& _file, const std::string& _prefix, Widget* _parent)")
		VectorWidgetPtr load(const std::string& _file);
	};

	template <>
	class MemberObsolete<PluginManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);
	};

	template <>
	class MemberObsolete<PointerManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : void PointerManager::setDefaultPointer(const std::string& _value)")
		void setDeafultPointer(const std::string& _value);
		MYGUI_OBSOLETE(" is deprecated, use : void PointerManager::setVisible(bool _visible)")
		void show();
		MYGUI_OBSOLETE(" is deprecated, use : void PointerManager::setVisible(bool _visible)")
		void hide();
		MYGUI_OBSOLETE(" is deprecated, use : bool PointerManager::isVisible()")
		bool isShow();
		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);
	};

	template <>
	class MemberObsolete<ResourceManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : size_t ResourceManager::getCount()")
		size_t getResourceCount();
		MYGUI_OBSOLETE(" is deprecated, use : IResourcePtr ResourceManager::getByName(const std::string& _name, bool _throw)")
		IResourcePtr getResource(const std::string& _name, bool _throw = true);
	};

	template <>
	class MemberObsolete<SkinManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : ResourceSkin* SkinManager::getByName(const std::string& _name)")
		ResourceSkin* getSkin(const std::string& _name);
		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);
	};

	template <>
	class MemberObsolete<WidgetManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : void WidgetManager::destroyWidgets(VectorWidgetPtr &_widgets)")
		void destroyWidgetsVector(VectorWidgetPtr& _widgets);
		MYGUI_OBSOLETE(" is deprecated")
		Widget* findWidgetT(const std::string& _name, bool _throw = true);
		MYGUI_OBSOLETE(" is deprecated")
		Widget* findWidgetT(const std::string& _name, const std::string& _prefix, bool _throw = true);
		MYGUI_OBSOLETE(" is deprecated, use : void Widget::setProperty(const std::string &_key, const std::string &_value)")
		void parse(Widget* _widget, const std::string& _key, const std::string& _value);
	};

#endif // MYGUI_DONT_USE_OBSOLETE

	class BackwardCompatibility
	{
	public:
		static bool checkProperty(Widget* _owner, std::string& _key, std::string& _value);
		static void initialise();
		static void shutdown();
		static bool isIgnoreProperty(const std::string& _key);
		static std::string MYGUI_EXPORT getPropertyRename(const std::string& _propertyName);

	private:
#ifndef MYGUI_DONT_USE_OBSOLETE
		typedef std::map<std::string, std::string> MapString;
		typedef std::set<std::string> SetString;
		static MapString mPropertyRename;
		static SetString mPropertyIgnore;
#endif // MYGUI_DONT_USE_OBSOLETE
	};

} // namespace MyGUI

#endif // __MYGUI_BACKWARD_COMPATIBILITY_H__
