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
		void SetProperty(
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setProperty(
				Convert<const std::string &>::From(_key) ,
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
			Convert<const MyGUI::Guid &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->setItemResource(
					Convert<const MyGUI::Guid &>::From(_id) ) );
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
			Convert<const MyGUI::types::TCoord< int > &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemFrame(
				Convert<size_t>::From(_index) ,
				Convert<size_t>::From(_indexFrame) ,
				Convert<const MyGUI::types::TCoord< int > &>::From(_item) );
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
		void DeleteAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteAllItems( );
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
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getItemCount( ) );
		}



   	public:
		void SetImageRect(
			Convert<const MyGUI::types::TRect< int > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setImageRect(
				Convert<const MyGUI::types::TRect< int > &>::From(_value) );
		}



		};

	} // namespace Managed
} // namespace MyGUI
