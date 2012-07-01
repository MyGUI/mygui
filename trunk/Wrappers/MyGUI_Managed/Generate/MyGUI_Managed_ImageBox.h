/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI_Managed_Widget.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class ImageBox : public Widget
		{

		private:
			typedef MyGUI::ImageBox ThisType;

		public:
			ImageBox() : Widget() { }

		internal:
			ImageBox( MyGUI::ImageBox* _native ) : Widget(_native) { }
			ImageBox( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew ImageBox(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

	public:
		void SetItemName(
			Convert<const std::string &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemName(
				Convert<const std::string &>::From(_value) );
		}
	public:
		void SetItemGroup(
			Convert<const std::string &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemGroup(
				Convert<const std::string &>::From(_value) );
		}
	public:
		Convert<bool>::Type SetItemResource(
			Convert<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->setItemResource(
					Convert<const std::string &>::From(_name) ) );
		}
	public:
		Convert<float>::Type GetItemFrameRate(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<float>::To(
				static_cast<ThisType*>(mNative)->getItemFrameRate(
					Convert<size_t>::From(_index) ) );
		}
	public:
		void SetItemFrameRate(
			Convert<size_t>::Type _index ,
			Convert<float>::Type _rate )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemFrameRate(
				Convert<size_t>::From(_index) ,
				Convert<float>::From(_rate) );
		}
	public:
		void DeleteAllItemFrames(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteAllItemFrames(
				Convert<size_t>::From(_index) );
		}
	public:
		void DeleteItemFrame(
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteItemFrame(
				Convert<size_t>::From(_index) ,
				Convert<size_t>::From(_indexFrame) );
		}
	public:
		void SetItemFrame(
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame ,
			Convert<const MyGUI::types::TCoord < int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemFrame(
				Convert<size_t>::From(_index) ,
				Convert<size_t>::From(_indexFrame) ,
				Convert<const MyGUI::types::TCoord < int > &>::From(_item) );
		}
	public:
		void InsertItemFrameDublicate(
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame ,
			Convert<size_t>::Type _indexSourceFrame )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemFrameDublicate(
				Convert<size_t>::From(_index) ,
				Convert<size_t>::From(_indexFrame) ,
				Convert<size_t>::From(_indexSourceFrame) );
		}
	public:
		void AddItemFrameDublicate(
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexSourceFrame )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItemFrameDublicate(
				Convert<size_t>::From(_index) ,
				Convert<size_t>::From(_indexSourceFrame) );
		}
	public:
		void InsertItemFrame(
			Convert<size_t>::Type _index ,
			Convert<size_t>::Type _indexFrame ,
			Convert<const MyGUI::types::TCoord < int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemFrame(
				Convert<size_t>::From(_index) ,
				Convert<size_t>::From(_indexFrame) ,
				Convert<const MyGUI::types::TCoord < int > &>::From(_item) );
		}
	public:
		void AddItemFrame(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::types::TCoord < int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItemFrame(
				Convert<size_t>::From(_index) ,
				Convert<const MyGUI::types::TCoord < int > &>::From(_item) );
		}
	public:
		void DeleteAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteAllItems( );
		}
	public:
		void DeleteItem(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteItem(
				Convert<size_t>::From(_index) );
		}
	public:
		void SetItem(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::types::TCoord < int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItem(
				Convert<size_t>::From(_index) ,
				Convert<const MyGUI::types::TCoord < int > &>::From(_item) );
		}
	public:
		void AddItem(
			Convert<const MyGUI::types::TCoord < int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem(
				Convert<const MyGUI::types::TCoord < int > &>::From(_item) );
		}
	public:
		void InsertItem(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::types::TCoord < int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItem(
				Convert<size_t>::From(_index) ,
				Convert<const MyGUI::types::TCoord < int > &>::From(_item) );
		}
	public:
		void ResetItemSelect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->resetItemSelect( );
		}
	public:
		void SetImageTile(
			Convert<const MyGUI::types::TSize < int > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageTile(
				Convert<const MyGUI::types::TSize < int > &>::From(_value) );
		}
	public:
		void SetImageCoord(
			Convert<const MyGUI::types::TCoord < int > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageCoord(
				Convert<const MyGUI::types::TCoord < int > &>::From(_value) );
		}
	public:
		void SetImageRect(
			Convert<const MyGUI::types::TRect < int > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageRect(
				Convert<const MyGUI::types::TRect < int > &>::From(_value) );
		}
	public:
		void SetImageTexture(
			Convert<const std::string &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageTexture(
				Convert<const std::string &>::From(_value) );
		}
	public:
		void SetImageInfo(
			Convert<const std::string &>::Type _texture ,
			Convert<const MyGUI::types::TCoord < int > &>::Type _coord ,
			Convert<const MyGUI::types::TSize < int > &>::Type _tile )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageInfo(
				Convert<const std::string &>::From(_texture) ,
				Convert<const MyGUI::types::TCoord < int > &>::From(_coord) ,
				Convert<const MyGUI::types::TSize < int > &>::From(_tile) );
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
		property Convert<size_t>::Type ItemCount
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getItemCount() );
			}
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
	
		};

	} // namespace Managed
} // namespace MyGUI
