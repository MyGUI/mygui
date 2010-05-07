/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_MenuCtrl_H__
#define __EXPORT_WIDGET_MenuCtrl_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeMenuCtrlEvent_MenuCtrlClose
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::MenuCtrl *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::MenuCtrl * _sender )
		{
			mExportHandle(
				Convert<MyGUI::MenuCtrl *>::To( _sender ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportMenuCtrlEvent_DelegateMenuCtrlClose( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportMenuCtrlEvent_AdviseMenuCtrlClose( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::MenuCtrl* >(_widget)->eventMenuCtrlClose = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeMenuCtrlEvent_MenuCtrlAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::MenuCtrl *>::Type ,
			Convert<MyGUI::MenuItem *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::MenuCtrl * _sender ,
			MyGUI::MenuItem * _item )
		{
			mExportHandle(
				Convert<MyGUI::MenuCtrl *>::To( _sender ) ,
				Convert<MyGUI::MenuItem *>::To( _item ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportMenuCtrlEvent_DelegateMenuCtrlAccept( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportMenuCtrlEvent_AdviseMenuCtrlAccept( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::MenuCtrl* >(_widget)->eventMenuCtrlAccept = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeMenuCtrlMethod_GetMenuItemParent
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_GetMenuItemParent( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getMenuItemParent( ) );
		}
	}



   	namespace ScopeMenuCtrlProperty_PopupAccept
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportMenuCtrl_GetPopupAccept( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getPopupAccept( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetPopupAccept( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setPopupAccept( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_SetItemType
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetItemType_item_type( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setItemType(
				Convert<MyGUI::MenuItem *>::From( _item ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_SetItemTypeAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetItemTypeAt_index_type( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setItemTypeAt(
				Convert<size_t>::From( _index ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemType
	{
		MYGUIEXPORT Convert<MyGUI::MenuItemType>::Type MYGUICALL ExportMenuCtrl_GetItemType_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<MyGUI::MenuItemType>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemType(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemTypeAt
	{
		MYGUIEXPORT Convert<MyGUI::MenuItemType>::Type MYGUICALL ExportMenuCtrl_GetItemTypeAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<MyGUI::MenuItemType>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemTypeAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_RemoveItemChild
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_RemoveItemChild_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->removeItemChild(
				Convert<MyGUI::MenuItem *>::From( _item ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_RemoveItemChildAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_RemoveItemChildAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->removeItemChildAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_CreateItemChild
	{
		MYGUIEXPORT Convert<MyGUI::MenuCtrl *>::Type MYGUICALL ExportMenuCtrl_CreateItemChild_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<MyGUI::MenuCtrl *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->createItemChild(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_CreateItemChildAt
	{
		MYGUIEXPORT Convert<MyGUI::MenuCtrl *>::Type MYGUICALL ExportMenuCtrl_CreateItemChildAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<MyGUI::MenuCtrl *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->createItemChildAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemChild
	{
		MYGUIEXPORT Convert<MyGUI::MenuCtrl *>::Type MYGUICALL ExportMenuCtrl_GetItemChild_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<MyGUI::MenuCtrl *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemChild(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemChildAt
	{
		MYGUIEXPORT Convert<MyGUI::MenuCtrl *>::Type MYGUICALL ExportMenuCtrl_GetItemChildAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<MyGUI::MenuCtrl *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemChildAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   


   


   	namespace ScopeMenuCtrlMethod_SetItemChildVisible
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetItemChildVisible_item_visible( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<bool>::Type _visible )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setItemChildVisible(
				Convert<MyGUI::MenuItem *>::From( _item ) ,
				Convert<bool>::From( _visible ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_SetItemChildVisibleAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetItemChildVisibleAt_index_visible( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<bool>::Type _visible )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setItemChildVisibleAt(
				Convert<size_t>::From( _index ) ,
				Convert<bool>::From( _visible ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_FindItemIndexWith
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMenuCtrl_FindItemIndexWith_name( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MenuCtrl * >(_native)->findItemIndexWith(
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemName
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportMenuCtrl_GetItemName_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<const MyGUI::UString &>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemName(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemNameAt
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportMenuCtrl_GetItemNameAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<const MyGUI::UString &>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemNameAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_SetItemName
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetItemName_item_name( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setItemName(
				Convert<MyGUI::MenuItem *>::From( _item ) ,
				Convert<const MyGUI::UString &>::From( _name ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_SetItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetItemNameAt_index_name( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setItemNameAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemIndexById
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMenuCtrl_GetItemIndexById_id( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _id )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemIndexById(
				Convert<const std::string &>::From( _id ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemById
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_GetItemById_id( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _id )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemById(
				Convert<const std::string &>::From( _id ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemId
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportMenuCtrl_GetItemId_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemId(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemIdAt
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportMenuCtrl_GetItemIdAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemIdAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_SetItemId
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetItemId_item_id( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<const std::string &>::Type _id )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setItemId(
				Convert<MyGUI::MenuItem *>::From( _item ) ,
				Convert<const std::string &>::From( _id ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_SetItemIdAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetItemIdAt_index_id( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const std::string &>::Type _id )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setItemIdAt(
				Convert<size_t>::From( _index ) ,
				Convert<const std::string &>::From( _id ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemData
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportMenuCtrl_GetItemData_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			Convert<MyGUI::Any>::Type* data = 
				static_cast< MyGUI::MenuCtrl * >(_native)->getItemData< Convert<MyGUI::Any>::Type >(
					Convert<MyGUI::MenuItem *>::From( _item ) , false );
			return data == nullptr ? nullptr : *data;
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemDataAt
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportMenuCtrl_GetItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			Convert<MyGUI::Any>::Type* data = 
				static_cast< MyGUI::MenuCtrl * >(_native)->getItemDataAt< Convert<MyGUI::Any>::Type >(
					Convert<size_t>::From( _index ) , false );
			return data == nullptr ? nullptr : *data;
		}
	}



   	namespace ScopeMenuCtrlMethod_ClearItemData
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_ClearItemData_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->clearItemData(
				Convert<MyGUI::MenuItem *>::From( _item ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_ClearItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_ClearItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->clearItemDataAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_SetItemData
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetItemData_item_data( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setItemData(
				Convert<MyGUI::MenuItem *>::From( _item ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_SetItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetItemDataAt_index_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setItemDataAt(
				Convert<size_t>::From( _index ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_FindItemWith
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_FindItemWith_name( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->findItemWith(
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_FindItemIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMenuCtrl_FindItemIndex_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MenuCtrl * >(_native)->findItemIndex(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMenuCtrl_GetItemIndex_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemIndex(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_GetItemAt
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_GetItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_RemoveAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_RemoveAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->removeAllItems( );
		}
	}



   	namespace ScopeMenuCtrlMethod_RemoveItem
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_RemoveItem_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->removeItem(
				Convert<MyGUI::MenuItem *>::From( _item ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_RemoveItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_RemoveItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->removeItemAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_AddItem
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_AddItem_name_type_id_data( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id ,
			Convert<MyGUI::Any>::Type _data )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ,
				Convert<const std::string &>::From( _id ) ,
				Convert<MyGUI::Any>::From( _data ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_AddItem_name_type_id( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ,
				Convert<const std::string &>::From( _id ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_AddItem_name_type( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_AddItem_name( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_InsertItem
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_InsertItem_to_name_type_id_data( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _to ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id ,
			Convert<MyGUI::Any>::Type _data )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->insertItem(
				Convert<MyGUI::MenuItem *>::From( _to ) ,
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ,
				Convert<const std::string &>::From( _id ) ,
				Convert<MyGUI::Any>::From( _data ) ));
		}
	}



   	namespace ScopeMenuCtrlMethod_InsertItemAt
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_InsertItemAt_index_name_type_id_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id ,
			Convert<MyGUI::Any>::Type _data )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ,
				Convert<const std::string &>::From( _id ) ,
				Convert<MyGUI::Any>::From( _data ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_InsertItemAt_index_name_type_id( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ,
				Convert<const std::string &>::From( _id ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_InsertItemAt_index_name_type( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuCtrl_InsertItemAt_index_name( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuCtrl * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeMenuCtrlProperty_ItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMenuCtrl_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MenuCtrl * >(_native)->getItemCount( ) );
		}
	}



   	namespace ScopeMenuCtrlMethod_SetVisibleSmooth
	{
		MYGUIEXPORT void MYGUICALL ExportMenuCtrl_SetVisibleSmooth_value( MyGUI::Widget* _native,
			Convert<bool>::Type _value )
		{
			static_cast< MyGUI::MenuCtrl * >(_native)->setVisibleSmooth(
				Convert<bool>::From( _value ) );
		}
	}



   


   


   


   


}

#endif // __EXPORT_WIDGET_MenuCtrl_H__
