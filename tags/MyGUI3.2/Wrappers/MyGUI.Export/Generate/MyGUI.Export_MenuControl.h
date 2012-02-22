/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_MenuControl_H__
#define __EXPORT_WIDGET_MenuControl_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeMenuControlEvent_MenuCtrlClose
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::MenuControl *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::MenuControl * _sender )
		{
			mExportHandle(
				Convert<MyGUI::MenuControl *>::To( _sender ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportMenuControlEvent_DelegateMenuCtrlClose( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportMenuControlEvent_AdviseMenuCtrlClose( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::MenuControl* >(_widget)->eventMenuCtrlClose += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::MenuControl* >(_widget)->eventMenuCtrlClose -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeMenuControlEvent_MenuCtrlAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::MenuControl *>::Type ,
			Convert<MyGUI::MenuItem *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::MenuControl * _sender ,
			MyGUI::MenuItem * _item )
		{
			mExportHandle(
				Convert<MyGUI::MenuControl *>::To( _sender ) ,
				Convert<MyGUI::MenuItem *>::To( _item ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportMenuControlEvent_DelegateMenuCtrlAccept( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportMenuControlEvent_AdviseMenuCtrlAccept( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::MenuControl* >(_widget)->eventMenuCtrlAccept += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::MenuControl* >(_widget)->eventMenuCtrlAccept -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeMenuControlProperty_VerticalAlignment
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportMenuControl_GetVerticalAlignment( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::MenuControl * >(_native)->getVerticalAlignment( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetVerticalAlignment( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setVerticalAlignment( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeMenuControlMethod_GetMenuItemParent
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_GetMenuItemParent( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->getMenuItemParent( ) );
		}
	}



   	namespace ScopeMenuControlProperty_PopupAccept
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportMenuControl_GetPopupAccept( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::MenuControl * >(_native)->getPopupAccept( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetPopupAccept( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setPopupAccept( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeMenuControlMethod_SetItemType
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetItemType_item_type( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setItemType(
				Convert<MyGUI::MenuItem *>::From( _item ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) );
		}
	}



   	namespace ScopeMenuControlMethod_SetItemTypeAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetItemTypeAt_index_type( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setItemTypeAt(
				Convert<size_t>::From( _index ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) );
		}
	}



   	namespace ScopeMenuControlMethod_GetItemType
	{
		MYGUIEXPORT Convert<MyGUI::MenuItemType>::Type MYGUICALL ExportMenuControl_GetItemType_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<MyGUI::MenuItemType>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemType(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuControlMethod_GetItemTypeAt
	{
		MYGUIEXPORT Convert<MyGUI::MenuItemType>::Type MYGUICALL ExportMenuControl_GetItemTypeAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<MyGUI::MenuItemType>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemTypeAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeMenuControlMethod_RemoveItemChild
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_RemoveItemChild_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			static_cast< MyGUI::MenuControl * >(_native)->removeItemChild(
				Convert<MyGUI::MenuItem *>::From( _item ) );
		}
	}



   	namespace ScopeMenuControlMethod_RemoveItemChildAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_RemoveItemChildAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::MenuControl * >(_native)->removeItemChildAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeMenuControlMethod_CreateItemChild
	{
		MYGUIEXPORT Convert<MyGUI::MenuControl *>::Type MYGUICALL ExportMenuControl_CreateItemChild_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<MyGUI::MenuControl *>::To( static_cast< MyGUI::MenuControl * >(_native)->createItemChild(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuControlMethod_CreateItemChildAt
	{
		MYGUIEXPORT Convert<MyGUI::MenuControl *>::Type MYGUICALL ExportMenuControl_CreateItemChildAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<MyGUI::MenuControl *>::To( static_cast< MyGUI::MenuControl * >(_native)->createItemChildAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeMenuControlMethod_GetItemChild
	{
		MYGUIEXPORT Convert<MyGUI::MenuControl *>::Type MYGUICALL ExportMenuControl_GetItemChild_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<MyGUI::MenuControl *>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemChild(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuControlMethod_GetItemChildAt
	{
		MYGUIEXPORT Convert<MyGUI::MenuControl *>::Type MYGUICALL ExportMenuControl_GetItemChildAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<MyGUI::MenuControl *>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemChildAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   


   


   	namespace ScopeMenuControlMethod_SetItemChildVisible
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetItemChildVisible_item_visible( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<bool>::Type _visible )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setItemChildVisible(
				Convert<MyGUI::MenuItem *>::From( _item ) ,
				Convert<bool>::From( _visible ) );
		}
	}



   	namespace ScopeMenuControlMethod_SetItemChildVisibleAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetItemChildVisibleAt_index_visible( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<bool>::Type _visible )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setItemChildVisibleAt(
				Convert<size_t>::From( _index ) ,
				Convert<bool>::From( _visible ) );
		}
	}



   	namespace ScopeMenuControlMethod_FindItemIndexWith
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMenuControl_FindItemIndexWith_name( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MenuControl * >(_native)->findItemIndexWith(
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeMenuControlMethod_GetItemName
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportMenuControl_GetItemName_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<const MyGUI::UString &>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemName(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuControlMethod_GetItemNameAt
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportMenuControl_GetItemNameAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<const MyGUI::UString &>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemNameAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeMenuControlMethod_SetItemName
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetItemName_item_name( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setItemName(
				Convert<MyGUI::MenuItem *>::From( _item ) ,
				Convert<const MyGUI::UString &>::From( _name ) );
		}
	}



   	namespace ScopeMenuControlMethod_SetItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetItemNameAt_index_name( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setItemNameAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) );
		}
	}



   	namespace ScopeMenuControlMethod_GetItemIndexById
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMenuControl_GetItemIndexById_id( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _id )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemIndexById(
				Convert<const std::string &>::From( _id ) ));
		}
	}



   	namespace ScopeMenuControlMethod_FindItemById
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_FindItemById_id_recursive( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _id ,
			Convert<bool>::Type _recursive )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->findItemById(
				Convert<const std::string &>::From( _id ) ,
				Convert<bool>::From( _recursive ) ));
		}
	}



   	namespace ScopeMenuControlMethod_GetItemById
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_GetItemById_id( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _id )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemById(
				Convert<const std::string &>::From( _id ) ));
		}
	}



   	namespace ScopeMenuControlMethod_GetItemId
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportMenuControl_GetItemId_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemId(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuControlMethod_GetItemIdAt
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportMenuControl_GetItemIdAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemIdAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeMenuControlMethod_SetItemId
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetItemId_item_id( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<const std::string &>::Type _id )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setItemId(
				Convert<MyGUI::MenuItem *>::From( _item ) ,
				Convert<const std::string &>::From( _id ) );
		}
	}



   	namespace ScopeMenuControlMethod_SetItemIdAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetItemIdAt_index_id( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const std::string &>::Type _id )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setItemIdAt(
				Convert<size_t>::From( _index ) ,
				Convert<const std::string &>::From( _id ) );
		}
	}



   	namespace ScopeMenuControlMethod_GetItemData
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportMenuControl_GetItemData_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			Convert<MyGUI::Any>::Type* data = 
				static_cast< MyGUI::MenuControl * >(_native)->getItemData< Convert<MyGUI::Any>::Type >(
					Convert<MyGUI::MenuItem *>::From( _item ) , false );
			return data == nullptr ? nullptr : *data;
		}
	}



   	namespace ScopeMenuControlMethod_GetItemDataAt
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportMenuControl_GetItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			Convert<MyGUI::Any>::Type* data = 
				static_cast< MyGUI::MenuControl * >(_native)->getItemDataAt< Convert<MyGUI::Any>::Type >(
					Convert<size_t>::From( _index ) , false );
			return data == nullptr ? nullptr : *data;
		}
	}



   	namespace ScopeMenuControlMethod_ClearItemData
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_ClearItemData_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			static_cast< MyGUI::MenuControl * >(_native)->clearItemData(
				Convert<MyGUI::MenuItem *>::From( _item ) );
		}
	}



   	namespace ScopeMenuControlMethod_ClearItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_ClearItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::MenuControl * >(_native)->clearItemDataAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeMenuControlMethod_SetItemData
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetItemData_item_data( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setItemData(
				Convert<MyGUI::MenuItem *>::From( _item ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
	}



   	namespace ScopeMenuControlMethod_SetItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetItemDataAt_index_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setItemDataAt(
				Convert<size_t>::From( _index ) ,
				Convert<MyGUI::Any>::From( _data ) );
		}
	}



   	namespace ScopeMenuControlMethod_FindItemWith
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_FindItemWith_name( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->findItemWith(
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeMenuControlMethod_FindItemIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMenuControl_FindItemIndex_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MenuControl * >(_native)->findItemIndex(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuControlMethod_GetItemIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMenuControl_GetItemIndex_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemIndex(
				Convert<MyGUI::MenuItem *>::From( _item ) ));
		}
	}



   	namespace ScopeMenuControlMethod_GetItemAt
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_GetItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeMenuControlMethod_RemoveAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_RemoveAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::MenuControl * >(_native)->removeAllItems( );
		}
	}



   	namespace ScopeMenuControlMethod_RemoveItem
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_RemoveItem_item( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			static_cast< MyGUI::MenuControl * >(_native)->removeItem(
				Convert<MyGUI::MenuItem *>::From( _item ) );
		}
	}



   	namespace ScopeMenuControlMethod_RemoveItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_RemoveItemAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::MenuControl * >(_native)->removeItemAt(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeMenuControlMethod_AddItem
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_AddItem_name_type_id_data( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id ,
			Convert<MyGUI::Any>::Type _data )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ,
				Convert<const std::string &>::From( _id ) ,
				Convert<MyGUI::Any>::From( _data ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_AddItem_name_type_id( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ,
				Convert<const std::string &>::From( _id ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_AddItem_name_type( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_AddItem_name( MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->addItem(
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeMenuControlMethod_InsertItem
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_InsertItem_to_name_type_id_data( MyGUI::Widget* _native,
			Convert<MyGUI::MenuItem *>::Type _to ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id ,
			Convert<MyGUI::Any>::Type _data )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->insertItem(
				Convert<MyGUI::MenuItem *>::From( _to ) ,
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ,
				Convert<const std::string &>::From( _id ) ,
				Convert<MyGUI::Any>::From( _data ) ));
		}
	}



   	namespace ScopeMenuControlMethod_InsertItemAt
	{
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_InsertItemAt_index_name_type_id_data( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id ,
			Convert<MyGUI::Any>::Type _data )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ,
				Convert<const std::string &>::From( _id ) ,
				Convert<MyGUI::Any>::From( _data ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_InsertItemAt_index_name_type_id( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ,
				Convert<const std::string &>::From( _id ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_InsertItemAt_index_name_type( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) ,
				Convert<MyGUI::MenuItemType>::From( _type ) ));
		}
		MYGUIEXPORT Convert<MyGUI::MenuItem *>::Type MYGUICALL ExportMenuControl_InsertItemAt_index_name( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			return Convert<MyGUI::MenuItem *>::To( static_cast< MyGUI::MenuControl * >(_native)->insertItemAt(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::UString &>::From( _name ) ));
		}
	}



   	namespace ScopeMenuControlProperty_ItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMenuControl_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MenuControl * >(_native)->getItemCount( ) );
		}
	}



   	namespace ScopeMenuControlMethod_SetVisibleSmooth
	{
		MYGUIEXPORT void MYGUICALL ExportMenuControl_SetVisibleSmooth_value( MyGUI::Widget* _native,
			Convert<bool>::Type _value )
		{
			static_cast< MyGUI::MenuControl * >(_native)->setVisibleSmooth(
				Convert<bool>::From( _value ) );
		}
	}



   


   


   


   


}

#endif // __EXPORT_WIDGET_MenuControl_H__
