/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_MultiListBox_H__
#define __EXPORT_WIDGET_MultiListBox_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI_Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{
	//InsertPoint

	namespace ScopeMultiListBoxEvent_OperatorLess
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::MultiListBox *,
			Convert<size_t>::Type,
			Convert<const MyGUI::UString &>::Type,
			Convert<const MyGUI::UString &>::Type,
			Convert<bool &>::Type);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::MultiListBox * _sender,
			size_t _column,
			const MyGUI::UString & _firstItem,
			const MyGUI::UString & _secondItem,
			bool & _less)
		{
			mExportHandle(
				_sender,
				Convert<size_t>::To(_column),
				Convert<const MyGUI::UString &>::To(_firstItem),
				Convert<const MyGUI::UString &>::To(_secondItem),
				Convert<bool &>::To(_less));
		}
		
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_DelegateOperatorLess(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_AdviseOperatorLess(MyGUI::Widget* _widget, bool _advise)
		{
			static_cast<MyGUI::MultiListBox*>(_widget)->requestOperatorLess = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}
	namespace ScopeMultiListBoxEvent_ListChangePosition
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::MultiListBox *,
			Convert<size_t>::Type);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::MultiListBox * _sender,
			size_t _index)
		{
			mExportHandle(
				_sender,
				Convert<size_t>::To(_index));
		}
		
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_DelegateListChangePosition(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_AdviseListChangePosition(MyGUI::Widget* _widget, bool _advise)
		{
			if (_advise)
				static_cast<MyGUI::MultiListBox*>(_widget)->eventListChangePosition += MyGUI::newDelegate(OnEvent);
			else
				static_cast<MyGUI::MultiListBox*>(_widget)->eventListChangePosition -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeMultiListBoxEvent_ListSelectAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::MultiListBox *,
			Convert<size_t>::Type);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::MultiListBox * _sender,
			size_t _index)
		{
			mExportHandle(
				_sender,
				Convert<size_t>::To(_index));
		}
		
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_DelegateListSelectAccept(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_AdviseListSelectAccept(MyGUI::Widget* _widget, bool _advise)
		{
			if (_advise)
				static_cast<MyGUI::MultiListBox*>(_widget)->eventListSelectAccept += MyGUI::newDelegate(OnEvent);
			else
				static_cast<MyGUI::MultiListBox*>(_widget)->eventListSelectAccept -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeMultiListBoxMethod_FindSubItemWith
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMultiListBox_FindSubItemWith__column__name(MyGUI::Widget* _native,
			Convert<size_t>::Type _column,
			Convert<const MyGUI::UString &>::Type _name)
		{
			return Convert<size_t>::To(static_cast<MyGUI::MultiListBox*>(_native)->findSubItemWith(
				Convert<size_t>::From(_column),
				Convert<const MyGUI::UString &>::From(_name)));
		}
	}
	namespace ScopeMultiListBoxMethod_GetSubItemNameAt
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportMultiListBox_GetSubItemNameAt__column__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _column,
			Convert<size_t>::Type _index)
		{
			return Convert<const MyGUI::UString &>::To(static_cast<MyGUI::MultiListBox*>(_native)->getSubItemNameAt(
				Convert<size_t>::From(_column),
				Convert<size_t>::From(_index)));
		}
	}
	namespace ScopeMultiListBoxMethod_SetSubItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetSubItemNameAt__column__index__name(MyGUI::Widget* _native,
			Convert<size_t>::Type _column,
			Convert<size_t>::Type _index,
			Convert<const MyGUI::UString &>::Type _name)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->setSubItemNameAt(
				Convert<size_t>::From(_column),
				Convert<size_t>::From(_index),
				Convert<const MyGUI::UString &>::From(_name));
		}
	}
	namespace ScopeMultiListBoxMethod_ClearIndexSelected
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_ClearIndexSelected(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->clearIndexSelected();
		}
	}
	namespace ScopeMultiListBoxMethod_GetItemNameAt
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportMultiListBox_GetItemNameAt__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _index)
		{
			return Convert<const MyGUI::UString &>::To(static_cast<MyGUI::MultiListBox*>(_native)->getItemNameAt(
				Convert<size_t>::From(_index)));
		}
	}
	namespace ScopeMultiListBoxMethod_SetItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetItemNameAt__index__name(MyGUI::Widget* _native,
			Convert<size_t>::Type _index,
			Convert<const MyGUI::UString &>::Type _name)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->setItemNameAt(
				Convert<size_t>::From(_index),
				Convert<const MyGUI::UString &>::From(_name));
		}
	}
	namespace ScopeMultiListBoxMethod_SwapItemsAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SwapItemsAt__index1__index2(MyGUI::Widget* _native,
			Convert<size_t>::Type _index1,
			Convert<size_t>::Type _index2)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->swapItemsAt(
				Convert<size_t>::From(_index1),
				Convert<size_t>::From(_index2));
		}
	}
	namespace ScopeMultiListBoxMethod_RemoveAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_RemoveAllItems(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->removeAllItems();
		}
	}
	namespace ScopeMultiListBoxMethod_RemoveItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_RemoveItemAt__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _index)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->removeItemAt(
				Convert<size_t>::From(_index));
		}
	}
	namespace ScopeMultiListBoxMethod_AddItem
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_AddItem__name(MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->addItem(
				Convert<const MyGUI::UString &>::From(_name));
		}
	}
	namespace ScopeMultiListBoxMethod_InsertItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_InsertItemAt__index__name(MyGUI::Widget* _native,
			Convert<size_t>::Type _index,
			Convert<const MyGUI::UString &>::Type _name)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->insertItemAt(
				Convert<size_t>::From(_index),
				Convert<const MyGUI::UString &>::From(_name));
		}
	}
	namespace ScopeMultiListBoxMethod_SetColumnResizingPolicyAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetColumnResizingPolicyAt__index__value(MyGUI::Widget* _native,
			Convert<size_t>::Type _index,
			Convert<MyGUI::ResizingPolicy>::Type _value)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->setColumnResizingPolicyAt(
				Convert<size_t>::From(_index),
				Convert<MyGUI::ResizingPolicy>::From(_value));
		}
	}
	namespace ScopeMultiListBoxMethod_SetColumnResizingPolicy
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetColumnResizingPolicy__item__value(MyGUI::Widget* _native,
			Convert<MyGUI::MultiListItem *>::Type _item,
			Convert<MyGUI::ResizingPolicy>::Type _value)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->setColumnResizingPolicy(
				Convert<MyGUI::MultiListItem *>::From(_item),
				Convert<MyGUI::ResizingPolicy>::From(_value));
		}
	}
	namespace ScopeMultiListBoxMethod_GetColumnIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMultiListBox_GetColumnIndex__item(MyGUI::Widget* _native,
			Convert<MyGUI::MultiListItem *>::Type _item)
		{
			return Convert<size_t>::To(static_cast<MyGUI::MultiListBox*>(_native)->getColumnIndex(
				Convert<MyGUI::MultiListItem *>::From(_item)));
		}
	}
	namespace ScopeMultiListBoxMethod_SortByColumn
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SortByColumn__column__backward(MyGUI::Widget* _native,
			Convert<size_t>::Type _column,
			Convert<bool>::Type _backward)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->sortByColumn(
				Convert<size_t>::From(_column),
				Convert<bool>::From(_backward));
		}
	}
	namespace ScopeMultiListBoxMethod_GetColumnWidthAt
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportMultiListBox_GetColumnWidthAt__column(MyGUI::Widget* _native,
			Convert<size_t>::Type _column)
		{
			return Convert<int>::To(static_cast<MyGUI::MultiListBox*>(_native)->getColumnWidthAt(
				Convert<size_t>::From(_column)));
		}
	}
	namespace ScopeMultiListBoxMethod_GetColumnName
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportMultiListBox_GetColumnName__item(MyGUI::Widget* _native,
			Convert<MyGUI::MultiListItem *>::Type _item)
		{
			return Convert<const MyGUI::UString &>::To(static_cast<MyGUI::MultiListBox*>(_native)->getColumnName(
				Convert<MyGUI::MultiListItem *>::From(_item)));
		}
	}
	namespace ScopeMultiListBoxMethod_GetColumnNameAt
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportMultiListBox_GetColumnNameAt__column(MyGUI::Widget* _native,
			Convert<size_t>::Type _column)
		{
			return Convert<const MyGUI::UString &>::To(static_cast<MyGUI::MultiListBox*>(_native)->getColumnNameAt(
				Convert<size_t>::From(_column)));
		}
	}
	namespace ScopeMultiListBoxMethod_SetColumnWidth
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetColumnWidth__item__width(MyGUI::Widget* _native,
			Convert<MyGUI::MultiListItem *>::Type _item,
			Convert<int>::Type _width)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->setColumnWidth(
				Convert<MyGUI::MultiListItem *>::From(_item),
				Convert<int>::From(_width));
		}
	}
	namespace ScopeMultiListBoxMethod_SetColumnWidthAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetColumnWidthAt__column__width(MyGUI::Widget* _native,
			Convert<size_t>::Type _column,
			Convert<int>::Type _width)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->setColumnWidthAt(
				Convert<size_t>::From(_column),
				Convert<int>::From(_width));
		}
	}
	namespace ScopeMultiListBoxMethod_SetColumnName
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetColumnName__item__name(MyGUI::Widget* _native,
			Convert<MyGUI::MultiListItem *>::Type _item,
			Convert<const MyGUI::UString &>::Type _name)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->setColumnName(
				Convert<MyGUI::MultiListItem *>::From(_item),
				Convert<const MyGUI::UString &>::From(_name));
		}
	}
	namespace ScopeMultiListBoxMethod_SetColumnNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetColumnNameAt__column__name(MyGUI::Widget* _native,
			Convert<size_t>::Type _column,
			Convert<const MyGUI::UString &>::Type _name)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->setColumnNameAt(
				Convert<size_t>::From(_column),
				Convert<const MyGUI::UString &>::From(_name));
		}
	}
	namespace ScopeMultiListBoxMethod_SwapColumnsAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SwapColumnsAt__index1__index2(MyGUI::Widget* _native,
			Convert<size_t>::Type _index1,
			Convert<size_t>::Type _index2)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->swapColumnsAt(
				Convert<size_t>::From(_index1),
				Convert<size_t>::From(_index2));
		}
	}
	namespace ScopeMultiListBoxMethod_RemoveAllColumns
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_RemoveAllColumns(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->removeAllColumns();
		}
	}
	namespace ScopeMultiListBoxMethod_RemoveColumnAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_RemoveColumnAt__column(MyGUI::Widget* _native,
			Convert<size_t>::Type _column)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->removeColumnAt(
				Convert<size_t>::From(_column));
		}
	}
	namespace ScopeMultiListBoxMethod_AddColumn
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_AddColumn__name__width(MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name,
			Convert<int>::Type _width)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->addColumn(
				Convert<const MyGUI::UString &>::From(_name),
				Convert<int>::From(_width));
		}
	}
	namespace ScopeMultiListBoxMethod_InsertColumnAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_InsertColumnAt__column__name__width(MyGUI::Widget* _native,
			Convert<size_t>::Type _column,
			Convert<const MyGUI::UString &>::Type _name,
			Convert<int>::Type _width)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->insertColumnAt(
				Convert<size_t>::From(_column),
				Convert<const MyGUI::UString &>::From(_name),
				Convert<int>::From(_width));
		}
	}
	namespace ScopeMultiListBoxProperty_IndexSelected
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMultiListBox_GetIndexSelected(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::MultiListBox*>(_native)->getIndexSelected());
		}
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetIndexSelected(MyGUI::Widget* _native, Convert<size_t>::Type _value)
		{
			static_cast<MyGUI::MultiListBox*>(_native)->setIndexSelected(Convert<size_t>::From(_value));
		}
	}
	namespace ScopeMultiListBoxProperty_ItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMultiListBox_GetItemCount(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::MultiListBox*>(_native)->getItemCount());
		}
	}
	namespace ScopeMultiListBoxProperty_ColumnCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMultiListBox_GetColumnCount(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::MultiListBox*>(_native)->getColumnCount());
		}
	}
}

#endif
