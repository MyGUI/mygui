/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeStaticImageMethod_SetProperty
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_SetProperty_key_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::StaticImage * >(_native)->setProperty(
				Convert<const std::string &>::From( _key ) ,
				Convert<const std::string &>::From( _value ) );
		}
	}



   


   


   


   


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



   


   


   	namespace ScopeStaticImageMethod_DeleteAllItemFrames
	{
		MYGUIEXPORT void MYGUICALL ExportStaticImage_DeleteAllItemFrames_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::StaticImage * >(_native)->deleteAllItemFrames(
				Convert<size_t>::From( _index ) );
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
