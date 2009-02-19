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

   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportList_GetOptimalHeight( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::List * >(_native)->getOptimalHeight( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportList_SetScrollVisible_visible( MyGUI::Widget* _native ,
			Convert<bool>::Type _visible )
		{
			static_cast< MyGUI::List * >(_native)->setScrollVisible(
				Convert<bool>::From(_visible) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportList_IsItemSelectedVisible_fill( MyGUI::Widget* _native ,
			Convert<bool>::Type _fill )
		{
			return Convert<bool>::To( static_cast< MyGUI::List * >(_native)->isItemSelectedVisible(
				Convert<bool>::From(_fill) ) );
		}
	}


   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportList_IsItemSelectedVisible( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::List * >(_native)->isItemSelectedVisible( ) );
		}
	}



   	namespace
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


   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportList_IsItemVisibleAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			return Convert<bool>::To( static_cast< MyGUI::List * >(_native)->isItemVisibleAt(
				Convert<size_t>::From(_index) ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportList_BeginToItemSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::List * >(_native)->beginToItemSelected( );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportList_BeginToItemLast( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::List * >(_native)->beginToItemLast( );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportList_BeginToItemFirst( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::List * >(_native)->beginToItemFirst( );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportList_BeginToItemAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::List * >(_native)->beginToItemAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<const Ogre::UTFString &>::Type MYGUICALL ExportList_GetItemNameAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			return Convert<const Ogre::UTFString &>::To( static_cast< MyGUI::List * >(_native)->getItemNameAt(
				Convert<size_t>::From(_index) ) );
		}
	}



   	namespace
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



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportList_ClearItemDataAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::List * >(_native)->clearItemDataAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace
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



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportList_ClearIndexSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::List * >(_native)->clearIndexSelected( );
		}
	}



   	namespace
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
	


   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportList_FindItemIndexWith_name( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			return Convert<size_t>::To( static_cast< MyGUI::List * >(_native)->findItemIndexWith(
				Convert<const Ogre::UTFString &>::From(_name) ) );
		}
	}



   	namespace
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



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportList_RemoveAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::List * >(_native)->removeAllItems( );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportList_RemoveItemAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::List * >(_native)->removeItemAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace
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


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportList_AddItem_name( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			static_cast< MyGUI::List * >(_native)->addItem(
				Convert<const Ogre::UTFString &>::From(_name) );
		}
	}



   	namespace
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


   	namespace
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



   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportList_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::List * >(_native)->getItemCount( ) );
		}
	}
	


}
