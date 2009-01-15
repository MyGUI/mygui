/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"


namespace MMyGUI
{

	public ref class StaticImage : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( StaticImage, Widget );

		//InsertPoint

   	public:
		void SetItemName( ConvertToType<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemName( ConvertToType<const std::string &>::ConvertFromValue(_name) );
		}



   	public:
		void SetItemGroup( ConvertToType<const std::string &>::Type _group )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemGroup( ConvertToType<const std::string &>::ConvertFromValue(_group) );
		}



   	public:
		void SetItemResource( ConvertToType< MyGUI::ResourceImageSet * >::Type _resource, ConvertToType<const std::string &>::Type _group, ConvertToType<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemResource( ConvertToType< MyGUI::ResourceImageSet * >::ConvertFromValue(_resource), ConvertToType<const std::string &>::ConvertFromValue(_group), ConvertToType<const std::string &>::ConvertFromValue(_name) );
		}



   	public:
		property ConvertToType< MyGUI::ResourceImageSet * >::Type ItemResource
		{
			ConvertToType< MyGUI::ResourceImageSet * >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType< MyGUI::ResourceImageSet * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemResource() );
			}
			void set(ConvertToType< MyGUI::ResourceImageSet * >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemResource( ConvertToType< MyGUI::ResourceImageSet * >::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType<bool>::Type SetItemResource( ConvertToType<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->setItemResource( ConvertToType<const std::string &>::ConvertFromValue(_name) ) );
		}



   	public:
		ConvertToType<bool>::Type SetItemResource( ConvertToType<const MyGUI::Guid &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->setItemResource( ConvertToType<const MyGUI::Guid &>::ConvertFromValue(_id) ) );
		}



   	public:
		void SetItemResourceInfo( ConvertToType<const MyGUI::ImageIndexInfo &>::Type _info )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemResourceInfo( ConvertToType<const MyGUI::ImageIndexInfo &>::ConvertFromValue(_info) );
		}



   	public:
		ConvertToType<float>::Type GetItemFrameRate( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<float>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemFrameRate( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void SetItemFrameRate( ConvertToType<size_t>::Type _index, ConvertToType<float>::Type _rate )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemFrameRate( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<float>::ConvertFromValue(_rate) );
		}



   	public:
		void DeleteAllItemsFrame( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteAllItemsFrame( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void DeleteItemFrame( ConvertToType<size_t>::Type _index, ConvertToType<size_t>::Type _indexFrame )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteItemFrame( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<size_t>::ConvertFromValue(_indexFrame) );
		}



   	public:
		void SetItemFrame( ConvertToType<size_t>::Type _index, ConvertToType<size_t>::Type _indexFrame, ConvertToType<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemFrame( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<size_t>::ConvertFromValue(_indexFrame), ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_item) );
		}



   	public:
		void InsertItemFrameDublicate( ConvertToType<size_t>::Type _index, ConvertToType<size_t>::Type _indexFrame, ConvertToType<size_t>::Type _indexSourceFrame )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemFrameDublicate( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<size_t>::ConvertFromValue(_indexFrame), ConvertToType<size_t>::ConvertFromValue(_indexSourceFrame) );
		}



   	public:
		void AddItemFrameDublicate( ConvertToType<size_t>::Type _index, ConvertToType<size_t>::Type _indexSourceFrame )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItemFrameDublicate( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<size_t>::ConvertFromValue(_indexSourceFrame) );
		}



   	public:
		void InsertItemFrame( ConvertToType<size_t>::Type _index, ConvertToType<size_t>::Type _indexFrame, ConvertToType<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemFrame( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<size_t>::ConvertFromValue(_indexFrame), ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_item) );
		}



   	public:
		void AddItemFrame( ConvertToType<size_t>::Type _index, ConvertToType<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItemFrame( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_item) );
		}



   	public:
		void DeleteAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteAllItems( );
		}



   	public:
		void DeleteItem( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteItem( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void SetItem( ConvertToType<size_t>::Type _index, ConvertToType<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItem( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_item) );
		}



   	public:
		void AddItem( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_item) );
		}



   	public:
		void InsertItem( ConvertToType<size_t>::Type _index, ConvertToType<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItem( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_item) );
		}



   	public:
		void ResetItemSelect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->resetItemSelect( );
		}



   	public:
		property ConvertToType<size_t>::Type ItemSelect
		{
			ConvertToType<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemSelect() );
			}
			void set(ConvertToType<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemSelect( ConvertToType<size_t>::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType<size_t>::Type GetItemCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemCount( ) );
		}



   	public:
		property ConvertToType<size_t>::Type ImageIndex
		{
			ConvertToType<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getImageIndex() );
			}
			void set(ConvertToType<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setImageIndex( ConvertToType<size_t>::ConvertFromValue(_value) );
			}
		}



   	public:
		void SetImageTile( ConvertToType<const MyGUI::types::TSize< int > &>::Type _tile )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageTile( ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_tile) );
		}



   	public:
		void SetImageCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void SetImageRect( ConvertToType<const MyGUI::types::TRect< int > &>::Type _rect )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageRect( ConvertToType<const MyGUI::types::TRect< int > &>::ConvertFromValue(_rect) );
		}



   	public:
		void SetImageTexture( ConvertToType<const std::string &>::Type _texture )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageTexture( ConvertToType<const std::string &>::ConvertFromValue(_texture) );
		}



   	public:
		void SetImageInfo( ConvertToType<const std::string &>::Type _texture, ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord, ConvertToType<const MyGUI::types::TSize< int > &>::Type _tile )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageInfo( ConvertToType<const std::string &>::ConvertFromValue(_texture), ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord), ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_tile) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
