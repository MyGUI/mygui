/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_Widget.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class StaticImage : public Widget
		{

		private:
			typedef MyGUI::StaticImage ThisType;

		public:
			StaticImage() : Widget() { }

		internal:
			StaticImage( MyGUI::StaticImage* _native ) : Widget(_native) { }
			StaticImage( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew StaticImage(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   


   


   


   


   	public:
		void SetItemName(
			Convert<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemName(
				Convert<const std::string &>::From(_name) );
		}



   	public:
		void SetItemGroup(
			Convert<const std::string &>::Type _group )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemGroup(
				Convert<const std::string &>::From(_group) );
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
		Convert<bool>::Type SetItemResource(
			Convert<const MyGUI::Guid &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->setItemResource(
					Convert<const MyGUI::Guid &>::From(_id) ) );
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
		void SetImageTile(
			Convert<const MyGUI::types::TSize< int > &>::Type _tile )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageTile(
				Convert<const MyGUI::types::TSize< int > &>::From(_tile) );
		}



   	public:
		void SetImageCoord(
			Convert<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageCoord(
				Convert<const MyGUI::types::TCoord< int > &>::From(_coord) );
		}



   


   	public:
		void SetImageTexture(
			Convert<const std::string &>::Type _texture )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageTexture(
				Convert<const std::string &>::From(_texture) );
		}



   	public:
		void SetImageInfo(
			Convert<const std::string &>::Type _texture ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _coord ,
			Convert<const MyGUI::types::TSize< int > &>::Type _tile )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageInfo(
				Convert<const std::string &>::From(_texture) ,
				Convert<const MyGUI::types::TCoord< int > &>::From(_coord) ,
				Convert<const MyGUI::types::TSize< int > &>::From(_tile) );
		}



   


   


		};

	} // namespace Managed
} // namespace MyGUI
