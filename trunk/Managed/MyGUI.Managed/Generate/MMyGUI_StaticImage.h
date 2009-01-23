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
		void SetItemName( Convert<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemName( Convert<const std::string &>::From(_name) );
		}



   	public:
		void SetItemGroup( Convert<const std::string &>::Type _group )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemGroup( Convert<const std::string &>::From(_group) );
		}



   	public:
		void SetItemResource( Convert< MyGUI::ResourceImageSet * >::Type _resource, Convert<const std::string &>::Type _group, Convert<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemResource( Convert< MyGUI::ResourceImageSet * >::From(_resource), Convert<const std::string &>::From(_group), Convert<const std::string &>::From(_name) );
		}



   	public:
		property Convert< MyGUI::ResourceImageSet * >::Type ItemResource
		{
			Convert< MyGUI::ResourceImageSet * >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert< MyGUI::ResourceImageSet * >::To( static_cast<ThisType*>(mNative)->getItemResource() );
			}
			void set(Convert< MyGUI::ResourceImageSet * >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemResource( Convert< MyGUI::ResourceImageSet * >::From(_value) );
			}
		}



   	public:
		Convert<bool>::Type SetItemResource( Convert<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->setItemResource( Convert<const std::string &>::From(_name) ) );
		}



   	public:
		Convert<bool>::Type SetItemResource( Convert<const MyGUI::Guid &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->setItemResource( Convert<const MyGUI::Guid &>::From(_id) ) );
		}



   	public:
		void SetItemResourceInfo( Convert<const MyGUI::ImageIndexInfo &>::Type _info )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemResourceInfo( Convert<const MyGUI::ImageIndexInfo &>::From(_info) );
		}



   	public:
		Convert<float>::Type GetItemFrameRate( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<float>::To( static_cast<ThisType*>(mNative)->getItemFrameRate( Convert<size_t>::From(_index) ) );
		}



   	public:
		void SetItemFrameRate( Convert<size_t>::Type _index, Convert<float>::Type _rate )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemFrameRate( Convert<size_t>::From(_index), Convert<float>::From(_rate) );
		}



   	public:
		void DeleteAllItemsFrame( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteAllItemsFrame( Convert<size_t>::From(_index) );
		}



   	public:
		void DeleteItemFrame( Convert<size_t>::Type _index, Convert<size_t>::Type _indexFrame )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteItemFrame( Convert<size_t>::From(_index), Convert<size_t>::From(_indexFrame) );
		}



   	public:
		void SetItemFrame( Convert<size_t>::Type _index, Convert<size_t>::Type _indexFrame, Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemFrame( Convert<size_t>::From(_index), Convert<size_t>::From(_indexFrame), Convert<const MyGUI::types::TCoord< int > &>::From(_item) );
		}



   	public:
		void InsertItemFrameDublicate( Convert<size_t>::Type _index, Convert<size_t>::Type _indexFrame, Convert<size_t>::Type _indexSourceFrame )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemFrameDublicate( Convert<size_t>::From(_index), Convert<size_t>::From(_indexFrame), Convert<size_t>::From(_indexSourceFrame) );
		}



   	public:
		void AddItemFrameDublicate( Convert<size_t>::Type _index, Convert<size_t>::Type _indexSourceFrame )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItemFrameDublicate( Convert<size_t>::From(_index), Convert<size_t>::From(_indexSourceFrame) );
		}



   	public:
		void InsertItemFrame( Convert<size_t>::Type _index, Convert<size_t>::Type _indexFrame, Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemFrame( Convert<size_t>::From(_index), Convert<size_t>::From(_indexFrame), Convert<const MyGUI::types::TCoord< int > &>::From(_item) );
		}



   	public:
		void AddItemFrame( Convert<size_t>::Type _index, Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItemFrame( Convert<size_t>::From(_index), Convert<const MyGUI::types::TCoord< int > &>::From(_item) );
		}



   	public:
		void DeleteAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteAllItems( );
		}



   	public:
		void DeleteItem( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteItem( Convert<size_t>::From(_index) );
		}



   	public:
		void SetItem( Convert<size_t>::Type _index, Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItem( Convert<size_t>::From(_index), Convert<const MyGUI::types::TCoord< int > &>::From(_item) );
		}



   	public:
		void AddItem( Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem( Convert<const MyGUI::types::TCoord< int > &>::From(_item) );
		}



   	public:
		void InsertItem( Convert<size_t>::Type _index, Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItem( Convert<size_t>::From(_index), Convert<const MyGUI::types::TCoord< int > &>::From(_item) );
		}



   	public:
		void ResetItemSelect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->resetItemSelect( );
		}



   	public:
		property Convert<size_t>::Type ItemSelect
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getItemSelect() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemSelect( Convert<size_t>::From(_value) );
			}
		}



   	public:
		Convert<size_t>::Type GetItemCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getItemCount( ) );
		}



   	public:
		property Convert<size_t>::Type ImageIndex
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getImageIndex() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setImageIndex( Convert<size_t>::From(_value) );
			}
		}



   	public:
		void SetImageTile( Convert<const MyGUI::types::TSize< int > &>::Type _tile )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageTile( Convert<const MyGUI::types::TSize< int > &>::From(_tile) );
		}



   	public:
		void SetImageCoord( Convert<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageCoord( Convert<const MyGUI::types::TCoord< int > &>::From(_coord) );
		}



   	public:
		void SetImageRect( Convert<const MyGUI::types::TRect< int > &>::Type _rect )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageRect( Convert<const MyGUI::types::TRect< int > &>::From(_rect) );
		}



   	public:
		void SetImageTexture( Convert<const std::string &>::Type _texture )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageTexture( Convert<const std::string &>::From(_texture) );
		}



   	public:
		void SetImageInfo( Convert<const std::string &>::Type _texture, Convert<const MyGUI::types::TCoord< int > &>::Type _coord, Convert<const MyGUI::types::TSize< int > &>::Type _tile )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageInfo( Convert<const std::string &>::From(_texture), Convert<const MyGUI::types::TCoord< int > &>::From(_coord), Convert<const MyGUI::types::TSize< int > &>::From(_tile) );
		}



	};

} // namespace MMyGUI
