/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_BackwardCompatibility.h"
#include "MyGUI_Button.h"
#include "MyGUI_ComboBox.h"
#include "MyGUI_EditBox.h"
#include "MyGUI_ItemBox.h"
#include "MyGUI_ListBox.h"
#include "MyGUI_MenuControl.h"
#include "MyGUI_MenuItem.h"
#include "MyGUI_MultiListBox.h"
#include "MyGUI_ProgressBar.h"
#include "MyGUI_ScrollView.h"
#include "MyGUI_TabControl.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Window.h"
#include "MyGUI_TabItem.h"
#include "MyGUI_Canvas.h"
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
#include "MyGUI_DeprecatedWidgets.h"

namespace MyGUI
{

#ifndef MYGUI_DONT_USE_OBSOLETE

	void MemberObsolete<Button>::setButtonPressed(bool _value)
	{
		static_cast<Button*>(this)->setStateSelected(_value);
	}
	bool MemberObsolete<Button>::getButtonPressed()
	{
		return static_cast<Button*>(this)->getStateSelected();
	}
	void MemberObsolete<Button>::setStateCheck(bool _value)
	{
		static_cast<Button*>(this)->setStateSelected(_value);
	}
	bool MemberObsolete<Button>::getStateCheck()
	{
		return static_cast<Button*>(this)->getStateSelected();
	}
	ImageBox* MemberObsolete<Button>::getStaticImage()
	{
		return static_cast<Button*>(this)->_getImageBox();
	}
	void MemberObsolete<Button>::setImageIndex(size_t _index)
	{
		if (static_cast<Button*>(this)->_getImageBox())
			static_cast<Button*>(this)->_getImageBox()->setImageIndex(_index);
	}
	size_t MemberObsolete<Button>::getImageIndex()
	{
		if (static_cast<Button*>(this)->_getImageBox())
			return static_cast<Button*>(this)->_getImageBox()->getImageIndex();
		return ITEM_NONE;
	}


	size_t MemberObsolete<ComboBox>::getItemIndexSelected()
	{
		return static_cast<ComboBox*>(this)->getIndexSelected();
	}
	void MemberObsolete<ComboBox>::setItemSelectedAt(size_t _index)
	{
		static_cast<ComboBox*>(this)->setIndexSelected(_index);
	}
	void MemberObsolete<ComboBox>::clearItemSelected()
	{
		static_cast<ComboBox*>(this)->clearIndexSelected();
	}

	void MemberObsolete<ComboBox>::insertItem(size_t _index, const UString& _name)
	{
		static_cast<ComboBox*>(this)->insertItemAt(_index, _name);
	}
	void MemberObsolete<ComboBox>::setItem(size_t _index, const UString& _item)
	{
		static_cast<ComboBox*>(this)->setItemNameAt(_index, _item);
	}
	const UString& MemberObsolete<ComboBox>::getItem(size_t _index)
	{
		return static_cast<ComboBox*>(this)->getItemNameAt(_index);
	}
	void MemberObsolete<ComboBox>::deleteItem(size_t _index)
	{
		static_cast<ComboBox*>(this)->removeItemAt(_index);
	}
	void MemberObsolete<ComboBox>::deleteAllItems()
	{
		static_cast<ComboBox*>(this)->removeAllItems();
	}
	size_t MemberObsolete<ComboBox>::getItemSelect()
	{
		return static_cast<ComboBox*>(this)->getIndexSelected();
	}
	void MemberObsolete<ComboBox>::resetItemSelect()
	{
		static_cast<ComboBox*>(this)->clearIndexSelected();
	}
	void MemberObsolete<ComboBox>::setItemSelect(size_t _index)
	{
		static_cast<ComboBox*>(this)->setIndexSelected(_index);
	}

	void MemberObsolete<ComboBox>::setMaxListHeight(int _value)
	{
		static_cast<ComboBox*>(this)->setMaxListLength(_value);
	}
	int MemberObsolete<ComboBox>::getMaxListHeight()
	{
		return static_cast<ComboBox*>(this)->getMaxListLength();
	}


	void MemberObsolete<EditBox>::showVScroll(bool _visible)
	{
		static_cast<EditBox*>(this)->setVisibleVScroll(_visible);
	}
	bool MemberObsolete<EditBox>::isShowVScroll()
	{
		return static_cast<EditBox*>(this)->isVisibleVScroll();
	}
	void MemberObsolete<EditBox>::showHScroll(bool _visible)
	{
		static_cast<EditBox*>(this)->setVisibleHScroll(_visible);
	}
	bool MemberObsolete<EditBox>::isShowHScroll()
	{
		return static_cast<EditBox*>(this)->isVisibleHScroll();
	}

	void MemberObsolete<EditBox>::setTextColour(size_t _start, size_t _count, const Colour& _colour)
	{
		static_cast<EditBox*>(this)->setTextIntervalColour(_start, _count, _colour);
	}
	void MemberObsolete<EditBox>::getTextSelect(size_t& _start, size_t& _end)
	{
		_start = static_cast<EditBox*>(this)->getTextSelectionStart();
		_end = static_cast<EditBox*>(this)->getTextSelectionEnd();
	}
	UString MemberObsolete<EditBox>::getText(size_t _start, size_t _count)
	{
		return static_cast<EditBox*>(this)->getTextInterval(_start, _count);
	}
	void MemberObsolete<EditBox>::setTextSelect(size_t _start, size_t _end)
	{
		static_cast<EditBox*>(this)->setTextSelection(_start, _end);
	}
	void MemberObsolete<EditBox>::deleteTextSelect()
	{
		static_cast<EditBox*>(this)->deleteTextSelection();
	}
	UString MemberObsolete<EditBox>::getSelectedText()
	{
		return static_cast<EditBox*>(this)->getTextSelection();
	}
	bool MemberObsolete<EditBox>::isTextSelect()
	{
		return static_cast<EditBox*>(this)->isTextSelection();
	}
	void MemberObsolete<EditBox>::setTextSelectColour(const Colour& _colour)
	{
		static_cast<EditBox*>(this)->setTextSelectionColour(_colour);
	}


