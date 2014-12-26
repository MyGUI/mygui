/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef EXPORT_WIDGET_TabControl_H_
#define EXPORT_WIDGET_TabControl_H_

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI_Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{
	//InsertPoint

	namespace ScopeTabControlEvent_TabChangeSelect
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::TabControl *,
			Convert<size_t>::Type);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::TabControl * _sender,
			size_t _index)
		{
			mExportHandle(
				_sender,
				Convert<size_t>::To(_index));
		}
		
		MYGUIEXPORT void MYGUICALL ExportTabControlEvent_DelegateTabChangeSelect(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportTabControlEvent_AdviseTabChangeSelect(MyGUI::Widget* _widget, bool _advise)
		{
			if (_advise)
				static_cast<MyGUI::TabControl*>(_widget)->eventTabChangeSelect += MyGUI::newDelegate(OnEvent);
			else
				static_cast<MyGUI::TabControl*>(_widget)->eventTabChangeSelect -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeTabControlMethod_GetButtonWidth
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportTabControl_GetButtonWidth__item(MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item)
		{
			return Convert<int>::To(static_cast<MyGUI::TabControl*>(_native)->getButtonWidth(
				Convert<MyGUI::TabItem *>::From(_item)));
		}
	}
	namespace ScopeTabControlMethod_GetButtonWidthAt
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportTabControl_GetButtonWidthAt__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _index)
		{
			return Convert<int>::To(static_cast<MyGUI::TabControl*>(_native)->getButtonWidthAt(
				Convert<size_t>::From(_index)));
		}
	}
	namespace ScopeTabControlMethod_SetButtonWidth
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_SetButtonWidth__item__width(MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item,
			Convert<int>::Type _width)
		{
			static_cast<MyGUI::TabControl*>(_native)->setButtonWidth(
				Convert<MyGUI::TabItem *>::From(_item),
				Convert<int>::From(_width));
		}
	}
	namespace ScopeTabControlMethod_SetButtonWidthAt
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_SetButtonWidthAt__index__width(MyGUI::Widget* _native,
			Convert<size_t>::Type _index,
			Convert<int>::Type _width)
		{
			static_cast<MyGUI::TabControl*>(_native)->setButtonWidthAt(
				Convert<size_t>::From(_index),
				Convert<int>::From(_width));
		}
	}
	namespace ScopeTabControlMethod_BeginToItemSelected
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_BeginToItemSelected(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::TabControl*>(_native)->beginToItemSelected();
		}
	}
	namespace ScopeTabControlMethod_BeginToItemLast
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_BeginToItemLast(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::TabControl*>(_native)->beginToItemLast();
		}
	}
	namespace ScopeTabControlMethod_BeginToItemFirst
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_BeginToItemFirst(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::TabControl*>(_native)->beginToItemFirst();
		}
	}
	namespace ScopeTabControlMethod_BeginToItem
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_BeginToItem__item(MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item)
		{
			static_cast<MyGUI::TabControl*>(_native)->beginToItem(
				Convert<MyGUI::TabItem *>::From(_item));
		}
	}
	namespace ScopeTabControlMethod_BeginToItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_BeginToItemAt__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _index)
		{
			static_cast<MyGUI::TabControl*>(_native)->beginToItemAt(
				Convert<size_t>::From(_index));
		}
	}
	namespace ScopeTabControlMethod_GetItemName
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportTabControl_GetItemName__item(MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item)
		{
			return Convert<const MyGUI::UString &>::To(static_cast<MyGUI::TabControl*>(_native)->getItemName(
				Convert<MyGUI::TabItem *>::From(_item)));
		}
	}
	namespace ScopeTabControlMethod_GetItemNameAt
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportTabControl_GetItemNameAt__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _index)
		{
			return Convert<const MyGUI::UString &>::To(static_cast<MyGUI::TabControl*>(_native)->getItemNameAt(
				Convert<size_t>::From(_index)));
		}
	}
	namespace ScopeTabControlMethod_SetItemName
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_SetItemName__item__name(MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item,
			Convert<const MyGUI::UString &>::Type _name)
		{
			static_cast<MyGUI::TabControl*>(_native)->setItemName(
				Convert<MyGUI::TabItem *>::From(_item),
				Convert<const MyGUI::UString &>::From(_name));
		}
	}
	namespace ScopeTabControlMethod_SetItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_SetItemNameAt__index__name(MyGUI::Widget* _native,
			Convert<size_t>::Type _index,
			Convert<const MyGUI::UString &>::Type _name)
		{
			static_cast<MyGUI::TabControl*>(_native)->setItemNameAt(
				Convert<size_t>::From(_index),
				Convert<const MyGUI::UString &>::From(_name));
		}
	}
	namespace ScopeTabControlMethod_SwapItems
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_SwapItems__index1__index2(MyGUI::Widget* _native,
			Convert<size_t>::Type _index1,
			Convert<size_t>::Type _index2)
		{
			static_cast<MyGUI::TabControl*>(_native)->swapItems(
				Convert<size_t>::From(_index1),
				Convert<size_t>::From(_index2));
		}
	}
	namespace ScopeTabControlMethod_FindItemWith
	{
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabControl_FindItemWith__name(MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name)
		{
			return Convert<MyGUI::TabItem *>::To(static_cast<MyGUI::TabControl*>(_native)->findItemWith(
				Convert<const MyGUI::UString &>::From(_name)));
		}
	}
	namespace ScopeTabControlMethod_FindItemIndexWith
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportTabControl_FindItemIndexWith__name(MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name)
		{
			return Convert<size_t>::To(static_cast<MyGUI::TabControl*>(_native)->findItemIndexWith(
				Convert<const MyGUI::UString &>::From(_name)));
		}
	}
	namespace ScopeTabControlMethod_FindItemIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportTabControl_FindItemIndex__item(MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item)
		{
			return Convert<size_t>::To(static_cast<MyGUI::TabControl*>(_native)->findItemIndex(
				Convert<MyGUI::TabItem *>::From(_item)));
		}
	}
	namespace ScopeTabControlMethod_GetItemIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportTabControl_GetItemIndex__item(MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item)
		{
			return Convert<size_t>::To(static_cast<MyGUI::TabControl*>(_native)->getItemIndex(
				Convert<MyGUI::TabItem *>::From(_item)));
		}
	}
	namespace ScopeTabControlMethod_GetItemAt
	{
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabControl_GetItemAt__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _index)
		{
			return Convert<MyGUI::TabItem *>::To(static_cast<MyGUI::TabControl*>(_native)->getItemAt(
				Convert<size_t>::From(_index)));
		}
	}
	namespace ScopeTabControlMethod_RemoveAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_RemoveAllItems(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::TabControl*>(_native)->removeAllItems();
		}
	}
	namespace ScopeTabControlMethod_RemoveItem
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_RemoveItem__item(MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item)
		{
			static_cast<MyGUI::TabControl*>(_native)->removeItem(
				Convert<MyGUI::TabItem *>::From(_item));
		}
	}
	namespace ScopeTabControlMethod_RemoveItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportTabControl_RemoveItemAt__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _index)
		{
			static_cast<MyGUI::TabControl*>(_native)->removeItemAt(
				Convert<size_t>::From(_index));
		}
	}
	namespace ScopeTabControlMethod_AddItem
	{
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabControl_AddItem__name(MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name)
		{
			return Convert<MyGUI::TabItem *>::To(static_cast<MyGUI::TabControl*>(_native)->addItem(
				Convert<const MyGUI::UString &>::From(_name)));
		}
	}
	namespace ScopeTabControlMethod_InsertItemAt
	{
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabControl_InsertItemAt__index__name(MyGUI::Widget* _native,
			Convert<size_t>::Type _index,
			Convert<const MyGUI::UString &>::Type _name)
		{
			return Convert<MyGUI::TabItem *>::To(static_cast<MyGUI::TabControl*>(_native)->insertItemAt(
				Convert<size_t>::From(_index),
				Convert<const MyGUI::UString &>::From(_name)));
		}
	}
	namespace ScopeTabControlProperty_SmoothShow
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportTabControl_GetSmoothShow(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::TabControl*>(_native)->getSmoothShow());
		}
		MYGUIEXPORT void MYGUICALL ExportTabControl_SetSmoothShow(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::TabControl*>(_native)->setSmoothShow(Convert<bool>::From(_value));
		}
	}
	namespace ScopeTabControlProperty_ButtonAutoWidth
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportTabControl_GetButtonAutoWidth(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::TabControl*>(_native)->getButtonAutoWidth());
		}
		MYGUIEXPORT void MYGUICALL ExportTabControl_SetButtonAutoWidth(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::TabControl*>(_native)->setButtonAutoWidth(Convert<bool>::From(_value));
		}
	}
	namespace ScopeTabControlProperty_ButtonDefaultWidth
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportTabControl_GetButtonDefaultWidth(MyGUI::Widget* _native)
		{
			return Convert<int>::To(static_cast<MyGUI::TabControl*>(_native)->getButtonDefaultWidth());
		}
		MYGUIEXPORT void MYGUICALL ExportTabControl_SetButtonDefaultWidth(MyGUI::Widget* _native, Convert<int>::Type _value)
		{
			static_cast<MyGUI::TabControl*>(_native)->setButtonDefaultWidth(Convert<int>::From(_value));
		}
	}
	namespace ScopeTabControlProperty_ItemSelected
	{
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabControl_GetItemSelected(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::TabItem *>::To(static_cast<MyGUI::TabControl*>(_native)->getItemSelected());
		}
		MYGUIEXPORT void MYGUICALL ExportTabControl_SetItemSelected(MyGUI::Widget* _native, Convert<MyGUI::TabItem *>::Type _value)
		{
			static_cast<MyGUI::TabControl*>(_native)->setItemSelected(Convert<MyGUI::TabItem *>::From(_value));
		}
	}
	namespace ScopeTabControlProperty_IndexSelected
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportTabControl_GetIndexSelected(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::TabControl*>(_native)->getIndexSelected());
		}
		MYGUIEXPORT void MYGUICALL ExportTabControl_SetIndexSelected(MyGUI::Widget* _native, Convert<size_t>::Type _value)
		{
			static_cast<MyGUI::TabControl*>(_native)->setIndexSelected(Convert<size_t>::From(_value));
		}
	}
	namespace ScopeTabControlProperty_ItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportTabControl_GetItemCount(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::TabControl*>(_native)->getItemCount());
		}
	}
}

#endif
