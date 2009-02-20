/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeListEvent_ListChangeScroll
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::List* _sender,
			size_t _position )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<size_t>::To( _position ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportListEvent_DelegateListChangeScroll( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportListEvent_AdviseListChangeScroll( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::List* >(_widget)->eventListChangeScroll = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeListEvent_ListMouseItemFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::List* _sender,
			size_t _index )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportListEvent_DelegateListMouseItemFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportListEvent_AdviseListMouseItemFocus( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::List* >(_widget)->eventListMouseItemFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeListEvent_ListMouseItemActivate
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::List* _sender,
			size_t _index )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportListEvent_DelegateListMouseItemActivate( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportListEvent_AdviseListMouseItemActivate( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::List* >(_widget)->eventListMouseItemActivate = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeListEvent_ListChangePosition
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::List* _sender,
			size_t _index )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportListEvent_DelegateListChangePosition( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportListEvent_AdviseListChangePosition( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::List* >(_widget)->eventListChangePosition = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeListEvent_ListSelectAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::List* _sender,
			size_t _index )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportListEvent_DelegateListSelectAccept( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportListEvent_AdviseListSelectAccept( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::List* >(_widget)->eventListSelectAccept = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeListMethod_GetOptimalHeight
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportList_GetOptimalHeight( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::List * >(_native)->getOptimalHeight( ) );
		}
	}



   	namespace ScopeListMethod_SetScrollVisible
	{
		MYGUIEXPORT void MYGUICALL ExportList_SetScrollVisible_visible( MyGUI::Widget* _native ,
			Convert<bool>::Type _visible )
		{
			static_cast< MyGUI::List * >(_native)->setScrollVisible(
				Convert<bool>::From(_visible) );
		}
	}



   	namespace ScopeListMethod_IsItemSelectedVisible
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportList_IsItemSelectedVisible_fill( MyGUI::Widget* _native ,
			Convert<bool>::Type _fill )
		{
			return Convert<bool>::To( static_cast< MyGUI::List * >(_native)->isItemSelectedVisible(
				Convert<bool>::From(_fill) ) );
		}
	}


   	namespace ScopeListMethod_IsItemSelectedVisible
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportList_IsItemSelectedVisible( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::List * >(_native)->isItemSelectedVisible( ) );
		}
	}



   	namespace ScopeListMethod_IsItemVisibleAt
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportList_IsItemVisibleAt_index_fill( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert<bool>::Type _fill )
		{
			return Convert<bool>::To( static_cast< MyGUI::List * >(_native)->isItemVisibleAt(
				Convert<size_t>::From(_index) ,
				Convert<bool>::From(_fill) ) );
		}
	}


   	namespace ScopeListMethod_IsItemVisibleAt
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportList_IsItemVisibleAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			return Convert<bool>::To( static_cast< MyGUI::List * >(_native)->isItemVisibleAt(
				Convert<size_t>::From(_index) ) );
		}
	}



   	namespace ScopeListMethod_BeginToItemSelected
	{
		MYGUIEXPORT void MYGUICALL ExportList_BeginToItemSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::List * >(_native)->beginToItemSelected( );
		}
	}



   	namespace ScopeListMethod_BeginToItemLast
	{
		MYGUIEXPORT void MYGUICALL ExportList_BeginToItemLast( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::List * >(_native)->beginToItemLast( );
		}
	}



   	namespace ScopeListMethod_BeginToItemFirst
	{
		MYGUIEXPORT void MYGUICALL ExportList_BeginToItemFirst( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::List * >(_native)->beginToItemFirst( );
		}
	}



   	namespace ScopeListMethod_BeginToItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportList_BeginToItemAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::List * >(_native)->beginToItemAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace ScopeListMethod_GetItemNameAt
	{
		MYGUIEXPORT Convert<const Ogre::UTFString &>::Type MYGUICALL ExportList_GetItemNameAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			return Convert<const Ogre::UTFString &>::To( static_cast< MyGUI::List * >(_native)->getItemNameAt(
				Convert<size_t>::From(_index) ) );
		}
	}



   	namespace ScopeListMethod_SetItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportList_SetItemNameAt_index_name( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::List * >(_native)->setItemNameAt(
				Convert<size_t>::From(_index) ,
				Convert<const Ogre::UTFString &>::From(_name) );
		}
	}



   	namespace ScopeListMethod_ClearItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportList_ClearItemDataAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::List * >(_native)->clearItemDataAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace ScopeListMethod_SetItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportList_SetItemDataAt_index_data( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::List * >(_native)->setItemDataAt(
				Convert<size_t>::From(_index) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}



   	namespace ScopeListMethod_ClearIndexSelected
	{
		MYGUIEXPORT void MYGUICALL ExportList_ClearIndexSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::List * >(_native)->clearIndexSelected( );
		}
	}



   	namespace ScopeListProperty_IndexSelected
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportList_GetIndexSelected( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::List * >(_native)->getIndexSelected( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportList_SetIndexSelected( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::List * >(_native)->setIndexSelected( Convert<size_t>::From( _value ) );
		}
	}
	


   	namespace ScopeListMethod_FindItemIndexWith
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportList_FindItemIndexWith_name( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			return Convert<size_t>::To( static_cast< MyGUI::List * >(_native)->findItemIndexWith(
				Convert<const Ogre::UTFString &>::From(_name) ) );
		}
	}



   	namespace ScopeListMethod_SwapItemsAt
	{
		MYGUIEXPORT void MYGUICALL ExportList_SwapItemsAt_index1_index2( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index1 ,
			Convert<size_t>::Type _index2 )
		{
			static_cast< MyGUI::List * >(_native)->swapItemsAt(
				Convert<size_t>::From(_index1) ,
				Convert<size_t>::From(_index2) );
		}
	}



   	namespace ScopeListMethod_RemoveAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportList_RemoveAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::List * >(_native)->removeAllItems( );
		}
	}



   	namespace ScopeListMethod_RemoveItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportList_RemoveItemAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::List * >(_native)->removeItemAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace ScopeListMethod_AddItem
	{
		MYGUIEXPORT void MYGUICALL ExportList_AddItem_name_data( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _name ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::List * >(_native)->addItem(
				Convert<const Ogre::UTFString &>::From(_name) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}


   	namespace ScopeListMethod_AddItem
	{
		MYGUIEXPORT void MYGUICALL ExportList_AddItem_name( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::List * >(_native)->addItem(
				Convert<const Ogre::UTFString &>::From(_name) );
		}
	}



   	namespace ScopeListMethod_InsertItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportList_InsertItemAt_index_name_data( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert<const Ogre::UTFString &>::Type _name ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::List * >(_native)->insertItemAt(
				Convert<size_t>::From(_index) ,
				Convert<const Ogre::UTFString &>::From(_name) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}


   	namespace ScopeListMethod_InsertItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportList_InsertItemAt_index_name( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::List * >(_native)->insertItemAt(
				Convert<size_t>::From(_index) ,
				Convert<const Ogre::UTFString &>::From(_name) );
		}
	}



   	namespace ScopeListProperty_ItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportList_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::List * >(_native)->getItemCount( ) );
		}
	}
	


}
