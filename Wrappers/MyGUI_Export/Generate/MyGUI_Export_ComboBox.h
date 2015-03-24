/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef EXPORT_WIDGET_ComboBox_H_
#define EXPORT_WIDGET_ComboBox_H_

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI_Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{
	//InsertPoint

	namespace ScopeComboBoxEvent_ComboChangePosition
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::ComboBox *,
			Convert<size_t>::Type);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ComboBox * _sender,
			size_t _index)
		{
			mExportHandle(
				_sender,
				Convert<size_t>::To(_index));
		}
		
		MYGUIEXPORT void MYGUICALL ExportComboBoxEvent_DelegateComboChangePosition(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportComboBoxEvent_AdviseComboChangePosition(MyGUI::Widget* _widget, bool _advise)
		{
			if (_advise)
				static_cast<MyGUI::ComboBox*>(_widget)->eventComboChangePosition += MyGUI::newDelegate(OnEvent);
			else
				static_cast<MyGUI::ComboBox*>(_widget)->eventComboChangePosition -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeComboBoxEvent_ComboAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::ComboBox *,
			Convert<size_t>::Type);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ComboBox * _sender,
			size_t _index)
		{
			mExportHandle(
				_sender,
				Convert<size_t>::To(_index));
		}
		
		MYGUIEXPORT void MYGUICALL ExportComboBoxEvent_DelegateComboAccept(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportComboBoxEvent_AdviseComboAccept(MyGUI::Widget* _widget, bool _advise)
		{
			if (_advise)
				static_cast<MyGUI::ComboBox*>(_widget)->eventComboAccept += MyGUI::newDelegate(OnEvent);
			else
				static_cast<MyGUI::ComboBox*>(_widget)->eventComboAccept -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeComboBoxMethod_BeginToItemSelected
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_BeginToItemSelected(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::ComboBox*>(_native)->beginToItemSelected();
		}
	}
	namespace ScopeComboBoxMethod_BeginToItemLast
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_BeginToItemLast(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::ComboBox*>(_native)->beginToItemLast();
		}
	}
	namespace ScopeComboBoxMethod_BeginToItemFirst
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_BeginToItemFirst(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::ComboBox*>(_native)->beginToItemFirst();
		}
	}
	namespace ScopeComboBoxMethod_BeginToItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_BeginToItemAt__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _index)
		{
			static_cast<MyGUI::ComboBox*>(_native)->beginToItemAt(
				Convert<size_t>::From(_index));
		}
	}
	namespace ScopeComboBoxMethod_GetItemNameAt
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportComboBox_GetItemNameAt__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _index)
		{
			return Convert<const MyGUI::UString &>::To(static_cast<MyGUI::ComboBox*>(_native)->getItemNameAt(
				Convert<size_t>::From(_index)));
		}
	}
	namespace ScopeComboBoxMethod_SetItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetItemNameAt__index__name(MyGUI::Widget* _native,
			Convert<size_t>::Type _index,
			Convert<const MyGUI::UString &>::Type _name)
		{
			static_cast<MyGUI::ComboBox*>(_native)->setItemNameAt(
				Convert<size_t>::From(_index),
				Convert<const MyGUI::UString &>::From(_name));
		}
	}
	namespace ScopeComboBoxMethod_ClearIndexSelected
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_ClearIndexSelected(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::ComboBox*>(_native)->clearIndexSelected();
		}
	}
	namespace ScopeComboBoxMethod_FindItemIndexWith
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportComboBox_FindItemIndexWith__name(MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name)
		{
			return Convert<size_t>::To(static_cast<MyGUI::ComboBox*>(_native)->findItemIndexWith(
				Convert<const MyGUI::UString &>::From(_name)));
		}
	}
	namespace ScopeComboBoxMethod_RemoveAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_RemoveAllItems(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::ComboBox*>(_native)->removeAllItems();
		}
	}
	namespace ScopeComboBoxMethod_RemoveItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_RemoveItemAt__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _index)
		{
			static_cast<MyGUI::ComboBox*>(_native)->removeItemAt(
				Convert<size_t>::From(_index));
		}
	}
	namespace ScopeComboBoxMethod_AddItem
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_AddItem__name(MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name)
		{
			static_cast<MyGUI::ComboBox*>(_native)->addItem(
				Convert<const MyGUI::UString &>::From(_name));
		}
	}
	namespace ScopeComboBoxMethod_InsertItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportComboBox_InsertItemAt__index__name(MyGUI::Widget* _native,
			Convert<size_t>::Type _index,
			Convert<const MyGUI::UString &>::Type _name)
		{
			static_cast<MyGUI::ComboBox*>(_native)->insertItemAt(
				Convert<size_t>::From(_index),
				Convert<const MyGUI::UString &>::From(_name));
		}
	}
	namespace ScopeComboBoxProperty_FlowDirection
	{
		MYGUIEXPORT Convert<MyGUI::FlowDirection>::Type MYGUICALL ExportComboBox_GetFlowDirection(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::FlowDirection>::To(static_cast<MyGUI::ComboBox*>(_native)->getFlowDirection());
		}
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetFlowDirection(MyGUI::Widget* _native, Convert<MyGUI::FlowDirection>::Type _value)
		{
			static_cast<MyGUI::ComboBox*>(_native)->setFlowDirection(Convert<MyGUI::FlowDirection>::From(_value));
		}
	}
	namespace ScopeComboBoxProperty_MaxListLength
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportComboBox_GetMaxListLength(MyGUI::Widget* _native)
		{
			return Convert<int>::To(static_cast<MyGUI::ComboBox*>(_native)->getMaxListLength());
		}
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetMaxListLength(MyGUI::Widget* _native, Convert<int>::Type _value)
		{
			static_cast<MyGUI::ComboBox*>(_native)->setMaxListLength(Convert<int>::From(_value));
		}
	}
	namespace ScopeComboBoxProperty_SmoothShow
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportComboBox_GetSmoothShow(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::ComboBox*>(_native)->getSmoothShow());
		}
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetSmoothShow(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::ComboBox*>(_native)->setSmoothShow(Convert<bool>::From(_value));
		}
	}
	namespace ScopeComboBoxProperty_ComboModeDrop
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportComboBox_GetComboModeDrop(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::ComboBox*>(_native)->getComboModeDrop());
		}
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetComboModeDrop(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::ComboBox*>(_native)->setComboModeDrop(Convert<bool>::From(_value));
		}
	}
	namespace ScopeComboBoxProperty_IndexSelected
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportComboBox_GetIndexSelected(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::ComboBox*>(_native)->getIndexSelected());
		}
		MYGUIEXPORT void MYGUICALL ExportComboBox_SetIndexSelected(MyGUI::Widget* _native, Convert<size_t>::Type _value)
		{
			static_cast<MyGUI::ComboBox*>(_native)->setIndexSelected(Convert<size_t>::From(_value));
		}
	}
	namespace ScopeComboBoxProperty_ItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportComboBox_GetItemCount(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::ComboBox*>(_native)->getItemCount());
		}
	}
}

#endif
