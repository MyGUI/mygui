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

	public ref class Window : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( Window, Widget );

		//InsertPoint

   	public:
		delegate void HandleWindowChangeCoord( ConvertToType<MyGUI::Widget *>::Type _value1 );
		event HandleWindowChangeCoord^ WindowChangeCoord
		{
			void add(HandleWindowChangeCoord^ _value)
			{
				mDelegateWindowChangeCoord += _value;
				static_cast<ThisType*>(mNative)->eventWindowChangeCoord = new Delegate1< HandleWindowChangeCoord^, MyGUI::Widget * >(mDelegateWindowChangeCoord);
			}
			void remove(HandleWindowChangeCoord^ _value)
			{
				mDelegateWindowChangeCoord -= _value;
				if (mDelegateWindowChangeCoord == nullptr)
					static_cast<ThisType*>(mNative)->eventWindowChangeCoord = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventWindowChangeCoord = new Delegate1< HandleWindowChangeCoord^, MyGUI::Widget * >(mDelegateWindowChangeCoord);
			}
		}
	private:
		HandleWindowChangeCoord^ mDelegateWindowChangeCoord;




   	public:
		delegate void HandleWindowButtonPressed( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<const std::string &>::Type _value2 );
		event HandleWindowButtonPressed^ WindowButtonPressed
		{
			void add(HandleWindowButtonPressed^ _value)
			{
				mDelegateWindowButtonPressed += _value;
				static_cast<ThisType*>(mNative)->eventWindowButtonPressed = new Delegate2< HandleWindowButtonPressed^, MyGUI::Widget *, const std::string & >(mDelegateWindowButtonPressed);
			}
			void remove(HandleWindowButtonPressed^ _value)
			{
				mDelegateWindowButtonPressed -= _value;
				if (mDelegateWindowButtonPressed == nullptr)
					static_cast<ThisType*>(mNative)->eventWindowButtonPressed = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventWindowButtonPressed = new Delegate2< HandleWindowButtonPressed^, MyGUI::Widget *, const std::string & >(mDelegateWindowButtonPressed);
			}
		}
	private:
		HandleWindowButtonPressed^ mDelegateWindowButtonPressed;




   	public:
		property ConvertToType< unsigned short >::Type FontHeight
		{
			ConvertToType< unsigned short >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType< unsigned short >::ConvertToValue( static_cast<ThisType*>(mNative)->getFontHeight() );
			}
			void set(ConvertToType< unsigned short >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setFontHeight( ConvertToType< unsigned short >::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<const Ogre::String &>::Type FontName
		{
			ConvertToType<const Ogre::String &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<const Ogre::String &>::ConvertToValue( static_cast<ThisType*>(mNative)->getFontName() );
			}
			void set(ConvertToType<const Ogre::String &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setFontName( ConvertToType<const Ogre::String &>::ConvertFromValue(_value) );
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
		property ConvertToType<bool>::Type Snap
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getSnap() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setSnap( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		void setPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void setCoord( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setSize( ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top) );
		}



   	public:
		void setCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void setSize( ConvertToType<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_size) );
		}



   	public:
		void setPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::Type _point )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::ConvertFromValue(_point) );
		}



   	public:
		ConvertToType< MyGUI::types::TSize< int > >::Type getMaxSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::types::TSize< int > >::ConvertToValue( static_cast<ThisType*>(mNative)->getMaxSize( ) );
		}



   	public:
		void setMaxSize( ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaxSize( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setMaxSize( ConvertToType<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaxSize( ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_size) );
		}



   	public:
		ConvertToType< MyGUI::types::TSize< int > >::Type getMinSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::types::TSize< int > >::ConvertToValue( static_cast<ThisType*>(mNative)->getMinSize( ) );
		}



   	public:
		void setMinSize( ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMinSize( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setMinSize( ConvertToType<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMinSize( ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_size) );
		}



   	public:
		void setMinMax( ConvertToType<int>::Type _min_h, ConvertToType<int>::Type _min_v, ConvertToType<int>::Type _max_h, ConvertToType<int>::Type _max_v )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMinMax( ConvertToType<int>::ConvertFromValue(_min_h), ConvertToType<int>::ConvertFromValue(_min_v), ConvertToType<int>::ConvertFromValue(_max_h), ConvertToType<int>::ConvertFromValue(_max_v) );
		}



   	public:
		property ConvertToType<const MyGUI::types::TRect< int > &>::Type MinMax
		{
			ConvertToType<const MyGUI::types::TRect< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<const MyGUI::types::TRect< int > &>::ConvertToValue( static_cast<ThisType*>(mNative)->getMinMax() );
			}
			void set(ConvertToType<const MyGUI::types::TRect< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMinMax( ConvertToType<const MyGUI::types::TRect< int > &>::ConvertFromValue(_value) );
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
		property ConvertToType<bool>::Type AutoAlpha
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getAutoAlpha() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAutoAlpha( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		void destroySmooth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->destroySmooth( );
		}



   	public:
		void hideSmooth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->hideSmooth( );
		}



   	public:
		void showSmooth( ConvertToType<bool>::Type _reset )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showSmooth( ConvertToType<bool>::ConvertFromValue(_reset) );
		}



   	public:
		ConvertToType<const std::string &>::Type getTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



   	public:
		ConvertToType<bool>::Type isType( ConvertToType<const std::type_info &>::Type t )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isType( ConvertToType<const std::type_info &>::ConvertFromValue(t) ) );
		}



	};

} // namespace MMyGUI
