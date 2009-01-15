/*!
	@file
	@author		Generate utility by Albert Semenov
	@date
	@module
*/
#pragma once

#include "MMyGUI_Core.h"

namespace MMyGUI
{

	public ref class Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_BASE( Widget );

		//InsertPoint

   	public:
		property ConvertToType< MyGUI::WidgetStyle >::Type WidgetStyle
		{
			ConvertToType< MyGUI::WidgetStyle >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType< MyGUI::WidgetStyle >::ConvertToValue( static_cast<ThisType*>(mNative)->getWidgetStyle() );
			}
			void set(ConvertToType< MyGUI::WidgetStyle >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setWidgetStyle( ConvertToType< MyGUI::WidgetStyle >::ConvertFromValue(_value) );
			}
		}



   	public:
		void ChangeWidgetSkin( ConvertToType<const std::string &>::Type _skinname )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->changeWidgetSkin( ConvertToType<const std::string &>::ConvertFromValue(_skinname) );
		}



   	public:
		void AttachToWidget( ConvertToType< MyGUI::Widget * >::Type _widget )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->attachToWidget( ConvertToType< MyGUI::Widget * >::ConvertFromValue(_widget) );
		}



   	public:
		void DetachFromWidget( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->detachFromWidget( );
		}



   	public:
		void EnableToolTip( ConvertToType<bool>::Type _enable )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->enableToolTip( ConvertToType<bool>::ConvertFromValue(_enable) );
		}



   	public:
		ConvertToType<bool>::Type GetEnableToolTip( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getEnableToolTip( ) );
		}



   	public:
		property ConvertToType<bool>::Type NeedToolTip
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getNeedToolTip() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedToolTip( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType< MyGUI::ISubWidgetRect *>::Type GetSubWidgetMain( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::ISubWidgetRect *>::ConvertToValue( static_cast<ThisType*>(mNative)->getSubWidgetMain( ) );
		}



   	public:
		ConvertToType< MyGUI::ISubWidgetText *>::Type GetSubWidgetText( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::ISubWidgetText *>::ConvertToValue( static_cast<ThisType*>(mNative)->getSubWidgetText( ) );
		}



   	public:
		void GetContainer( ConvertToType< MyGUI::Widget * &>::Type _container, ConvertToType<size_t &>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->getContainer( ConvertToType< MyGUI::Widget * &>::ConvertFromValue(_container), ConvertToType<size_t &>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType< MyGUI::Widget * >::Type GetClientWidget( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::Widget * >::ConvertToValue( static_cast<ThisType*>(mNative)->getClientWidget( ) );
		}



   	public:
		ConvertToType< MyGUI::types::TCoord< int > >::Type GetClientCoord( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::types::TCoord< int > >::ConvertToValue( static_cast<ThisType*>(mNative)->getClientCoord( ) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetLayerName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getLayerName( ) );
		}



   	public:
		property ConvertToType<const std::string &>::Type Pointer
		{
			ConvertToType<const std::string &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getPointer() );
			}
			void set(ConvertToType<const std::string &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPointer( ConvertToType<const std::string &>::ConvertFromValue(_value) );
			}
		}



   	public:
		void SetEnabledSilent( ConvertToType<bool>::Type _enabled )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setEnabledSilent( ConvertToType<bool>::ConvertFromValue(_enabled) );
		}



   	public:
		property bool Enabled
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isEnabled( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEnabled(_value);
			}
		}



   	public:
		void SetMaskPeek( ConvertToType<const std::string &>::Type _filename )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaskPeek( ConvertToType<const std::string &>::ConvertFromValue(_filename) );
		}



   	public:
		property bool InheritsPeek
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isInheritsPeek( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setInheritsPeek(_value);
			}
		}



   	public:
		property bool NeedMouseFocus
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isNeedMouseFocus( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedMouseFocus(_value);
			}
		}



   	public:
		property bool NeedKeyFocus
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isNeedKeyFocus( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedKeyFocus(_value);
			}
		}



   	public:
		ConvertToType< MyGUI::Widget * >::Type FindWidget( ConvertToType<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::Widget * >::ConvertToValue( static_cast<ThisType*>(mNative)->findWidget( ConvertToType<const std::string &>::ConvertFromValue(_name) ) );
		}



   	public:
		ConvertToType< MyGUI::Enumerator<  std::vector<  MyGUI::Widget *  >  > >::Type GetEnumerator( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::Enumerator<  std::vector<  MyGUI::Widget *  >  > >::ConvertToValue( static_cast<ThisType*>(mNative)->getEnumerator( ) );
		}



   	public:
		ConvertToType< MyGUI::Widget * >::Type GetParent( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::Widget * >::ConvertToValue( static_cast<ThisType*>(mNative)->getParent( ) );
		}



   	public:
		ConvertToType<bool>::Type IsRootWidget( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isRootWidget( ) );
		}



   	public:
		ConvertToType<bool>::Type SetState( ConvertToType<const std::string &>::Type _state )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->setState( ConvertToType<const std::string &>::ConvertFromValue(_state) ) );
		}



   	public:
		property bool InheritsAlpha
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isInheritsAlpha( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setInheritsAlpha(_value);
			}
		}



   	public:
		property ConvertToType<float>::Type Alpha
		{
			ConvertToType<float>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<float>::ConvertToValue( static_cast<ThisType*>(mNative)->getAlpha() );
			}
			void set(ConvertToType<float>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAlpha( ConvertToType<float>::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType< MyGUI::types::TSize< int > >::Type GetTextSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::types::TSize< int > >::ConvertToValue( static_cast<ThisType*>(mNative)->getTextSize( ) );
		}



   	public:
		ConvertToType< MyGUI::types::TCoord< int > >::Type GetTextCoord( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::types::TCoord< int > >::ConvertToValue( static_cast<ThisType*>(mNative)->getTextCoord( ) );
		}



   	public:
		property ConvertToType< MyGUI::Align >::Type TextAlign
		{
			ConvertToType< MyGUI::Align >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType< MyGUI::Align >::ConvertToValue( static_cast<ThisType*>(mNative)->getTextAlign() );
			}
			void set(ConvertToType< MyGUI::Align >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTextAlign( ConvertToType< MyGUI::Align >::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType<Ogre::ushort>::Type GetFontHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<Ogre::ushort>::ConvertToValue( static_cast<ThisType*>(mNative)->getFontHeight( ) );
		}



   	public:
		void SetFontHeight( ConvertToType< unsigned short >::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setFontHeight( ConvertToType< unsigned short >::ConvertFromValue(_height) );
		}



   	public:
		property ConvertToType<const std::string &>::Type FontName
		{
			ConvertToType<const std::string &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getFontName() );
			}
			void set(ConvertToType<const std::string &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setFontName( ConvertToType<const std::string &>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<const MyGUI::Colour &>::Type Colour
		{
			ConvertToType<const MyGUI::Colour &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<const MyGUI::Colour &>::ConvertToValue( static_cast<ThisType*>(mNative)->getColour() );
			}
			void set(ConvertToType<const MyGUI::Colour &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setColour( ConvertToType<const MyGUI::Colour &>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<const Ogre::UTFString &>::Type Caption
		{
			ConvertToType<const Ogre::UTFString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getCaption() );
			}
			void set(ConvertToType<const Ogre::UTFString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCaption( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_value) );
			}
		}



   	public:
		void SetVisible( ConvertToType<bool>::Type _visible )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setVisible( ConvertToType<bool>::ConvertFromValue(_visible) );
		}



   	public:
		void SetRealCoord( ConvertToType<float>::Type _left, ConvertToType<float>::Type _top, ConvertToType<float>::Type _width, ConvertToType<float>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealCoord( ConvertToType<float>::ConvertFromValue(_left), ConvertToType<float>::ConvertFromValue(_top), ConvertToType<float>::ConvertFromValue(_width), ConvertToType<float>::ConvertFromValue(_height) );
		}



   	public:
		void SetRealSize( ConvertToType<float>::Type _width, ConvertToType<float>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealSize( ConvertToType<float>::ConvertFromValue(_width), ConvertToType<float>::ConvertFromValue(_height) );
		}



   	public:
		void SetRealPosition( ConvertToType<float>::Type _left, ConvertToType<float>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealPosition( ConvertToType<float>::ConvertFromValue(_left), ConvertToType<float>::ConvertFromValue(_top) );
		}



   	public:
		void SetRealCoord( ConvertToType<const MyGUI::types::TCoord< float > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealCoord( ConvertToType<const MyGUI::types::TCoord< float > &>::ConvertFromValue(_coord) );
		}



   	public:
		void SetRealSize( ConvertToType<const MyGUI::types::TSize< float > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealSize( ConvertToType<const MyGUI::types::TSize< float > &>::ConvertFromValue(_size) );
		}



   	public:
		void SetRealPosition( ConvertToType<const MyGUI::types::TPoint< float > &>::Type _point )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealPosition( ConvertToType<const MyGUI::types::TPoint< float > &>::ConvertFromValue(_point) );
		}



   	public:
		void SetPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void SetPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void SetCoord( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void SetSize( ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void SetPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top) );
		}



   	public:
		void SetCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void SetSize( ConvertToType<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_size) );
		}



   	public:
		void SetPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::Type _pos )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::ConvertFromValue(_pos) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getName( ) );
		}





   	public:
		ConvertToType< MyGUI::Widget * >::Type CreateWidgetRealT( ConvertToType<const std::string &>::Type _type, ConvertToType<const std::string &>::Type _skin, ConvertToType<const MyGUI::types::TCoord< float > &>::Type _coord, ConvertToType< MyGUI::Align >::Type _align, ConvertToType<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::Widget * >::ConvertToValue( static_cast<ThisType*>(mNative)->createWidgetRealT( ConvertToType<const std::string &>::ConvertFromValue(_type), ConvertToType<const std::string &>::ConvertFromValue(_skin), ConvertToType<const MyGUI::types::TCoord< float > &>::ConvertFromValue(_coord), ConvertToType< MyGUI::Align >::ConvertFromValue(_align), ConvertToType<const std::string &>::ConvertFromValue(_name) ) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