	size_t MemberObsolete<ItemBox>::getItemIndexSelected()
	{
		return static_cast<ItemBox*>(this)->getIndexSelected();
	}
	void MemberObsolete<ItemBox>::setItemSelectedAt(size_t _index)
	{
		static_cast<ItemBox*>(this)->setIndexSelected(_index);
	}
	void MemberObsolete<ItemBox>::clearItemSelected()
	{
		static_cast<ItemBox*>(this)->clearIndexSelected();
	}

	void MemberObsolete<ItemBox>::insertItem(size_t _index, Any _data)
	{
		static_cast<ItemBox*>(this)->insertItemAt(_index, _data);
	}
	void MemberObsolete<ItemBox>::setItemData(size_t _index, Any _data)
	{
		static_cast<ItemBox*>(this)->setItemDataAt(_index, _data);
	}
	void MemberObsolete<ItemBox>::deleteItem(size_t _index)
	{
		static_cast<ItemBox*>(this)->removeItemAt(_index);
	}
	void MemberObsolete<ItemBox>::deleteAllItems()
	{
		static_cast<ItemBox*>(this)->removeAllItems();
	}
	size_t MemberObsolete<ItemBox>::getItemSelect()
	{
		return static_cast<ItemBox*>(this)->getIndexSelected();
	}
	void MemberObsolete<ItemBox>::resetItemSelect()
	{
		static_cast<ItemBox*>(this)->clearIndexSelected();
	}
	void MemberObsolete<ItemBox>::setItemSelect(size_t _index)
	{
		static_cast<ItemBox*>(this)->setIndexSelected(_index);
	}

	Widget* MemberObsolete<ItemBox>::getWidgetDrop()
	{
		return static_cast<ItemBox*>(this)->getWidgetDrag();
	}
	void MemberObsolete<ItemBox>::resetDrop()
	{
		static_cast<ItemBox*>(this)->resetDrag();
	}

	void MemberObsolete<ItemBox>::setItemBoxAlignVert(bool _value)
	{
		static_cast<ItemBox*>(this)->setVerticalAlignment(_value);
	}
	bool MemberObsolete<ItemBox>::getItemBoxAlignVert()
	{
		return static_cast<ItemBox*>(this)->getVerticalAlignment();
	}


	size_t MemberObsolete<ListBox>::getItemIndexSelected()
	{
		return static_cast<ListBox*>(this)->getIndexSelected();
	}
	void MemberObsolete<ListBox>::setItemSelectedAt(size_t _index)
	{
		static_cast<ListBox*>(this)->setIndexSelected(_index);
	}
	void MemberObsolete<ListBox>::clearItemSelected()
	{
		static_cast<ListBox*>(this)->clearIndexSelected();
	}

	void MemberObsolete<ListBox>::insertItem(size_t _index, const UString& _item)
	{
		static_cast<ListBox*>(this)->insertItemAt(_index, _item);
	}
	void MemberObsolete<ListBox>::setItem(size_t _index, const UString& _item)
	{
		static_cast<ListBox*>(this)->setItemNameAt(_index, _item);
	}
	const UString& MemberObsolete<ListBox>::getItem(size_t _index)
	{
		return static_cast<ListBox*>(this)->getItemNameAt(_index);
	}
	void MemberObsolete<ListBox>::deleteItem(size_t _index)
	{
		static_cast<ListBox*>(this)->removeItemAt(_index);
	}
	void MemberObsolete<ListBox>::deleteAllItems()
	{
		static_cast<ListBox*>(this)->removeAllItems();
	}
	size_t MemberObsolete<ListBox>::findItem(const UString& _item)
	{
		return static_cast<ListBox*>(this)->findItemIndexWith(_item);
	}
	size_t MemberObsolete<ListBox>::getItemSelect()
	{
		return static_cast<ListBox*>(this)->getIndexSelected();
	}
	void MemberObsolete<ListBox>::resetItemSelect()
	{
		static_cast<ListBox*>(this)->clearIndexSelected();
	}
	void MemberObsolete<ListBox>::setItemSelect(size_t _index)
	{
		static_cast<ListBox*>(this)->setIndexSelected(_index);
	}
	void MemberObsolete<ListBox>::beginToIndex(size_t _index)
	{
		static_cast<ListBox*>(this)->beginToItemAt(_index);
	}
	void MemberObsolete<ListBox>::beginToStart()
	{
		static_cast<ListBox*>(this)->beginToItemFirst();
	}
	void MemberObsolete<ListBox>::beginToEnd()
	{
		static_cast<ListBox*>(this)->beginToItemLast();
	}
	void MemberObsolete<ListBox>::beginToSelect()
	{
		static_cast<ListBox*>(this)->beginToItemSelected();
	}
	bool MemberObsolete<ListBox>::isItemVisible(size_t _index, bool _fill)
	{
		return static_cast<ListBox*>(this)->isItemVisibleAt(_index, _fill);
	}
	bool MemberObsolete<ListBox>::isItemSelectVisible(bool _fill)
	{
		return static_cast<ListBox*>(this)->isItemSelectedVisible(_fill);
	}


	void MemberObsolete<MenuControl>::showMenu()
	{
		static_cast<MenuControl*>(this)->setVisible(true);
	}
	void MemberObsolete<MenuControl>::hideMenu()
	{
		static_cast<MenuControl*>(this)->setVisible(false);
	}
	bool MemberObsolete<MenuControl>::isShowMenu()
	{
		return static_cast<MenuControl*>(this)->getVisible();
	}

