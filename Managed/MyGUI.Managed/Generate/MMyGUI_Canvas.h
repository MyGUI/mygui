/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"


namespace MyGUI
{
	namespace Managed
	{

		public ref class Canvas : public Widget
		{

			//--------------------------------------------------------------------
			// объявление типов и конструкторов
			MMYGUI_DECLARE_DERIVED( Canvas, Canvas, Widget );

			

			//InsertPoint





   	public:
		property bool TextureManaged
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isTextureManaged( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTextureManaged(_value);
			}
		}



   	public:
		Convert<bool>::Type IsTextureCreated( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->isTextureCreated( ) );
		}



   	public:
		Convert<bool>::Type IsTextureSrcSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->isTextureSrcSize( ) );
		}





   	public:
		void SetCoord( Convert<int>::Type _left, Convert<int>::Type _top, Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( Convert<int>::From(_left), Convert<int>::From(_top), Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		void SetSize( Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		void SetCoord( Convert<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( Convert<const MyGUI::types::TCoord< int > &>::From(_coord) );
		}



   	public:
		void SetSize( Convert<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( Convert<const MyGUI::types::TSize< int > &>::From(_size) );
		}



   	public:
		Convert<const std::string &>::Type GetTextureGroup( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getTextureGroup( ) );
		}



   	public:
		Convert<const std::string &>::Type GetTextureName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getTextureName( ) );
		}



   	public:
		Convert<Ogre::PixelFormat>::Type GetTextureFormat( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<Ogre::PixelFormat>::To( static_cast<ThisType*>(mNative)->getTextureFormat( ) );
		}



   	public:
		Convert< MyGUI::types::TSize< int > >::Type GetTextureSrcSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::types::TSize< int > >::To( static_cast<ThisType*>(mNative)->getTextureSrcSize( ) );
		}



   	public:
		Convert<int>::Type GetTextureSrcHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getTextureSrcHeight( ) );
		}



   	public:
		Convert<int>::Type GetTextureSrcWidth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getTextureSrcWidth( ) );
		}



   	public:
		Convert< MyGUI::types::TSize< int > >::Type GetTextureRealSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::types::TSize< int > >::To( static_cast<ThisType*>(mNative)->getTextureRealSize( ) );
		}



   	public:
		Convert<int>::Type GetTextureRealHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getTextureRealHeight( ) );
		}



   	public:
		Convert<int>::Type GetTextureRealWidth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getTextureRealWidth( ) );
		}



   	public:
		Convert<Ogre::ColourValue>::Type GetPixel( Convert<int>::Type _x, Convert<int>::Type _y )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<Ogre::ColourValue>::To( static_cast<ThisType*>(mNative)->getPixel( Convert<int>::From(_x), Convert<int>::From(_y) ) );
		}



   	public:
		Convert<Ogre::ColourValue>::Type GetPixel( Convert<const MyGUI::types::TPoint< int > &>::Type _pixel )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<Ogre::ColourValue>::To( static_cast<ThisType*>(mNative)->getPixel( Convert<const MyGUI::types::TPoint< int > &>::From(_pixel) ) );
		}



   	public:
		void SetPixel( Convert<const MyGUI::types::TPoint< int > &>::Type _pixel, Convert<const Ogre::ColourValue &>::Type value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPixel( Convert<const MyGUI::types::TPoint< int > &>::From(_pixel), Convert<const Ogre::ColourValue &>::From(value) );
		}



   	public:
		void SetPixel( Convert<int>::Type _x, Convert<int>::Type _y, Convert<const Ogre::ColourValue &>::Type value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPixel( Convert<int>::From(_x), Convert<int>::From(_y), Convert<const Ogre::ColourValue &>::From(value) );
		}



   	public:
		Convert<Ogre::HardwarePixelBufferSharedPtr>::Type GetBuffer( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<Ogre::HardwarePixelBufferSharedPtr>::To( static_cast<ThisType*>(mNative)->getBuffer( ) );
		}



   	public:
		Convert<bool>::Type IsLocked( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->isLocked( ) );
		}



   	public:
		void Unlock( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->unlock( );
		}



   	public:
		Convert<void *>::Type Lock( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<void *>::To( static_cast<ThisType*>(mNative)->lock( ) );
		}



   	public:
		void UpdateTexture( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->updateTexture( );
		}



   	public:
		void LoadTexture( Convert<Ogre::TexturePtr>::Type texture, Convert<bool>::Type manageMode )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->loadTexture( Convert<Ogre::TexturePtr>::From(texture), Convert<bool>::From(manageMode) );
		}


   	public:
		void LoadTexture( Convert<Ogre::TexturePtr>::Type texture )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->loadTexture( Convert<Ogre::TexturePtr>::From(texture) );
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
