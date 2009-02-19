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
		MYGUIEXPORT void MYGUICALL ExportItemBox_ResetDrag( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ItemBox * >(_native)->resetDrag( );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportItemBox_GetWidgetByIndex_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::ItemBox * >(_native)->getWidgetByIndex(
				Convert<size_t>::From(_index) ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportItemBox_GetWidgetDrag( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::ItemBox * >(_native)->getWidgetDrag( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportItemBox_GetIndexByWidget_widget( MyGUI::Widget* _native ,
			Convert< MyGUI::Widget * >::Type _widget )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ItemBox * >(_native)->getIndexByWidget(
				Convert< MyGUI::Widget * >::From(_widget) ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportItemBox_GetItemBoxAlignVert( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::ItemBox * >(_native)->getItemBoxAlignVert( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportItemBox_SetItemBoxAlignVert( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::ItemBox * >(_native)->setItemBoxAlignVert( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_ClearItemDataAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ItemBox * >(_native)->clearItemDataAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_SetItemDataAt_index_data( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::ItemBox * >(_native)->setItemDataAt(
				Convert<size_t>::From(_index) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_ClearIndexSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ItemBox * >(_native)->clearIndexSelected( );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportItemBox_GetIndexSelected( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ItemBox * >(_native)->getIndexSelected( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportItemBox_SetIndexSelected( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ItemBox * >(_native)->setIndexSelected( Convert<size_t>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_RedrawAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ItemBox * >(_native)->redrawAllItems( );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_RedrawItemAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ItemBox * >(_native)->redrawItemAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_RemoveAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ItemBox * >(_native)->removeAllItems( );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_RemoveItemAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ItemBox * >(_native)->removeItemAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_AddItem_data( MyGUI::Widget* _native ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::ItemBox * >(_native)->addItem(
				Convert< MyGUI::Any >::From(_data) );
		}
	}


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_AddItem( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ItemBox * >(_native)->addItem( );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_InsertItemAt_index_data( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index ,
			Convert< MyGUI::Any >::Type _data )
		{
			static_cast< MyGUI::ItemBox * >(_native)->insertItemAt(
				Convert<size_t>::From(_index) ,
				Convert< MyGUI::Any >::From(_data) );
		}
	}


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportItemBox_InsertItemAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ItemBox * >(_native)->insertItemAt(
				Convert<size_t>::From(_index) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportItemBox_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ItemBox * >(_native)->getItemCount( ) );
		}
	}
	


}