	void MemberObsolete<MenuControl>::showItemChildAt(size_t _index)
	{
		static_cast<MenuControl*>(this)->setItemChildVisibleAt(_index, true);
	}
	void MemberObsolete<MenuControl>::showItemChild(MenuItem* _item)
	{
		static_cast<MenuControl*>(this)->setItemChildVisible(_item, true);
	}
	void MemberObsolete<MenuControl>::hideItemChildAt(size_t _index)
	{
		static_cast<MenuControl*>(this)->setItemChildVisibleAt(_index, false);
	}
	void MemberObsolete<MenuControl>::hideItemChild(MenuItem* _item)
	{
		static_cast<MenuControl*>(this)->setItemChildVisible(_item, false);
	}

	void MemberObsolete<MenuControl>::setAlignVert(bool _value)
	{
		static_cast<MenuControl*>(this)->setVerticalAlignment(_value);
	}
	bool MemberObsolete<MenuControl>::getAlignVert()
	{
		return static_cast<MenuControl*>(this)->getVerticalAlignment();
	}

	void MemberObsolete<MenuItem>::showItemChild()
	{
		static_cast<MenuItem*>(this)->setItemChildVisible(true);
	}
	void MemberObsolete<MenuItem>::hideItemChild()
	{
		static_cast<MenuItem*>(this)->setItemChildVisible(false);
	}


	size_t MemberObsolete<MultiListBox>::getItemIndexSelected()
	{
		return static_cast<MultiListBox*>(this)->getIndexSelected();
	}
	void MemberObsolete<MultiListBox>::setItemSelectedAt(size_t _index)
	{
		static_cast<MultiListBox*>(this)->setIndexSelected(_index);
	}
	void MemberObsolete<MultiListBox>::clearItemSelected()
	{
		static_cast<MultiListBox*>(this)->clearIndexSelected();
	}

	size_t MemberObsolete<MultiListBox>::findItem(size_t _column, const UString& _name)
	{
		return static_cast<MultiListBox*>(this)->findSubItemWith(_column, _name);
	}
	const UString& MemberObsolete<MultiListBox>::getSubItem(size_t _column, size_t _index)
	{
		return static_cast<MultiListBox*>(this)->getSubItemNameAt(_column, _index);
	}
	void MemberObsolete<MultiListBox>::setSubItem(size_t _column, size_t _index, const UString& _name)
	{
		static_cast<MultiListBox*>(this)->setSubItemNameAt(_column, _index, _name);
	}
	void MemberObsolete<MultiListBox>::deleteColumn(size_t _column)
	{
		static_cast<MultiListBox*>(this)->removeColumnAt(_column);
	}
	void MemberObsolete<MultiListBox>::deleteAllColumns()
	{
		static_cast<MultiListBox*>(this)->removeAllColumns();
	}
	int MemberObsolete<MultiListBox>::getColumnWidth(size_t _column)
	{
		return static_cast<MultiListBox*>(this)->getColumnWidthAt(_column);
	}
	const UString& MemberObsolete<MultiListBox>::getColumnName(size_t _column)
	{
		return static_cast<MultiListBox*>(this)->getColumnNameAt(_column);
	}
	void MemberObsolete<MultiListBox>::setColumnWidth(size_t _column, int _width)
	{
		static_cast<MultiListBox*>(this)->setColumnWidthAt(_column, _width);
	}
	void MemberObsolete<MultiListBox>::addColumn(int _width, const UString& _name)
	{
		static_cast<MultiListBox*>(this)->addColumn(_name, _width);
	}
	void MemberObsolete<MultiListBox>::setColumnName(size_t _column, const UString& _name)
	{
		static_cast<MultiListBox*>(this)->setColumnNameAt(_column, _name);
	}
	void MemberObsolete<MultiListBox>::insertColumn(size_t _column, int _width, const UString& _name)
	{
		static_cast<MultiListBox*>(this)->insertColumnAt(_column, _name, _width);
	}
	size_t MemberObsolete<MultiListBox>::getItemSelect()
	{
		return static_cast<MultiListBox*>(this)->getIndexSelected();
	}
	void MemberObsolete<MultiListBox>::resetItemSelect()
	{
		static_cast<MultiListBox*>(this)->clearIndexSelected();
	}
	void MemberObsolete<MultiListBox>::setItemSelect(size_t _index)
	{
		static_cast<MultiListBox*>(this)->setIndexSelected(_index);
	}
	void MemberObsolete<MultiListBox>::insertItem(size_t _index, const UString& _name)
	{
		static_cast<MultiListBox*>(this)->insertItemAt(_index, _name);
	}
	void MemberObsolete<MultiListBox>::setItem(size_t _index, const UString& _name)
	{
		static_cast<MultiListBox*>(this)->setItemNameAt(_index, _name);
	}
	const UString& MemberObsolete<MultiListBox>::getItem(size_t _index)
	{
		return static_cast<MultiListBox*>(this)->getItemNameAt(_index);
	}
	void MemberObsolete<MultiListBox>::deleteItem(size_t _index)
	{
		static_cast<MultiListBox*>(this)->removeItemAt(_index);
	}
	void MemberObsolete<MultiListBox>::deleteAllItems()
	{
		static_cast<MultiListBox*>(this)->removeAllItems();
	}


	void MemberObsolete<ProgressBar>::setProgressStartPoint(Align _value)
	{
		if (_value == Align::Right)
			static_cast<ProgressBar*>(this)->setFlowDirection(FlowDirection::RightToLeft);
		else if (_value == Align::Top)
			static_cast<ProgressBar*>(this)->setFlowDirection(FlowDirection::TopToBottom);
		else if (_value == Align::Bottom)
			static_cast<ProgressBar*>(this)->setFlowDirection(FlowDirection::BottomToTop);
		else
			static_cast<ProgressBar*>(this)->setFlowDirection(FlowDirection::LeftToRight);
	}

