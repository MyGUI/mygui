﻿/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_ListBox_H__
#define __EXPORT_WIDGET_ListBox_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeListBoxEvent_ListChangeScroll
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ListBox *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ListBox * _sender ,
			size_t _position )
		{
			mExportHandle(
				Convert<MyGUI::ListBox *>::To( _sender ) ,
				Convert<size_t>::To( _position ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportListBoxEvent_DelegateListChangeScroll( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportListBoxEvent_AdviseListChangeScroll( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::ListBox* >(_widget)->eventListChangeScroll += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::ListBox* >(_widget)->eventListChangeScroll -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeListBoxEvent_ListMouseItemFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ListBox *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ListBox * _sender ,
			size_t _index )
		{
			mExportHandle(
				Convert<MyGUI::ListBox *>::To( _sender ) ,
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportListBoxEvent_DelegateListMouseItemFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportListBoxEvent_AdviseListMouseItemFocus( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::ListBox* >(_widget)->eventListMouseItemFocus += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::ListBox* >(_widget)->eventListMouseItemFocus -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeListBoxEvent_ListMouseItemActivate
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ListBox *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ListBox * _sender ,
			size_t _index )
		{
			mExportHandle(
				Convert<MyGUI::ListBox *>::To( _sender ) ,
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportListBoxEvent_DelegateListMouseItemActivate( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportListBoxEvent_AdviseListMouseItemActivate( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::ListBox* >(_widget)->eventListMouseItemActivate += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::ListBox* >(_widget)->eventListMouseItemActivate -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeListBoxEvent_ListChangePosition
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ListBox *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ListBox * _sender ,
			size_t _index )
		{
			mExportHandle(
				Convert<MyGUI::ListBox *>::To( _sender ) ,
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportListBoxEvent_DelegateListChangePosition( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportListBoxEvent_AdviseListChangePosition( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::ListBox* >(_widget)->eventListChangePosition += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::ListBox* >(_widget)->eventListChangePosition -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeListBoxEvent_ListSelectAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ListBox *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ListBox * _sender ,
			size_t _index )
		{
			mExportHandle(
				Convert<MyGUI::ListBox *>::To( _sender ) ,
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportListBoxEvent_DelegateListSelectAccept( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportListBoxEvent_AdviseListSelectAccept( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::ListBox* >(_widget)->eventListSelectAccept += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::ListBox* >(_widget)->eventListSelectAccept -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeListBoxMethod_GetOptimalHeight
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportListBox_GetOptimalHeight( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::ListBox * >(_native)->getOptimalHeight( ) );
		}
	}



   	namespace ScopeListBoxMethod_GetItemNameAt
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportListBox_GetItemNameAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<const MyGUI::UString &>::To( static_cast< MyGUI::ListBox * >(_native)->getItemNameAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeListBoxMethod_SetItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_SetItemNameAt_index_name( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			static_cast< MyGUI::ListBox * >(_native)->setItemNameAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) );
		}
	}



   


   


   


   


   


   


   


   


   


   	namespace ScopeListBoxMethod_SetScrollPosition
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_SetScrollPosition_position( MyGUI::Widget* _native,
			Convert<size_t>::Type _position )
		{
			static_cast< MyGUI::ListBox * >(_native)->setScrollPosition(
				Convert<size_t>::From( _position ) );
		}
	}



   	namespace ScopeListBoxMethod_SetScrollVisible
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_SetScrollVisible_visible( MyGUI::Widget* _native,
			Convert<bool>::Type _visible )
		{
			static_cast< MyGUI::ListBox * >(_native)->setScrollVisible(
				Convert<bool>::From( _visible ) );
		}
	}



   	namespace ScopeListBoxMethod_IsItemSelectedVisible
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportListBox_IsItemSelectedVisible_fill( MyGUI::Widget* _native,
			Convert<bool>::Type _fill )
		{
			return Convert<bool>::To( static_cast< MyGUI::ListBox * >(_native)->isItemSelectedVisible(
				Convert<bool>::From( _fill ) ));
		}
	}



   	namespace ScopeListBoxMethod_IsItemVisibleAt
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportListBox_IsItemVisibleAt_index_fill( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<bool>::Type _fill )
		{
			return Convert<bool>::To( static_cast< MyGUI::ListBox * >(_native)->isItemVisibleAt(
				Convert<size_t>::From( _index ) ,
				Convert<bool>::From( _fill ) ));
		}
	}



   	namespace ScopeListBoxMethod_BeginToItemSelected
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_BeginToItemSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ListBox * >(_native)->beginToItemSelected( );
		}
	}



   	namespace ScopeListBoxMethod_BeginToItemLast
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_BeginToItemLast( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ListBox * >(_native)->beginToItemLast( );
		}
	}



   	namespace ScopeListBoxMethod_BeginToItemFirst
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_BeginToItemFirst( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ListBox * >(_native)->beginToItemFirst( );
		}
	}



   	namespace ScopeListBoxMethod_BeginToItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_BeginToItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ListBox * >(_native)->beginToItemAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeListBoxMethod_GetItemDataAt
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportListBox_GetItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			Convert<MyGUI::Any>::Type* data = 
				static_cast< MyGUI::ListBox * >(_native)->getItemDataAt< Convert<MyGUI::Any>::Type >(
					Convert<size_t>::From( _index ) , false );
			return data == nullptr ? nullptr : *data;
		}
	}



   	namespace ScopeListBoxMethod_ClearItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_ClearItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ListBox * >(_native)->clearItemDataAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeListBoxMethod_SetItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_SetItemDataAt_index_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::ListBox * >(_native)->setItemDataAt(
				Convert<size_t>::From( _index ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
	}



   	namespace ScopeListBoxMethod_ClearIndexSelected
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_ClearIndexSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ListBox * >(_native)->clearIndexSelected( );
		}
	}



   	namespace ScopeListBoxProperty_IndexSelected
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportListBox_GetIndexSelected( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ListBox * >(_native)->getIndexSelected( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportListBox_SetIndexSelected( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ListBox * >(_native)->setIndexSelected( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeListBoxMethod_FindItemIndexWith
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportListBox_FindItemIndexWith_name( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ListBox * >(_native)->findItemIndexWith(
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeListBoxMethod_SwapItemsAt
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_SwapItemsAt_index1_index2( MyGUI::Widget* _native,
			Convert<size_t>::Type _index1 ,
			Convert<size_t>::Type _index2 )
		{
			static_cast< MyGUI::ListBox * >(_native)->swapItemsAt(
				Convert<size_t>::From( _index1 ) ,
				Convert<size_t>::From( _index2 ) );
		}
	}



   	namespace ScopeListBoxMethod_RemoveAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_RemoveAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ListBox * >(_native)->removeAllItems( );
		}
	}



   	namespace ScopeListBoxMethod_RemoveItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_RemoveItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ListBox * >(_native)->removeItemAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeListBoxMethod_AddItem
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_AddItem_name_data( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::ListBox * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
		MYGUIEXPORT void MYGUICALL ExportListBox_AddItem_name( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name )
		{
			static_cast< MyGUI::ListBox * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) );
		}
	}



   	namespace ScopeListBoxMethod_InsertItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportListBox_InsertItemAt_index_name_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::ListBox * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
		MYGUIEXPORT void MYGUICALL ExportListBox_InsertItemAt_index_name( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			static_cast< MyGUI::ListBox * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) );
		}
	}



   	namespace ScopeListBoxProperty_ItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportListBox_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ListBox * >(_native)->getItemCount( ) );
		}
	}



}

#endif // __EXPORT_WIDGET_ListBox_H__
