/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_StaticImage_H__
#define __EXPORT_WIDGET_StaticImage_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   


   


   


   


   


   	namespace ScopeStaticImageMethod_SetItemName
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetItemName_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setItemName(
				Convert<const std::string &>::From( _value ) );
		}
	}



   	namespace ScopeStaticImageMethod_SetItemGroup
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetItemGroup_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setItemGroup(
				Convert<const std::string &>::From( _value ) );
		}
	}



   	namespace ScopeStaticImageMethod_SetItemResource
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportStaticImage_SetItemResource_name( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _name )
		{
			return Convert<bool>::To( static_cast< MyGUI::StaticImage * >(_native)->setItemResource(
				Convert<const std::string &>::From( _name ) ));
		}
	}



   	namespace ScopeStaticImageMethod_SetItemResource
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportStaticImage_SetItemResource_id( MyGUI::Widget* _native,
			Convert<const MyGUI::Guid &>::Type _id )
		{
			return Convert<bool>::To( static_cast< MyGUI::StaticImage * >(_native)->setItemResource(
				Convert<const MyGUI::Guid &>::From( _id ) ));
		}
	}



   	namespace ScopeStaticImageMethod_GetItemFrameRate
	{
		MYGUIEXPORT Convert<float>::Type MYGUICALL ExportStaticImage_GetItemFrameRate_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<float>::To( static_cast< MyGUI::StaticImage * >(_native)->getItemFrameRate(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeStaticImageMethod_SetItemFrameRate
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetItemFrameRate_index_rate( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<float>::Type _rate )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setItemFrameRate(
				Convert<size_t>::From( _index ) ,
				Convert<float>::From( _rate ) );
		}
	}



   	namespace ScopeStaticImageMethod_DeleteAllItemFrames
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_DeleteAllItemFrames_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::StaticImage * >(_native)->deleteAllItemFrames(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeStaticImageMethod_DeleteItemFrame
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_DeleteItemFrame_index_indexFrame( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame )
		{
			static_cast< MyGUI::StaticImage * >(_native)->deleteItemFrame(
				Convert<size_t>::From( _index ) ,
				Convert<size_t>::From( _indexFrame ) );
		}
	}



   	namespace ScopeStaticImageMethod_SetItemFrame
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetItemFrame_index_indexFrame_item( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setItemFrame(
				Convert<size_t>::From( _index ) ,
				Convert<size_t>::From( _indexFrame ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeStaticImageMethod_InsertItemFrameDublicate
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_InsertItemFrameDublicate_index_indexFrame_indexSourceFrame( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame ,
			Convert<size_t>::Type _indexSourceFrame )
		{
			static_cast< MyGUI::StaticImage * >(_native)->insertItemFrameDublicate(
				Convert<size_t>::From( _index ) ,
				Convert<size_t>::From( _indexFrame ) ,
				Convert<size_t>::From( _indexSourceFrame ) );
		}
	}



   	namespace ScopeStaticImageMethod_AddItemFrameDublicate
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_AddItemFrameDublicate_index_indexSourceFrame( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexSourceFrame )
		{
			static_cast< MyGUI::StaticImage * >(_native)->addItemFrameDublicate(
				Convert<size_t>::From( _index ) ,
				Convert<size_t>::From( _indexSourceFrame ) );
		}
	}



   	namespace ScopeStaticImageMethod_InsertItemFrame
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_InsertItemFrame_index_indexFrame_item( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::StaticImage * >(_native)->insertItemFrame(
				Convert<size_t>::From( _index ) ,
				Convert<size_t>::From( _indexFrame ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeStaticImageMethod_AddItemFrame
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_AddItemFrame_index_item( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::StaticImage * >(_native)->addItemFrame(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeStaticImageMethod_DeleteAllItems
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_DeleteAllItems( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::StaticImage * >(_native)->deleteAllItems( );
		}
	}



   	namespace ScopeStaticImageMethod_DeleteItem
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_DeleteItem_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::StaticImage * >(_native)->deleteItem(
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeStaticImageMethod_SetItem
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetItem_index_item( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setItem(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeStaticImageMethod_AddItem
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_AddItem_item( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::StaticImage * >(_native)->addItem(
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeStaticImageMethod_InsertItem
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_InsertItem_index_item( MyGUI::Widget* _native,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			static_cast< MyGUI::StaticImage * >(_native)->insertItem(
				Convert<size_t>::From( _index ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _item ) );
		}
	}



   	namespace ScopeStaticImageMethod_ResetItemSelect
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_ResetItemSelect( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::StaticImage * >(_native)->resetItemSelect( );
		}
	}



   	namespace ScopeStaticImageProperty_ItemSelect
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportStaticImage_GetItemSelect( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::StaticImage * >(_native)->getItemSelect( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetItemSelect( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setItemSelect( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeStaticImageMethod_GetItemCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportStaticImage_GetItemCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::StaticImage * >(_native)->getItemCount( ) );
		}
	}



   	namespace ScopeStaticImageProperty_ImageIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportStaticImage_GetImageIndex( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::StaticImage * >(_native)->getImageIndex( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetImageIndex( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setImageIndex( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeStaticImageMethod_SetImageTile
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetImageTile_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TSize< int > &>::Type _value )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setImageTile(
				Convert<const MyGUI::types::TSize< int > &>::From( _value ) );
		}
	}



   	namespace ScopeStaticImageMethod_SetImageCoord
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetImageCoord_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TCoord< int > &>::Type _value )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setImageCoord(
				Convert<const MyGUI::types::TCoord< int > &>::From( _value ) );
		}
	}



   	namespace ScopeStaticImageMethod_SetImageRect
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetImageRect_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TRect< int > &>::Type _value )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setImageRect(
				Convert<const MyGUI::types::TRect< int > &>::From( _value ) );
		}
	}



   	namespace ScopeStaticImageMethod_SetImageTexture
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetImageTexture_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setImageTexture(
				Convert<const std::string &>::From( _value ) );
		}
	}



   	namespace ScopeStaticImageMethod_SetImageInfo
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetImageInfo_texture_coord_tile( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _texture ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _coord ,
			Convert<const MyGUI::types::TSize< int > &>::Type _tile )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setImageInfo(
				Convert<const std::string &>::From( _texture ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _coord ) ,
				Convert<const MyGUI::types::TSize< int > &>::From( _tile ) );
		}
	}



   


   


   


}

#endif // __EXPORT_WIDGET_StaticImage_H__