	Align MemberObsolete<ProgressBar>::getProgressStartPoint()
	{
		if (static_cast<ProgressBar*>(this)->getFlowDirection() == FlowDirection::RightToLeft)
			return Align::Right;
		else if (static_cast<ProgressBar*>(this)->getFlowDirection() == FlowDirection::TopToBottom)
			return Align::Top;
		else if (static_cast<ProgressBar*>(this)->getFlowDirection() == FlowDirection::BottomToTop)
			return Align::Bottom;

		return Align::Left;
	}


	void MemberObsolete<ScrollView>::showVScroll(bool _visible)
	{
		static_cast<ScrollView*>(this)->setVisibleVScroll(_visible);
	}
	bool MemberObsolete<ScrollView>::isShowVScroll()
	{
		return static_cast<ScrollView*>(this)->isVisibleVScroll();
	}
	void MemberObsolete<ScrollView>::showHScroll(bool _visible)
	{
		static_cast<ScrollView*>(this)->setVisibleHScroll(_visible);
	}
	bool MemberObsolete<ScrollView>::isShowHScroll()
	{
		return static_cast<ScrollView*>(this)->isVisibleHScroll();
	}


	int MemberObsolete<TabControl>::getSheetButtonWidthIndex(size_t _index)
	{
		return static_cast<TabControl*>(this)->getButtonWidthAt(_index);
	}
	int MemberObsolete<TabControl>::getSheetButtonWidth(TabItem* _sheet)
	{
		return static_cast<TabControl*>(this)->getButtonWidth(_sheet);
	}
	void MemberObsolete<TabControl>::setSheetButtonWidthIndex(size_t _index, int _width)
	{
		static_cast<TabControl*>(this)->setButtonWidthAt(_index, _width);
	}
	void MemberObsolete<TabControl>::setSheetButtonWidth(TabItem* _sheet, int _width)
	{
		static_cast<TabControl*>(this)->setButtonWidth(_sheet, _width);
	}
	void MemberObsolete<TabControl>::showBarButton(size_t _index)
	{
		static_cast<TabControl*>(this)->beginToItemAt(_index);
	}
	void MemberObsolete<TabControl>::showBarSelectButton()
	{
		static_cast<TabControl*>(this)->beginToItemSelected();
	}
	size_t MemberObsolete<TabControl>::getSheetCount()
	{
		return static_cast<TabControl*>(this)->getItemCount();
	}
	const UString& MemberObsolete<TabControl>::getSheetName(TabItem* _sheet)
	{
		return static_cast<TabControl*>(this)->getItemName(_sheet);
	}
	const UString& MemberObsolete<TabControl>::getSheetNameIndex(size_t _index)
	{
		return static_cast<TabControl*>(this)->getItemNameAt(_index);
	}
	TabItem* MemberObsolete<TabControl>::getSheet(size_t _index)
	{
		return static_cast<TabControl*>(this)->getItemAt(_index);
	}
	void MemberObsolete<TabControl>::setSheetNameIndex(size_t _index, const UString& _name, int _width)
	{
		static_cast<TabControl*>(this)->setItemNameAt(_index, _name);
	}
	void MemberObsolete<TabControl>::setSheetName(TabItem* _sheet, const UString& _name, int _width)
	{
		static_cast<TabControl*>(this)->setItemName(_sheet, _name);
	}
	TabItem* MemberObsolete<TabControl>::addSheet(const UString& _name, int _width)
	{
		return static_cast<TabControl*>(this)->addItem(_name, _width);
	}
	TabItem* MemberObsolete<TabControl>::insertSheet(size_t _index, const UString& _name, int _width)
	{
		return static_cast<TabControl*>(this)->insertItemAt(_index, _name);
	}
	void MemberObsolete<TabControl>::removeSheetIndex(size_t _index)
	{
		static_cast<TabControl*>(this)->removeItemAt(_index);
	}
	void MemberObsolete<TabControl>::removeSheet(TabItem* _sheet)
	{
		static_cast<TabControl*>(this)->removeItem(_sheet);
	}
	void MemberObsolete<TabControl>::selectSheetIndex(size_t _index, bool _smooth)
	{
		static_cast<TabControl*>(this)->setIndexSelected(_index);
	}
	void MemberObsolete<TabControl>::selectSheet(TabItem* _sheet, bool _smooth)
	{
		static_cast<TabControl*>(this)->setItemSelected(_sheet);
	}
	size_t MemberObsolete<TabControl>::getSelectSheetIndex()
	{
		return static_cast<TabControl*>(this)->getIndexSelected();
	}

	size_t MemberObsolete<TabControl>::getItemIndexSelected()
	{
		return static_cast<TabControl*>(this)->getIndexSelected();
	}
	void MemberObsolete<TabControl>::setItemSelectedAt(size_t _index)
	{
		static_cast<TabControl*>(this)->setIndexSelected(_index);
	}


	bool MemberObsolete<Widget>::isEnabled()
	{
		return static_cast<Widget*>(this)->getEnabled();
	}
	bool MemberObsolete<Widget>::isInheritsAlpha()
	{
		return static_cast<Widget*>(this)->getInheritsAlpha();
	}
	bool MemberObsolete<Widget>::isNeedKeyFocus()
	{
		return static_cast<Widget*>(this)->getNeedKeyFocus();
	}
	bool MemberObsolete<Widget>::isNeedMouseFocus()
	{
		return static_cast<Widget*>(this)->getNeedMouseFocus();
	}
	bool MemberObsolete<Widget>::isInheritsPick()
	{
		return static_cast<Widget*>(this)->getInheritsPick();
	}
	bool MemberObsolete<Widget>::isVisible()
	{
		return static_cast<Widget*>(this)->getVisible();
	}


