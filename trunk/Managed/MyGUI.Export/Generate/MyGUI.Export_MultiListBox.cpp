﻿/*!
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

   	namespace ScopeMultiListBoxEvent_OperatorLess
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<size_t>::Type ,
			Convert<const Ogre::UTFString &>::Type ,
			Convert<const Ogre::UTFString &>::Type ,
			Convert<bool &>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::MultiList* _sender,
			size_t _column ,
			const Ogre::UTFString & _firstItem ,
			const Ogre::UTFString & _secondItem ,
			bool & _less )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<size_t>::To( _column ) ,
				Convert<const Ogre::UTFString &>::To( _firstItem ) ,
				Convert<const Ogre::UTFString &>::To( _secondItem ) ,
				Convert<bool &>::To( _less ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_DelegateOperatorLess( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_AdviseOperatorLess( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::MultiList* >(_widget)->requestOperatorLess = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeMultiListBoxEvent_ListChangePosition
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::MultiList* _sender,
			size_t _index )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_DelegateListChangePosition( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_AdviseListChangePosition( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::MultiList* >(_widget)->eventListChangePosition = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeMultiListBoxEvent_ListSelectAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::MultiList* _sender,
			size_t _index )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<size_t>::To( _index ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_DelegateListSelectAccept( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportMultiListBoxEvent_AdviseListSelectAccept( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::MultiList* >(_widget)->eventListSelectAccept = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeMultiListBoxMethod_ClearSubItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_ClearSubItemDataAt_column_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::MultiList * >(_native)->clearSubItemDataAt(
				Convert<size_t>::From(_column) ,
				Convert<size_t>::From(_index) );
		}
	}



   	namespace ScopeMultiListBoxMethod_SetSubItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetSubItemDataAt_column_index_data( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column ,
			Convert<size_t>::Type _index ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::MultiList * >(_native)->setSubItemDataAt(
				Convert<size_t>::From(_column) ,
				Convert<size_t>::From(_index) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}



   	namespace ScopeMultiListBoxMethod_FindSubItemWith
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMultiListBox_FindSubItemWith_column_name( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MultiList * >(_native)->findSubItemWith(
				Convert<size_t>::From(_column) ,
				Convert<const Ogre::UTFString &>::From(_name) ) );
		}
	}



   	namespace ScopeMultiListBoxMethod_GetSubItemNameAt
	{
		MYGUIEXPORT Convert<const Ogre::UTFString &>::Type MYGUICALL ExportMultiListBox_GetSubItemNameAt_column_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column ,
			Convert<size_t>::Type _index )
		{
			return Convert<const Ogre::UTFString &>::To( static_cast< MyGUI::MultiList * >(_native)->getSubItemNameAt(
				Convert<size_t>::From(_column) ,
				Convert<size_t>::From(_index) ) );
		}
	}



   	namespace ScopeMultiListBoxMethod_SetSubItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetSubItemNameAt_column_index_name( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column ,
			Convert<size_t>::Type _index ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::MultiList * >(_native)->setSubItemNameAt(
				Convert<size_t>::From(_column) ,
				Convert<size_t>::From(_index) ,
				Convert<const Ogre::UTFString &>::From(_name) );
		}
	}



   	namespace ScopeMultiListBoxMethod_ClearItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_ClearItemDataAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::MultiList * >(_native)->clearItemDataAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace ScopeMultiListBoxMethod_SetItemDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetItemDataAt_index_data( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::MultiList * >(_native)->setItemDataAt(
				Convert<size_t>::From(_index) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}



   	namespace ScopeMultiListBoxMethod_ClearIndexSelected
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_ClearIndexSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::MultiList * >(_native)->clearIndexSelected( );
		}
	}



   	namespace ScopeMultiListBoxProperty_IndexSelected
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMultiListBox_GetIndexSelected( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MultiList * >(_native)->getIndexSelected( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetIndexSelected( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::MultiList * >(_native)->setIndexSelected( Convert<size_t>::From( _value ) );
		}
	}
	


   	namespace ScopeMultiListBoxMethod_GetItemNameAt
	{
		MYGUIEXPORT Convert<const Ogre::UTFString &>::Type MYGUICALL ExportMultiListBox_GetItemNameAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			return Convert<const Ogre::UTFString &>::To( static_cast< MyGUI::MultiList * >(_native)->getItemNameAt(
				Convert<size_t>::From(_index) ) );
		}
	}



   	namespace ScopeMultiListBoxMethod_SetItemNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetItemNameAt_index_name( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::MultiList * >(_native)->setItemNameAt(
				Convert<size_t>::From(_index) ,
				Convert<const Ogre::UTFString &>::From(_name) );
		}
	}



   	namespace ScopeMultiListBoxMethod_SwapItemsAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SwapItemsAt_index1_index2( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index1 ,
			Convert<size_t>::Type _index2 )
		{
			static_cast< MyGUI::MultiList * >(_native)->swapItemsAt(
				Convert<size_t>::From(_index1) ,
				Convert<size_t>::From(_index2) );
		}
	}



   	namespace ScopeMultiListBoxMethod_RemoveAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_RemoveAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::MultiList * >(_native)->removeAllItems( );
		}
	}



   	namespace ScopeMultiListBoxMethod_RemoveItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_RemoveItemAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::MultiList * >(_native)->removeItemAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace ScopeMultiListBoxMethod_AddItem
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_AddItem_name_data( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _name ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::MultiList * >(_native)->addItem(
				Convert<const Ogre::UTFString &>::From(_name) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}


   	namespace ScopeMultiListBoxMethod_AddItem
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_AddItem_name( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::MultiList * >(_native)->addItem(
				Convert<const Ogre::UTFString &>::From(_name) );
		}
	}



   	namespace ScopeMultiListBoxMethod_InsertItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_InsertItemAt_index_name_data( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert<const Ogre::UTFString &>::Type _name ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::MultiList * >(_native)->insertItemAt(
				Convert<size_t>::From(_index) ,
				Convert<const Ogre::UTFString &>::From(_name) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}


   	namespace ScopeMultiListBoxMethod_InsertItemAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_InsertItemAt_index_name( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::MultiList * >(_native)->insertItemAt(
				Convert<size_t>::From(_index) ,
				Convert<const Ogre::UTFString &>::From(_name) );
		}
	}



   	namespace ScopeMultiListBoxProperty_ItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMultiListBox_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MultiList * >(_native)->getItemCount( ) );
		}
	}
	


   	namespace ScopeMultiListBoxMethod_ClearColumnDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_ClearColumnDataAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::MultiList * >(_native)->clearColumnDataAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace ScopeMultiListBoxMethod_SetColumnDataAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetColumnDataAt_index_data( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::MultiList * >(_native)->setColumnDataAt(
				Convert<size_t>::From(_index) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}



   	namespace ScopeMultiListBoxMethod_SortByColumn
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SortByColumn_column_backward( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column ,
			Convert<bool>::Type _backward )
		{
			static_cast< MyGUI::MultiList * >(_native)->sortByColumn(
				Convert<size_t>::From(_column) ,
				Convert<bool>::From(_backward) );
		}
	}


   	namespace ScopeMultiListBoxMethod_SortByColumn
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SortByColumn_column( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column )
		{
			static_cast< MyGUI::MultiList * >(_native)->sortByColumn(
				Convert<size_t>::From(_column) );
		}
	}



   	namespace ScopeMultiListBoxMethod_GetColumnWidthAt
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportMultiListBox_GetColumnWidthAt_column( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column )
		{
			return Convert<int>::To( static_cast< MyGUI::MultiList * >(_native)->getColumnWidthAt(
				Convert<size_t>::From(_column) ) );
		}
	}



   	namespace ScopeMultiListBoxMethod_GetColumnNameAt
	{
		MYGUIEXPORT Convert<const Ogre::UTFString &>::Type MYGUICALL ExportMultiListBox_GetColumnNameAt_column( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column )
		{
			return Convert<const Ogre::UTFString &>::To( static_cast< MyGUI::MultiList * >(_native)->getColumnNameAt(
				Convert<size_t>::From(_column) ) );
		}
	}



   	namespace ScopeMultiListBoxMethod_SetColumnWidthAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetColumnWidthAt_column_width( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column ,
			Convert<int>::Type _width )
		{
			static_cast< MyGUI::MultiList * >(_native)->setColumnWidthAt(
				Convert<size_t>::From(_column) ,
				Convert<int>::From(_width) );
		}
	}



   	namespace ScopeMultiListBoxMethod_SetColumnNameAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_SetColumnNameAt_column_name( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::MultiList * >(_native)->setColumnNameAt(
				Convert<size_t>::From(_column) ,
				Convert<const Ogre::UTFString &>::From(_name) );
		}
	}



   	namespace ScopeMultiListBoxMethod_RemoveAllColumns
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_RemoveAllColumns( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::MultiList * >(_native)->removeAllColumns( );
		}
	}



   	namespace ScopeMultiListBoxMethod_RemoveColumnAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_RemoveColumnAt_column( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column )
		{
			static_cast< MyGUI::MultiList * >(_native)->removeColumnAt(
				Convert<size_t>::From(_column) );
		}
	}



   	namespace ScopeMultiListBoxMethod_AddColumn
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_AddColumn_name_width_data( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _name ,
			Convert<int>::Type _width ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::MultiList * >(_native)->addColumn(
				Convert<const Ogre::UTFString &>::From(_name) ,
				Convert<int>::From(_width) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}


   	namespace ScopeMultiListBoxMethod_AddColumn
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_AddColumn_name_width( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _name ,
			Convert<int>::Type _width )
		{
			static_cast< MyGUI::MultiList * >(_native)->addColumn(
				Convert<const Ogre::UTFString &>::From(_name) ,
				Convert<int>::From(_width) );
		}
	}



   	namespace ScopeMultiListBoxMethod_InsertColumnAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_InsertColumnAt_column_name_width_data( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column ,
			Convert<const Ogre::UTFString &>::Type _name ,
			Convert<int>::Type _width ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::MultiList * >(_native)->insertColumnAt(
				Convert<size_t>::From(_column) ,
				Convert<const Ogre::UTFString &>::From(_name) ,
				Convert<int>::From(_width) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}


   	namespace ScopeMultiListBoxMethod_InsertColumnAt
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListBox_InsertColumnAt_column_name_width( MyGUI::Widget* _native ,
			Convert<size_t>::Type _column ,
			Convert<const Ogre::UTFString &>::Type _name ,
			Convert<int>::Type _width )
		{
			static_cast< MyGUI::MultiList * >(_native)->insertColumnAt(
				Convert<size_t>::From(_column) ,
				Convert<const Ogre::UTFString &>::From(_name) ,
				Convert<int>::From(_width) );
		}
	}



   	namespace ScopeMultiListBoxProperty_ColumnCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMultiListBox_GetColumnCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MultiList * >(_native)->getColumnCount( ) );
		}
	}
	


}
