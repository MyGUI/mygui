/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_TabBar_H__
#define __EXPORT_WIDGET_TabBar_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeTabBarEvent_TabChangeSelect
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Tab *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Tab * _sender ,
			size_t _index )
		{
			mExportHandle(
				Convert<MyGUI::Tab *>::To( _sender ) ,
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportTabBarEvent_DelegateTabChangeSelect( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportTabBarEvent_AdviseTabChangeSelect( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Tab* >(_widget)->eventTabChangeSelect = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   


   	namespace ScopeTabBarProperty_SmoothShow
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportTabBar_GetSmoothShow( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Tab * >(_native)->getSmoothShow( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportTabBar_SetSmoothShow( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Tab * >(_native)->setSmoothShow( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeTabBarProperty_ButtonAutoWidth
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportTabBar_GetButtonAutoWidth( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Tab * >(_native)->getButtonAutoWidth( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportTabBar_SetButtonAutoWidth( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Tab * >(_native)->setButtonAutoWidth( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeTabBarProperty_ButtonDefaultWidth
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportTabBar_GetButtonDefaultWidth( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Tab * >(_native)->getButtonDefaultWidth( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportTabBar_SetButtonDefaultWidth( MyGUI::Widget* _native , Convert<int>::Type _value )
		{
			static_cast< MyGUI::Tab * >(_native)->setButtonDefaultWidth( Convert<int>::From( _value ) );
		}
	}



   	namespace ScopeTabBarMethod_GetButtonWidth
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportTabBar_GetButtonWidth_item( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item )
		{
			return Convert<int>::To( static_cast< MyGUI::Tab * >(_native)->getButtonWidth(
				Convert<MyGUI::TabItem *>::From( _item ) ));
		}
	}



   	namespace ScopeTabBarMethod_GetButtonWidthAt
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportTabBar_GetButtonWidthAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<int>::To( static_cast< MyGUI::Tab * >(_native)->getButtonWidthAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeTabBarMethod_SetButtonWidth
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_SetButtonWidth_item_width( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item ,
			Convert<int>::Type _width )
		{
			static_cast< MyGUI::Tab * >(_native)->setButtonWidth(
				Convert<MyGUI::TabItem *>::From( _item ) ,
				Convert<int>::From( _width ) );
		}
	}



   	namespace ScopeTabBarMethod_SetButtonWidthAt
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_SetButtonWidthAt_index_width( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<int>::Type _width )
		{
			static_cast< MyGUI::Tab * >(_native)->setButtonWidthAt(
				Convert<size_t>::From( _index ) ,
				Convert<int>::From( _width ) );
		}
	}



   	namespace ScopeTabBarMethod_BeginToItemSelected
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_BeginToItemSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Tab * >(_native)->beginToItemSelected( );
		}
	}



   	namespace ScopeTabBarMethod_BeginToItemLast
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_BeginToItemLast( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Tab * >(_native)->beginToItemLast( );
		}
	}



   	namespace ScopeTabBarMethod_BeginToItemFirst
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_BeginToItemFirst( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Tab * >(_native)->beginToItemFirst( );
		}
	}



   	namespace ScopeTabBarMethod_BeginToItem
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_BeginToItem_item( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item )
		{
			static_cast< MyGUI::Tab * >(_native)->beginToItem(
				Convert<MyGUI::TabItem *>::From( _item ) );
		}
	}



   	namespace ScopeTabBarMethod_BeginToItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_BeginToItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::Tab * >(_native)->beginToItemAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeTabBarMethod_GetItemName
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportTabBar_GetItemName_item( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item )
		{
			return Convert<const MyGUI::UString &>::To( static_cast< MyGUI::Tab * >(_native)->getItemName(
				Convert<MyGUI::TabItem *>::From( _item ) ));
		}
	}



   	namespace ScopeTabBarMethod_GetItemNameAt
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportTabBar_GetItemNameAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<const MyGUI::UString &>::To( static_cast< MyGUI::Tab * >(_native)->getItemNameAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeTabBarMethod_SetItemName
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_SetItemName_item_name( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			static_cast< MyGUI::Tab * >(_native)->setItemName(
				Convert<MyGUI::TabItem *>::From( _item ) ,
				Convert<const MyGUI::UString &>::From( _name ) );
		}
	}



   	namespace ScopeTabBarMethod_SetItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_SetItemNameAt_index_name( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			static_cast< MyGUI::Tab * >(_native)->setItemNameAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) );
		}
	}



   	namespace ScopeTabBarMethod_GetItemData
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportTabBar_GetItemData_item( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item )
		{
			Convert<MyGUI::Any>::Type* data = 
				static_cast< MyGUI::Tab * >(_native)->getItemData< Convert<MyGUI::Any>::Type >(
					Convert<MyGUI::TabItem *>::From( _item ) , false );
			return data == nullptr ? nullptr : *data;
		}
	}



   	namespace ScopeTabBarMethod_GetItemDataAt
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportTabBar_GetItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			Convert<MyGUI::Any>::Type* data = 
				static_cast< MyGUI::Tab * >(_native)->getItemDataAt< Convert<MyGUI::Any>::Type >(
					Convert<size_t>::From( _index ) , false );
			return data == nullptr ? nullptr : *data;
		}
	}



   	namespace ScopeTabBarMethod_ClearItemData
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_ClearItemData_item( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item )
		{
			static_cast< MyGUI::Tab * >(_native)->clearItemData(
				Convert<MyGUI::TabItem *>::From( _item ) );
		}
	}



   	namespace ScopeTabBarMethod_ClearItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_ClearItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::Tab * >(_native)->clearItemDataAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeTabBarMethod_SetItemData
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_SetItemData_item_data( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::Tab * >(_native)->setItemData(
				Convert<MyGUI::TabItem *>::From( _item ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
	}



   	namespace ScopeTabBarMethod_SetItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_SetItemDataAt_index_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::Tab * >(_native)->setItemDataAt(
				Convert<size_t>::From( _index ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
	}



   	namespace ScopeTabBarProperty_ItemSelected
	{
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabBar_GetItemSelected( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::TabItem *>::To( static_cast< MyGUI::Tab * >(_native)->getItemSelected( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportTabBar_SetItemSelected( MyGUI::Widget* _native , Convert<MyGUI::TabItem *>::Type _value )
		{
			static_cast< MyGUI::Tab * >(_native)->setItemSelected( Convert<MyGUI::TabItem *>::From( _value ) );
		}
	}



   	namespace ScopeTabBarProperty_IndexSelected
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportTabBar_GetIndexSelected( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Tab * >(_native)->getIndexSelected( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportTabBar_SetIndexSelected( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::Tab * >(_native)->setIndexSelected( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeTabBarMethod_FindItemWith
	{
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabBar_FindItemWith_name( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<MyGUI::TabItem *>::To( static_cast< MyGUI::Tab * >(_native)->findItemWith(
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeTabBarMethod_FindItemIndexWith
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportTabBar_FindItemIndexWith_name( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Tab * >(_native)->findItemIndexWith(
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeTabBarMethod_FindItemIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportTabBar_FindItemIndex_item( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Tab * >(_native)->findItemIndex(
				Convert<MyGUI::TabItem *>::From( _item ) ));
		}
	}



   	namespace ScopeTabBarMethod_GetItemIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportTabBar_GetItemIndex_item( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Tab * >(_native)->getItemIndex(
				Convert<MyGUI::TabItem *>::From( _item ) ));
		}
	}



   	namespace ScopeTabBarMethod_GetItemAt
	{
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabBar_GetItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<MyGUI::TabItem *>::To( static_cast< MyGUI::Tab * >(_native)->getItemAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeTabBarMethod_RemoveAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_RemoveAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Tab * >(_native)->removeAllItems( );
		}
	}



   	namespace ScopeTabBarMethod_RemoveItem
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_RemoveItem_item( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item )
		{
			static_cast< MyGUI::Tab * >(_native)->removeItem(
				Convert<MyGUI::TabItem *>::From( _item ) );
		}
	}



   	namespace ScopeTabBarMethod_RemoveItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportTabBar_RemoveItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::Tab * >(_native)->removeItemAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeTabBarMethod_AddItem
	{
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabBar_AddItem_name_data( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			return Convert<MyGUI::TabItem *>::To( static_cast< MyGUI::Tab * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::Any>::From( _data ) ));
		}
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabBar_AddItem_name( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<MyGUI::TabItem *>::To( static_cast< MyGUI::Tab * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeTabBarMethod_InsertItem
	{
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabBar_InsertItem_to_name_data( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _to ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			return Convert<MyGUI::TabItem *>::To( static_cast< MyGUI::Tab * >(_native)->insertItem(
				Convert<MyGUI::TabItem *>::From( _to ) ,
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::Any>::From( _data ) ));
		}
	}



   	namespace ScopeTabBarMethod_InsertItemAt
	{
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabBar_InsertItemAt_index_name_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			return Convert<MyGUI::TabItem *>::To( static_cast< MyGUI::Tab * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::Any>::From( _data ) ));
		}
		MYGUIEXPORT Convert<MyGUI::TabItem *>::Type MYGUICALL ExportTabBar_InsertItemAt_index_name( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<MyGUI::TabItem *>::To( static_cast< MyGUI::Tab * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeTabBarProperty_ItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportTabBar_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Tab * >(_native)->getItemCount( ) );
		}
	}



   


   


   


   


   


   


   


   


   


}

#endif // __EXPORT_WIDGET_TabBar_H__