	void MemberObsolete<Window>::showSmooth(bool _reset)
	{
		static_cast<Window*>(this)->setVisibleSmooth(true);
	}
	void MemberObsolete<Window>::hideSmooth()
	{
		static_cast<Window*>(this)->setVisibleSmooth(false);
	}
	void MemberObsolete<Window>::setMinMax(const IntRect& _minmax)
	{
		static_cast<Window*>(this)->setMinSize(_minmax.left, _minmax.top);
		static_cast<Window*>(this)->setMaxSize(_minmax.right, _minmax.bottom);
	}
	void MemberObsolete<Window>::setMinMax(int _min_w, int _min_h, int _max_w, int _max_h)
	{
		static_cast<Window*>(this)->setMinSize(_min_w, _min_h);
		static_cast<Window*>(this)->setMaxSize(_max_w, _max_h);
	}
	IntRect MemberObsolete<Window>::getMinMax()
	{
		return IntRect(static_cast<Window*>(this)->getMinSize().width, static_cast<Window*>(this)->getMinSize().height, static_cast<Window*>(this)->getMaxSize().width, static_cast<Window*>(this)->getMaxSize().height);
	}

	bool MemberObsolete<FontManager>::load(const std::string& _file)
	{
		return ResourceManager::getInstance().load(_file);
	}

	void MemberObsolete<FontManager>::loadOldFontFormat(xml::ElementPtr _node2, const std::string& _file, Version _version, const std::string& _tag)
	{
		xml::ElementEnumerator _node = _node2->getElementEnumerator();
		while (_node.next())
		{
			if (_node->getName() == _tag)
			{
				std::string name;
				if (!_node->findAttribute("name", name))
					return;

				std::string type;
				if (type.empty())
				{
					if (_node->findAttribute("resolution").empty())
						type = "ResourceManualFont";
					else
						type = "ResourceTrueTypeFont";
				}

				xml::Document doc;
				xml::ElementPtr root = doc.createRoot("MyGUI");
				xml::ElementPtr node = root->createChild("Resource");
				node->addAttribute("type", type);
				node->addAttribute("name", name);

				std::string tmp;
				if (_node->findAttribute("source", tmp))
				{
					xml::ElementPtr prop = node->createChild("Property");
					prop->addAttribute("key", "Source");
					prop->addAttribute("value", tmp);
				}

				if (_node->findAttribute("size", tmp))
				{
					xml::ElementPtr prop = node->createChild("Property");
					prop->addAttribute("key", "Size");
					prop->addAttribute("value", tmp);
				}

				if (_node->findAttribute("resolution", tmp))
				{
					xml::ElementPtr prop = node->createChild("Property");
					prop->addAttribute("key", "Resolution");
					prop->addAttribute("value", tmp);
				}

				if (_node->findAttribute("antialias_colour", tmp))
				{
					xml::ElementPtr prop = node->createChild("Property");
					prop->addAttribute("key", "Antialias");
					prop->addAttribute("value", tmp);
				}

				if (_node->findAttribute("space_width", tmp))
				{
					xml::ElementPtr prop = node->createChild("Property");
					prop->addAttribute("key", "SpaceWidth");
					prop->addAttribute("value", tmp);
				}

				if (_node->findAttribute("tab_width", tmp))
				{
					xml::ElementPtr prop = node->createChild("Property");
					prop->addAttribute("key", "TabWidth");
					prop->addAttribute("value", tmp);
				}

				if (_node->findAttribute("cursor_width", tmp))
				{
					xml::ElementPtr prop = node->createChild("Property");
					prop->addAttribute("key", "CursorWidth");
					prop->addAttribute("value", tmp);
				}

				if (_node->findAttribute("distance", tmp))
				{
					xml::ElementPtr prop = node->createChild("Property");
					prop->addAttribute("key", "Distance");
					prop->addAttribute("value", tmp);
				}

				if (_node->findAttribute("offset_height", tmp))
				{
					xml::ElementPtr prop = node->createChild("Property");
					prop->addAttribute("key", "OffsetHeight");
					prop->addAttribute("value", tmp);
				}

				if (_node->findAttribute("default_height", tmp))
				{
					xml::ElementPtr prop = node->createChild("Property");
					prop->addAttribute("key", "DefaultHeight");
					prop->addAttribute("value", tmp);
				}

				xml::ElementPtr codes = node->createChild("Codes");

				xml::ElementEnumerator codeold = _node->getElementEnumerator();
				while (codeold.next("Code"))
				{
					xml::ElementPtr codenew = codes->createChild("Code");

					if (codeold->findAttribute("range", tmp))
						codenew->addAttribute("range", tmp);

					if (codeold->findAttribute("hide", tmp))
						codenew->addAttribute("hide", tmp);

					if (codeold->findAttribute("index", tmp))
						codenew->addAttribute("index", tmp);

					if (codeold->findAttribute("coord", tmp))
						codenew->addAttribute("coord", tmp);
				}

				ResourceManager::getInstance().loadFromXmlNode(root, _file, _version);
			}
		}
	}

	void MemberObsolete<Gui>::destroyWidgetsVector(VectorWidgetPtr& _widgets)
	{
		static_cast<Gui*>(this)->destroyWidgets(_widgets);
	}

	bool MemberObsolete<Gui>::injectMouseMove( int _absx, int _absy, int _absz)
	{
		return InputManager::getInstance().injectMouseMove(_absx, _absy, _absz);
	}
	bool MemberObsolete<Gui>::injectMousePress( int _absx, int _absy, MouseButton _id )
	{
		return InputManager::getInstance().injectMousePress(_absx, _absy, _id);
	}
	bool MemberObsolete<Gui>::injectMouseRelease( int _absx, int _absy, MouseButton _id )
	{
		return InputManager::getInstance().injectMouseRelease(_absx, _absy, _id);
	}
	bool MemberObsolete<Gui>::injectKeyPress(KeyCode _key, Char _text)
	{
		return InputManager::getInstance().injectKeyPress(_key, _text);
	}
	bool MemberObsolete<Gui>::injectKeyRelease(KeyCode _key)
	{
		return InputManager::getInstance().injectKeyRelease(_key);
	}

