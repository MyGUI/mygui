/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
	class MYGUI_EXPORT MemberObsolete<Button>
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
		MYGUI_OBSOLETE(" is deprecated")
		ImageBox* getStaticImage();
		MYGUI_OBSOLETE(" is deprecated, use : void Button::setImageName(const std::string& _name)")
		void setImageIndex(size_t _value);
		MYGUI_OBSOLETE(" is deprecated")
		size_t getImageIndex();
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<ComboBox>
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
	class MYGUI_EXPORT MemberObsolete<EditBox>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : void EditBox::setVisibleVScroll(bool _visible)")
		void showVScroll(bool _visible);
		MYGUI_OBSOLETE(" is deprecated, use : bool EditBox::isVisibleVScroll()")
		bool isShowVScroll();
		MYGUI_OBSOLETE(" is deprecated, use : void EditBox::setVisibleHScroll(bool _visible)")
		void showHScroll(bool _visible);
		MYGUI_OBSOLETE(" is deprecated, use : bool EditBox::isVisibleHScroll()")
		bool isShowHScroll();

		MYGUI_OBSOLETE(" is deprecated, use : void EditBox::setTextIntervalColour(size_t _start, size_t _count, const Colour& _colour)")
		void setTextColour(size_t _start, size_t _count, const Colour& _colour);
		MYGUI_OBSOLETE(" is deprecated, use : size_t EditBox::getTextSelectionStart() , size_t getTextSelectionEnd()")
		void getTextSelect(size_t& _start, size_t& _end);
		MYGUI_OBSOLETE(" is deprecated, use : UString EditBox::getTextInterval(size_t _start, size_t _count)")
		UString getText(size_t _start, size_t _count);
		MYGUI_OBSOLETE(" is deprecated, use : void EditBox::setTextSelection(size_t _start, size_t _end)")
		void setTextSelect(size_t _start, size_t _end);
		MYGUI_OBSOLETE(" is deprecated, use : void EditBox::deleteTextSelection()")
		void deleteTextSelect();
		MYGUI_OBSOLETE(" is deprecated, use : UString EditBox::getTextSelection()")
		UString getSelectedText();
		MYGUI_OBSOLETE(" is deprecated, use : bool EditBox::isTextSelection()")
		bool isTextSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void EditBox::setTextSelectionColour(const Colour& _colour)")
		void setTextSelectColour(const Colour& _colour);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<ItemBox>
	{
	public:
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

		MYGUI_OBSOLETE(" is deprecated, use : void ItemBox::setVerticalAlignment(bool _value)")
		void setItemBoxAlignVert(bool _value);
		MYGUI_OBSOLETE(" is deprecated, use : bool ItemBox::getVerticalAlignment() const")
		bool getItemBoxAlignVert();
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<ListBox>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : size_t ListBox::getIndexSelected()")
		size_t getItemIndexSelected();
		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::clearIndexSelected()")
		void clearItemSelected();

		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::insertItemAt(size_t _index, const UString& _name)")
		void insertItem(size_t _index, const UString& _item);
		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::setItemNameAt(size_t _index, const UString& _name)")
		void setItem(size_t _index, const UString& _item);
		MYGUI_OBSOLETE(" is deprecated, use : const UString& ListBox::getItemNameAt(size_t _index)")
		const UString& getItem(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::removeItemAt(size_t _index)")
		void deleteItem(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::removeAllItems()")
		void deleteAllItems();
		MYGUI_OBSOLETE(" is deprecated, use : size_t ListBox::findItemIndexWith(const UString& _name)")
		size_t findItem(const UString& _item);
		MYGUI_OBSOLETE(" is deprecated, use : size_t ListBox::getIndexSelected()")
		size_t getItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::clearIndexSelected()")
		void resetItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::setIndexSelected(size_t _index)")
		void setItemSelect(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::beginToItemAt(size_t _index)")
		void beginToIndex(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::beginToItemFirst()")
		void beginToStart();
		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::beginToItemLast()")
		void beginToEnd();
		MYGUI_OBSOLETE(" is deprecated, use : void ListBox::beginToItemSelected()")
		void beginToSelect();
		MYGUI_OBSOLETE(" is deprecated, use : bool ListBox::isItemVisibleAt(size_t _index, bool _fill)")
		bool isItemVisible(size_t _index, bool _fill = true);
		MYGUI_OBSOLETE(" is deprecated, use : bool ListBox::isItemSelectedVisible(bool _fill)")
		bool isItemSelectVisible(bool _fill = true);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<MenuControl>
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

		MYGUI_OBSOLETE(" is deprecated, use : void MenuControl::setVerticalAlignment(bool _value)")
		void setAlignVert(bool _value);
		MYGUI_OBSOLETE(" is deprecated, use : bool MenuControl::getVerticalAlignment() const")
		bool getAlignVert();
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<MenuItem>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : void setItemChildVisible(bool _visible)")
		void showItemChild();
		MYGUI_OBSOLETE(" is deprecated, use : void setItemChildVisible(bool _visible)")
		void hideItemChild();
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<MultiListBox>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : size_t MultiListBox::getIndexSelected()")
		size_t getItemIndexSelected();
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::clearIndexSelected()")
		void clearItemSelected();

		MYGUI_OBSOLETE(" is deprecated, use : size_t MultiListBox::findSubItemWith(size_t _column, const UString& _name)")
		size_t findItem(size_t _column, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : const UString& MultiListBox::getSubItemNameAt(size_t _column, size_t _index)")
		const UString& getSubItem(size_t _column, size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::setSubItemNameAt(size_t _column, size_t _index, const UString& _name)")
		void setSubItem(size_t _column, size_t _index, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::removeColumnAt(size_t _column)")
		void deleteColumn(size_t _column);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::removeAllColumns()")
		void deleteAllColumns();
		MYGUI_OBSOLETE(" is deprecated, use : int MultiListBox::getColumnWidthAt(size_t _column)")
		int getColumnWidth(size_t _column);
		MYGUI_OBSOLETE(" is deprecated, use : const UString& MultiListBox::getColumnNameAt(size_t _column)")
		const UString& getColumnName(size_t _column);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::setColumnWidthAt(size_t _column, int _width)")
		void setColumnWidth(size_t _column, int _width);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::addColumn(const UString& _name, int _width, Any _data)")
		void addColumn(int _width, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::setColumnNameAt(size_t _column, const UString& _name)")
		void setColumnName(size_t _column, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::insertColumnAt(size_t _column, const UString& _name, int _width, Any _data)")
		void insertColumn(size_t _column, int _width, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : size_t MultiListBox::getIndexSelected()")
		size_t getItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::clearIndexSelected()")
		void resetItemSelect();
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::setIndexSelected(size_t _index)")
		void setItemSelect(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::insertItemAt(size_t _index, const UString& _name, Any _data)")
		void insertItem(size_t _index, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::setItemNameAt(size_t _index, const UString& _name)")
		void setItem(size_t _index, const UString& _name);
		MYGUI_OBSOLETE(" is deprecated, use : const UString& MultiListBox::getItemNameAt(size_t _index)")
		const UString& getItem(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::removeItemAt(size_t _index)")
		void deleteItem(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void MultiListBox::removeAllItems()")
		void deleteAllItems();
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<ProgressBar>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : void ProgressBar::setFlowDirection(FlowDirection _value)")
		void setProgressStartPoint(Align _value);
		MYGUI_OBSOLETE(" is deprecated, use : FlowDirection ProgressBar::getFlowDirection()")
		Align getProgressStartPoint();
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<ScrollView>
	{
	public:
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
	class MYGUI_EXPORT MemberObsolete<TabControl>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : int TabControl::getButtonWidthAt(size_t _index)")
		int getSheetButtonWidthIndex(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : int TabControl::getButtonWidth(TabItem* _item)")
		int getSheetButtonWidth(TabItem* _sheet);
		MYGUI_OBSOLETE(" is deprecated, use : void TabControl::setButtonWidthAt(size_t _index, int _width)")
		void setSheetButtonWidthIndex(size_t _index, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : void TabControl::setButtonWidth(TabItem* _item, int _width)")
		void setSheetButtonWidth(TabItem* _sheet, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : void TabControl::beginToItemAt(size_t _index)")
		void showBarButton(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void TabControl::beginToItemSelected()")
		void showBarSelectButton();
		MYGUI_OBSOLETE(" is deprecated, use : size_t TabControl::getItemCount()")
		size_t getSheetCount();
		MYGUI_OBSOLETE(" is deprecated, use : const UString& TabControl::getItemName(TabItem* _item)")
		const UString& getSheetName(TabItem* _sheet);
		MYGUI_OBSOLETE(" is deprecated, use : const UString& TabControl::getItemNameAt(size_t _index)")
		const UString& getSheetNameIndex(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : TabItem* TabControl::getItemAt(size_t _index)")
		TabItem* getSheet(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void TabControl::setItemNameAt(size_t _index, const UString& _name)")
		void setSheetNameIndex(size_t _index, const UString& _name, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : void TabControl::setItemName(TabItem* _item, const UString& _name)")
		void setSheetName(TabItem* _sheet, const UString& _name, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : TabItem* TabControl::addItem(const UString& _name, Any _data)")
		TabItem* addSheet(const UString& _name, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : TabItem* TabControl::insertItemAt(size_t _index, const UString& _name, Any _data)")
		TabItem* insertSheet(size_t _index, const UString& _name, int _width = DEFAULT);
		MYGUI_OBSOLETE(" is deprecated, use : void TabControl::removeItemAt(size_t _index)")
		void removeSheetIndex(size_t _index);
		MYGUI_OBSOLETE(" is deprecated, use : void TabControl::removeItem(TabItem* _item)")
		void removeSheet(TabItem* _sheet);
		MYGUI_OBSOLETE(" is deprecated, use : void TabControl::setIndexSelected(size_t _index)")
		void selectSheetIndex(size_t _index, bool _smooth = true);
		MYGUI_OBSOLETE(" is deprecated, use : void TabControl::setItemSelected(TabItem* _item)")
		void selectSheet(TabItem* _sheet, bool _smooth = true);
		MYGUI_OBSOLETE(" is deprecated, use : size_t TabControl::getIndexSelected()")
		size_t getSelectSheetIndex();

		MYGUI_OBSOLETE(" is deprecated, use : size_t Widget::getIndexSelected()")
		size_t getItemIndexSelected();
		MYGUI_OBSOLETE(" is deprecated, use : void Widget::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<Widget>
	{
	public:
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
	class MYGUI_EXPORT MemberObsolete<Window>
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
	class MYGUI_EXPORT MemberObsolete<FontManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);
		void loadOldFontFormat(xml::ElementPtr _node, const std::string& _file, Version _version, const std::string& _tag);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<Gui>
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

		MYGUI_OBSOLETE(" is deprecated, use : RenderManager::onFrameEvent(float _time)")
		void _injectFrameEntered(float _time);
		MYGUI_OBSOLETE(" is deprecated, use : RenderManager::onResizeView(const IntSize& _size)")
		void _resizeWindow(const IntSize& _size);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<InputManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : const IntPoint& getLastPressedPosition(MouseButton _id) with _id MouseButton::Left")
		const IntPoint& getLastLeftPressed() const;
		MYGUI_OBSOLETE(" is deprecated, use : const IntPoint& getLastPressedPosition(MouseButton _id) with _id MouseButton::Right")
		const IntPoint& getLastRightPressed() const;
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<LanguageManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<LayerManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<LayoutManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : VectorWidgetPtr& LayoutManager::loadLayout(const std::string& _file, const std::string& _prefix, Widget* _parent)")
		VectorWidgetPtr load(const std::string& _file);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<PluginManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<PointerManager>
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

		void loadOldPointerFormat(xml::ElementPtr _node, const std::string& _file, Version _version, const std::string& _tag);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<ResourceManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : size_t ResourceManager::getCount()")
		size_t getResourceCount();
		MYGUI_OBSOLETE(" is deprecated, use : IResourcePtr ResourceManager::getByName(const std::string& _name, bool _throw)")
		IResourcePtr getResource(const std::string& _name, bool _throw = true);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<SkinManager>
	{
	public:
		MYGUI_OBSOLETE(" is deprecated, use : ResourceSkin* SkinManager::getByName(const std::string& _name)")
		ResourceSkin* getSkin(const std::string& _name);
		MYGUI_OBSOLETE(" is deprecated, use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);

		void loadOldSkinFormat(xml::ElementPtr _node, const std::string& _file, Version _version, const std::string& _tag);
	};

	template <>
	class MYGUI_EXPORT MemberObsolete<WidgetManager>
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

	class MYGUI_EXPORT BackwardCompatibility
	{
	public:
		static bool checkProperty(Widget* _owner, std::string& _key, std::string& _value);
		static void initialise();
		static void shutdown();
		static bool isIgnoreProperty(const std::string& _key);
		static std::string getPropertyRename(const std::string& _propertyName);
		static std::string getFactoryRename(const std::string& _categoryName, const std::string& _factoryName);
		static std::string getSkinRename(const std::string& _skinName);
		static void registerWidgetTypes();
	};

} // namespace MyGUI

#endif // __MYGUI_BACKWARD_COMPATIBILITY_H__
