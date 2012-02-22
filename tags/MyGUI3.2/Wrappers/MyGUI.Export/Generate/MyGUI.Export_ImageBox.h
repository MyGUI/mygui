/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_ImageBox_H__
#define __EXPORT_WIDGET_ImageBox_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   


   


   


   


   	namespace ScopeImageBoxMethod_SetItemName
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetItemName_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setItemName(
				Convert<const std::string &>::From( _value ) );
		}
	}



   	namespace ScopeImageBoxMethod_SetItemGroup
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetItemGroup_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setItemGroup(
				Convert<const std::string &>::From( _value ) );
		}
	}



   	namespace ScopeImageBoxMethod_SetItemResource
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportImageBox_SetItemResource_name( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _name )
		{
			return Convert<bool>::To( static_cast< MyGUI::ImageBox * >(_native)->setItemResource(
				Convert<const std::string &>::From( _name ) ));
		}
	}



   	namespace ScopeImageBoxMethod_GetItemFrameRate
	{
		MYGUIEXPORT Convert<float>::Type MYGUICALL ExportImageBox_GetItemFrameRate_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<float>::To( static_cast< MyGUI::ImageBox * >(_native)->getItemFrameRate(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeImageBoxMethod_SetItemFrameRate
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetItemFrameRate_index_rate( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<float>::Type _rate )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setItemFrameRate(
				Convert<size_t>::From( _index ) ,
				Convert<float>::From( _rate ) );
		}
	}



   	namespace ScopeImageBoxMethod_DeleteAllItemFrames
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_DeleteAllItemFrames_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ImageBox * >(_native)->deleteAllItemFrames(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeImageBoxMethod_DeleteItemFrame
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_DeleteItemFrame_index_indexFrame( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame )
		{
			static_cast< MyGUI::ImageBox * >(_native)->deleteItemFrame(
				Convert<size_t>::From( _index ) ,
				Convert<size_t>::From( _indexFrame ) );
		}
	}



   	namespace ScopeImageBoxMethod_SetItemFrame
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetItemFrame_index_indexFrame_item( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setItemFrame(
				Convert<size_t>::From( _index ) ,
				Convert<size_t>::From( _indexFrame ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeImageBoxMethod_InsertItemFrameDublicate
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_InsertItemFrameDublicate_index_indexFrame_indexSourceFrame( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame ,
			Convert<size_t>::Type _indexSourceFrame )
		{
			static_cast< MyGUI::ImageBox * >(_native)->insertItemFrameDublicate(
				Convert<size_t>::From( _index ) ,
				Convert<size_t>::From( _indexFrame ) ,
				Convert<size_t>::From( _indexSourceFrame ) );
		}
	}



   	namespace ScopeImageBoxMethod_AddItemFrameDublicate
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_AddItemFrameDublicate_index_indexSourceFrame( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexSourceFrame )
		{
			static_cast< MyGUI::ImageBox * >(_native)->addItemFrameDublicate(
				Convert<size_t>::From( _index ) ,
				Convert<size_t>::From( _indexSourceFrame ) );
		}
	}



   	namespace ScopeImageBoxMethod_InsertItemFrame
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_InsertItemFrame_index_indexFrame_item( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::ImageBox * >(_native)->insertItemFrame(
				Convert<size_t>::From( _index ) ,
				Convert<size_t>::From( _indexFrame ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeImageBoxMethod_AddItemFrame
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_AddItemFrame_index_item( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::ImageBox * >(_native)->addItemFrame(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeImageBoxMethod_DeleteAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_DeleteAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ImageBox * >(_native)->deleteAllItems( );
		}
	}



   	namespace ScopeImageBoxMethod_DeleteItem
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_DeleteItem_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::ImageBox * >(_native)->deleteItem(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeImageBoxMethod_SetItem
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetItem_index_item( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setItem(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeImageBoxMethod_AddItem
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_AddItem_item( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::ImageBox * >(_native)->addItem(
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeImageBoxMethod_InsertItem
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_InsertItem_index_item( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::ImageBox * >(_native)->insertItem(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeImageBoxMethod_ResetItemSelect
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_ResetItemSelect( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::ImageBox * >(_native)->resetItemSelect( );
		}
	}



   	namespace ScopeImageBoxProperty_ItemSelect
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportImageBox_GetItemSelect( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ImageBox * >(_native)->getItemSelect( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetItemSelect( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setItemSelect( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeImageBoxMethod_GetItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportImageBox_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ImageBox * >(_native)->getItemCount( ) );
		}
	}



   	namespace ScopeImageBoxProperty_ImageIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportImageBox_GetImageIndex( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ImageBox * >(_native)->getImageIndex( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetImageIndex( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setImageIndex( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeImageBoxMethod_SetImageTile
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetImageTile_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TSize< int > &>::Type _value )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setImageTile(
				Convert<const MyGUI::types::TSize< int > &>::From( _value ) );
		}
	}



   	namespace ScopeImageBoxMethod_SetImageCoord
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetImageCoord_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TCoord< int > &>::Type _value )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setImageCoord(
				Convert<const MyGUI::types::TCoord< int > &>::From( _value ) );
		}
	}



   	namespace ScopeImageBoxMethod_SetImageRect
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetImageRect_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TRect< int > &>::Type _value )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setImageRect(
				Convert<const MyGUI::types::TRect< int > &>::From( _value ) );
		}
	}



   	namespace ScopeImageBoxMethod_SetImageTexture
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetImageTexture_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setImageTexture(
				Convert<const std::string &>::From( _value ) );
		}
	}



   	namespace ScopeImageBoxMethod_SetImageInfo
	{
		MYGUIEXPORT void MYGUICALL ExportImageBox_SetImageInfo_texture_coord_tile( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _texture ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _coord ,
			Convert<const MyGUI::types::TSize< int > &>::Type _tile )
		{
			static_cast< MyGUI::ImageBox * >(_native)->setImageInfo(
				Convert<const std::string &>::From( _texture ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _coord ) ,
				Convert<const MyGUI::types::TSize< int > &>::From( _tile ) );
		}
	}



   


   


   


}

#endif // __EXPORT_WIDGET_ImageBox_H__