	void MemberObsolete<Gui>::hidePointer()
	{
		PointerManager::getInstance().setVisible(false);
	}
	void MemberObsolete<Gui>::showPointer()
	{
		PointerManager::getInstance().setVisible(true);
	}
	bool MemberObsolete<Gui>::isShowPointer()
	{
		return PointerManager::getInstance().isVisible();
	}

	bool MemberObsolete<Gui>::load(const std::string& _file)
	{
		return ResourceManager::getInstance().load(_file);
	}

	const IntSize& MemberObsolete<Gui>::getViewSize()
	{
		return RenderManager::getInstance().getViewSize();
	}
	int MemberObsolete<Gui>::getViewWidth()
	{
		return RenderManager::getInstance().getViewSize().width;
	}
	int MemberObsolete<Gui>::getViewHeight()
	{
		return RenderManager::getInstance().getViewSize().height;
	}

	void MemberObsolete<Gui>::setVisiblePointer(bool _value)
	{
		PointerManager::getInstance().setVisible(_value);
	}
	bool MemberObsolete<Gui>::isVisiblePointer()
	{
		return PointerManager::getInstance().isVisible();
	}

	void MemberObsolete<Gui>::_injectFrameEntered(float _time)
	{
		Gui::getInstance().frameEvent(_time);
	}

	void MemberObsolete<Gui>::_resizeWindow(const IntSize& _size)
	{
		LayerManager::getInstance().resizeView(_size);
	}

	const IntPoint& MemberObsolete<InputManager>::getLastLeftPressed() const
	{
		return InputManager::getInstance().getLastPressedPosition(MouseButton::Left);
	}

	const IntPoint& MemberObsolete<InputManager>::getLastRightPressed() const
	{
		return InputManager::getInstance().getLastPressedPosition(MouseButton::Right);
	}

	bool MemberObsolete<LanguageManager>::load(const std::string& _file)
	{
		return ResourceManager::getInstance().load(_file);
	}

	bool MemberObsolete<LayerManager>::load(const std::string& _file)
	{
		return ResourceManager::getInstance().load(_file);
	}

	VectorWidgetPtr MemberObsolete<LayoutManager>::load(const std::string& _file)
	{
		return static_cast<LayoutManager*>(this)->loadLayout(_file);
	}

	bool MemberObsolete<PluginManager>::load(const std::string& _file)
	{
		return ResourceManager::getInstance().load(_file);
	}

	void MemberObsolete<PointerManager>::setDeafultPointer(const std::string& _value)
	{
		static_cast<PointerManager*>(this)->setDefaultPointer(_value);
	}
	void MemberObsolete<PointerManager>::show()
	{
		static_cast<PointerManager*>(this)->setVisible(true);
	}
	void MemberObsolete<PointerManager>::hide()
	{
		static_cast<PointerManager*>(this)->setVisible(false);
	}
	bool MemberObsolete<PointerManager>::isShow()
	{
		return static_cast<PointerManager*>(this)->isVisible();
	}
	bool MemberObsolete<PointerManager>::load(const std::string& _file)
	{
		return ResourceManager::getInstance().load(_file);
	}
	void MemberObsolete<PointerManager>::loadOldPointerFormat(xml::ElementPtr _node, const std::string& _file, Version _version, const std::string& _tag)
	{
		std::string pointer;
		std::string layer;

		xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == _tag)
			{
				layer = node->findAttribute("layer");
				pointer = node->findAttribute("default");

				// сохраняем
				std::string shared_text = node->findAttribute("texture");

				// берем детей и крутимся, основной цикл
				xml::ElementEnumerator info = node->getElementEnumerator();
				while (info.next("Info"))
				{
					std::string name = info->findAttribute("name");
					if (name.empty())
						continue;

					std::string texture = info->findAttribute("texture");

					std::string type = (shared_text.empty() && texture.empty()) ? "ResourceImageSetPointer" : "ResourceManualPointer";

					xml::Document doc;
					xml::ElementPtr root = doc.createRoot("MyGUI");
					xml::ElementPtr newnode = root->createChild("Resource");
					newnode->addAttribute("type", type);
					newnode->addAttribute("name", name);

					std::string tmp;
					if (info->findAttribute("point", tmp))
					{
						xml::ElementPtr prop = newnode->createChild("Property");
						prop->addAttribute("key", "Point");
						prop->addAttribute("value", tmp);
					}

					if (info->findAttribute("size", tmp))
					{
						xml::ElementPtr prop = newnode->createChild("Property");
						prop->addAttribute("key", "Size");
						prop->addAttribute("value", tmp);
					}

					if (info->findAttribute("resource", tmp))
					{
						xml::ElementPtr prop = newnode->createChild("Property");
						prop->addAttribute("key", "Resource");
						prop->addAttribute("value", tmp);
					}

					if (info->findAttribute("offset", tmp))
					{
						xml::ElementPtr prop = newnode->createChild("Property");
						prop->addAttribute("key", "Coord");
						prop->addAttribute("value", tmp);
					}

					if (!shared_text.empty() || !texture.empty())
					{
						xml::ElementPtr prop = newnode->createChild("Property");
						prop->addAttribute("key", "Texture");
						prop->addAttribute("value",  shared_text.empty() ? texture : shared_text);
					}

					ResourceManager::getInstance().loadFromXmlNode(root, _file, _version);
				}
			}
		}

		if (!layer.empty())
			static_cast<PointerManager*>(this)->setLayerName(layer);

		if (!pointer.empty())
			static_cast<PointerManager*>(this)->setDefaultPointer(pointer);
	}

	size_t MemberObsolete<ResourceManager>::getResourceCount()
	{
		return static_cast<ResourceManager*>(this)->getCount();
	}
	IResourcePtr MemberObsolete<ResourceManager>::getResource(const std::string& _name, bool _throw)
	{
		return static_cast<ResourceManager*>(this)->getByName(_name, _throw);
	}

	ResourceSkin* MemberObsolete<SkinManager>::getSkin(const std::string& _name)
	{
		return static_cast<SkinManager*>(this)->getByName(_name);
	}
	bool MemberObsolete<SkinManager>::load(const std::string& _file)
	{
		return ResourceManager::getInstance().load(_file);
	}
	void MemberObsolete<SkinManager>::loadOldSkinFormat(xml::ElementPtr _node, const std::string& _file, Version _version, const std::string& _tag)
	{
		std::string resourceCategory = ResourceManager::getInstance().getCategoryName();

		// берем детей и крутимся, основной цикл со скинами
		xml::ElementEnumerator skin = _node->getElementEnumerator();
		while (skin.next(_tag))
		{
			std::string type = skin->findAttribute("type");
			if (type.empty())
				type = "ResourceSkin";

			IObject* object = FactoryManager::getInstance().createObject(resourceCategory, type);
			if (object != nullptr)
			{
				ResourceSkin* data = object->castType<ResourceSkin>();
				data->deserialization(skin.current(), _version);

				ResourceManager::getInstance().addResource(data);
			}
		}
	}


	void MemberObsolete<WidgetManager>::destroyWidgetsVector(VectorWidgetPtr& _widgets)
	{
		static_cast<WidgetManager*>(this)->destroyWidgets(_widgets);
	}
	Widget* MemberObsolete<WidgetManager>::findWidgetT(const std::string& _name, bool _throw)
	{
		return Gui::getInstance().findWidgetT(_name, _throw);
	}
	Widget* MemberObsolete<WidgetManager>::findWidgetT(const std::string& _name, const std::string& _prefix, bool _throw)
	{
		return Gui::getInstance().findWidgetT(_name, _prefix, _throw);
	}
	void MemberObsolete<WidgetManager>::parse(Widget* _widget, const std::string& _key, const std::string& _value)
	{
		_widget->setProperty(_key, _value);
	}


