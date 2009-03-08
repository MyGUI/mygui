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

		public ref class Canvas : public Widget
		{

		private:
			typedef MyGUI::Canvas ThisType;

		public:
			Canvas() : Widget() { }

		internal:
			Canvas( MyGUI::Canvas* _native ) : Widget(_native) { }
			Canvas( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew Canvas(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint





   	public:
		property Convert<bool>::Type TextureManaged
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isTextureManaged() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTextureManaged( Convert<bool>::From(_value) );
			}
		}
	


   	public:
		Convert<bool>::Type IsTextureCreated( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->isTextureCreated( ) );
		}



   	public:
		Convert<bool>::Type IsTextureSrcSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->isTextureSrcSize( ) );
		}





   


   


   


   


   	public:
		Convert<const std::string &>::Type GetTextureGroup( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To(
				static_cast<ThisType*>(mNative)->getTextureGroup( ) );
		}



   	public:
		Convert<const std::string &>::Type GetTextureName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To(
				static_cast<ThisType*>(mNative)->getTextureName( ) );
		}



   


   	public:
		Convert<MyGUI::types::TSize< int >>::Type GetTextureSrcSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::types::TSize< int >>::To(
				static_cast<ThisType*>(mNative)->getTextureSrcSize( ) );
		}



   	public:
		Convert<int>::Type GetTextureSrcHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getTextureSrcHeight( ) );
		}



   	public:
		Convert<int>::Type GetTextureSrcWidth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getTextureSrcWidth( ) );
		}



   	public:
		Convert<MyGUI::types::TSize< int >>::Type GetTextureRealSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::types::TSize< int >>::To(
				static_cast<ThisType*>(mNative)->getTextureRealSize( ) );
		}



   	public:
		Convert<int>::Type GetTextureRealHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getTextureRealHeight( ) );
		}



   	public:
		Convert<int>::Type GetTextureRealWidth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getTextureRealWidth( ) );
		}



   


   


   


   


   


   	public:
		Convert<bool>::Type IsLocked( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->isLocked( ) );
		}



   	public:
		void Unlock( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->unlock( );
		}



   


   	public:
		void UpdateTexture( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->updateTexture( );
		}



   


   	public:
		void DestroyTexture( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->destroyTexture( );
		}









   


   


		};

	} // namespace Managed
} // namespace MyGUI