#endif // MYGUI_DONT_USE_OBSOLETE

#ifndef MYGUI_DONT_USE_OBSOLETE

	static std::string convertAlignToDirection(const std::string& _value)
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

	static std::string convertRectToCoord(const std::string& _value)
	{
		IntRect rect = IntRect::parse(_value);
		IntCoord coord(rect.left, rect.top, rect.width(), rect.height());
		return coord.print();
	}

	typedef std::set<std::string> SetString;
	static MapString mPropertyRename;
	static SetString mPropertyIgnore;
	static MapString mSkinRename;

#endif // MYGUI_DONT_USE_OBSOLETE

	bool BackwardCompatibility::isIgnoreProperty(const std::string& _key)
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		if (mPropertyIgnore.find(_key) != mPropertyIgnore.end())
			return true;
#endif // MYGUI_DONT_USE_OBSOLETE
		return false;
	}

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
			ListBox* widget = _owner->castType<ListBox>(false);
			if (widget != nullptr)
				widget->addItem(_value);
			return false;
		}
		else if (_key == "Tab_AddSheet")
		{
			MYGUI_LOG(Warning, "Tab_AddSheet is deprecated" << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			TabControl* widget = _owner->castType<TabControl>(false);
			if (widget != nullptr)
				widget->addItem(_value);
			return false;
		}
		else if (_key == "Tab_AddItem")
		{
			MYGUI_LOG(Warning, "Tab_AddItem is deprecated" << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			TabControl* widget = _owner->castType<TabControl>(false);
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
		mPropertyRename["Widget_Caption"] = "Caption";
		mPropertyRename["Button_Pressed"] = "StateSelected";
		mPropertyRename["ButtonPressed"] = "StateSelected";
		mPropertyRename["StateCheck"] = "StateSelected";
		mPropertyRename["Edit_ShowVScroll"] = "VisibleVScroll";
		mPropertyRename["Edit_ShowHScroll"] = "VisibleHScroll";
		mPropertyRename["ScrollView_VScroll"] = "VisibleVScroll";
		mPropertyRename["ScrollView_HScroll"] = "VisibleHScroll";
		mPropertyRename["Progress_Position"] = "RangePosition";
		mPropertyRename["Scroll_Position"] = "RangePosition";
		mPropertyRename["Tab_SelectSheet"] = "SelectItem";
		mPropertyRename["Image_Texture"] = "ImageTexture";
		mPropertyRename["Image_Coord"] = "ImageRegion";
		mPropertyRename["Image_Tile"] = "ImageTile";
		mPropertyRename["Image_Index"] = "ImageIndex";
		mPropertyRename["Image_Resource"] = "ImageResource";
		mPropertyRename["Image_Group"] = "ImageGroup";
		mPropertyRename["Image_Name"] = "ImageName";
		mPropertyRename["MenuItem_Id"] = "MenuItemId";
		mPropertyRename["MenuItem_Type"] = "MenuItemType";
		mPropertyRename["Combo_MaxLength"] = "MaxListLength";
		mPropertyRename["AlignText"] = "TextAlign";
		mPropertyRename["ToStick"] = "Snap";
		mPropertyRename["ListSmoothShow"] = "SmoothShow";
		mPropertyRename["HeightList"] = "MaxListLength";
		mPropertyRename["AlignVert"] = "VerticalAlignment";

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
		mPropertyIgnore.insert("LevelOffset");
		mPropertyIgnore.insert("State");

		mSkinRename["StaticImage"] = "ImageBox";
		mSkinRename["StaticText"] = "TextBox";
		mSkinRename["HScroll"] = "ScrollBarH";
		mSkinRename["VScroll"] = "ScrollBarV";
		mSkinRename["ItemBoxH"] = "ItemBox";
		mSkinRename["ItemBoxV"] = "ItemBox";
		mSkinRename["VSlider"] = "SliderV";
		mSkinRename["HSlider"] = "SliderH";
		mSkinRename["Progress"] = "ProgressBar";
		mSkinRename["ProgressFill"] = "ProgressBarFill";
		mSkinRename["List"] = "ListBox";
		mSkinRename["MultiList"] = "MultiListBox";
		mSkinRename["Edit"] = "EditBox";
		mSkinRename["EditStretch"] = "EditBoxStretch";
		mSkinRename["EditEmpty"] = "EditBoxEmpty";
		mSkinRename["RadioBox"] = "RadioButton";
		mSkinRename["Tab"] = "TabControl";
		mSkinRename["WordWrap"] = "WordWrapEmpty";
		mSkinRename["WordWrapSimple"] = "WordWrapEmpty";
		mSkinRename["ButtonSmall"] = "Button";
		mSkinRename["ButtonImageText"] = "ButtonImage";

		mSkinRename["TileClient"] = "ClientTileSkin";
		mSkinRename["DefaultClient"] = "ClientDefaultSkin";
		mSkinRename["Panel"] = "PanelSkin";
		mSkinRename["PanelSmall"] = "PanelSkin";
		mSkinRename["Separator1"] = "SepUpVSkin";
		mSkinRename["Separator2"] = "SepDownVSkin";
		mSkinRename["Separator3"] = "SepUpHSkin";
		mSkinRename["Separator4"] = "SepDownHSkin";
		mSkinRename["MarkerWhite"] = "WhiteSkin";
		mSkinRename["ButtonLeft"] = "ButtonLeftSkin";
		mSkinRename["ButtonRight"] = "ButtonRightSkin";
		mSkinRename["ButtonUp"] = "ButtonUpSkin";
		mSkinRename["ButtonDown"] = "ButtonDownSkin";
		mSkinRename["ButtonV"] = "ButtonAcceptSkin";
		mSkinRename["ButtonX"] = "ButtonCloseSkin";
		mSkinRename["ButtonMinusPlus"] = "ButtonExpandSkin";
#endif // MYGUI_DONT_USE_OBSOLETE
	}

	std::string BackwardCompatibility::getPropertyRename(const std::string& _propertyName)
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		MapString::const_iterator item = mPropertyRename.find(_propertyName);
		if (item != mPropertyRename.end())
			return (*item).second;
#endif // MYGUI_DONT_USE_OBSOLETE
		return _propertyName;
	}

	std::string BackwardCompatibility::getFactoryRename(const std::string& _categoryName, const std::string& _factoryName)
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		if (_categoryName == "Widget")
		{
			if (_factoryName == "HScroll")
			{
				MYGUI_LOG(Warning, "HScroll factory is deprecated, use " << ScrollBar::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
			else if (_factoryName == "VScroll")
			{
				MYGUI_LOG(Warning, "VScroll factory is deprecated, use " << ScrollBar::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
			else if (_factoryName == "RenderBox")
			{
				MYGUI_LOG(Warning, "RenderBox factory is deprecated, use " << Canvas::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
			else if (_factoryName == "Sheet")
			{
				MYGUI_LOG(Warning, "Sheet factory is deprecated, use " << TabItem::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
			else if (_factoryName == "StaticImage")
			{
				MYGUI_LOG(Warning, "StaticImage factory is deprecated, use " << ImageBox::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
			else if (_factoryName == "StaticText")
			{
				MYGUI_LOG(Warning, "StaticText factory is deprecated, use " << TextBox::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
			else if (_factoryName == "Progress")
			{
				MYGUI_LOG(Warning, "Progress factory is deprecated, use " << ProgressBar::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
			else if (_factoryName == "List")
			{
				MYGUI_LOG(Warning, "List factory is deprecated, use " << ListBox::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
			else if (_factoryName == "Edit")
			{
				MYGUI_LOG(Warning, "Edit factory is deprecated, use " << EditBox::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
			else if (_factoryName == "Tab")
			{
				MYGUI_LOG(Warning, "Tab factory is deprecated, use " << TabControl::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
			else if (_factoryName == "MultiList")
			{
				MYGUI_LOG(Warning, "MultiList factory is deprecated, use " << MultiListBox::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
			else if (_factoryName == "MenuCtrl")
			{
				MYGUI_LOG(Warning, "MenuCtrl factory is deprecated, use " << MenuControl::getClassTypeName() << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
		}
#endif // MYGUI_DONT_USE_OBSOLETE
		return _factoryName;
	}

	std::string BackwardCompatibility::getSkinRename(const std::string& _skinName)
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		MapString::iterator item = mSkinRename.find(_skinName);
		if (item != mSkinRename.end())
		{
			MYGUI_LOG(Warning, (*item).first << " skin is deprecated, use " << (*item).second << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			return (*item).second;
		}
#endif // MYGUI_DONT_USE_OBSOLETE
		return _skinName;
	}

	void BackwardCompatibility::shutdown()
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		mPropertyRename.clear();
#endif // MYGUI_DONT_USE_OBSOLETE
	}

	void BackwardCompatibility::registerWidgetTypes()
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		FactoryManager& factory = FactoryManager::getInstance();
		std::string widgetCategory = MyGUI::WidgetManager::getInstance().getCategoryName();
		factory.registerFactory<HScroll>(widgetCategory);
		factory.registerFactory<VScroll>(widgetCategory);
		factory.registerFactory<Canvas>(widgetCategory, "RenderBox");
		factory.registerFactory<TabItem>(widgetCategory, "Sheet");
		factory.registerFactory<ImageBox>(widgetCategory, "StaticImage");
		factory.registerFactory<TextBox>(widgetCategory, "StaticText");
		factory.registerFactory<ProgressBar>(widgetCategory, "Progress");
		factory.registerFactory<ListBox>(widgetCategory, "List");
		factory.registerFactory<EditBox>(widgetCategory, "Edit");
		factory.registerFactory<TabControl>(widgetCategory, "Tab");
		factory.registerFactory<MultiListBox>(widgetCategory, "MultiList");
		factory.registerFactory<MenuControl>(widgetCategory, "MenuCtrl");
#endif // MYGUI_DONT_USE_OBSOLETE
	}

} // namespace MyGUI
