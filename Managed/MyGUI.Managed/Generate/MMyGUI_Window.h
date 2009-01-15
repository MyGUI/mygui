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
		delegate void HandleWindowChangeCoord( Convert<MyGUI::Widget *>::Type _value1 );
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
		delegate void HandleWindowButtonPressed( Convert<MyGUI::Widget *>::Type _value1, Convert<const std::string &>::Type _value2 );
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
		property Convert< unsigned short >::Type FontHeight
		{
			Convert< unsigned short >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert< unsigned short >::To( static_cast<ThisType*>(mNative)->getFontHeight() );
			}
			void set(Convert< unsigned short >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setFontHeight( Convert< unsigned short >::From(_value) );
			}
		}



   	public:
		property Convert<const Ogre::String &>::Type FontName
		{
			Convert<const Ogre::String &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const Ogre::String &>::To( static_cast<ThisType*>(mNative)->getFontName() );
			}
			void set(Convert<const Ogre::String &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setFontName( Convert<const Ogre::String &>::From(_value) );
			}
		}



   	public:
		property Convert<const MyGUI::Colour &>::Type Colour
		{
			Convert<const MyGUI::Colour &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::Colour &>::To( static_cast<ThisType*>(mNative)->getColour() );
			}
			void set(Convert<const MyGUI::Colour &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setColour( Convert<const MyGUI::Colour &>::From(_value) );
			}
		}



   	public:
		property Convert< MyGUI::Align >::Type TextAlign
		{
			Convert< MyGUI::Align >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert< MyGUI::Align >::To( static_cast<ThisType*>(mNative)->getTextAlign() );
			}
			void set(Convert< MyGUI::Align >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTextAlign( Convert< MyGUI::Align >::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type Snap
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getSnap() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setSnap( Convert<bool>::From(_value) );
			}
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
		void SetPosition( Convert<int>::Type _left, Convert<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( Convert<int>::From(_left), Convert<int>::From(_top) );
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
		void SetPosition( Convert<const MyGUI::types::TPoint< int > &>::Type _point )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( Convert<const MyGUI::types::TPoint< int > &>::From(_point) );
		}



   	public:
		Convert< MyGUI::types::TSize< int > >::Type GetMaxSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::types::TSize< int > >::To( static_cast<ThisType*>(mNative)->getMaxSize( ) );
		}



   	public:
		void SetMaxSize( Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaxSize( Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		void SetMaxSize( Convert<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaxSize( Convert<const MyGUI::types::TSize< int > &>::From(_size) );
		}



   	public:
		Convert< MyGUI::types::TSize< int > >::Type GetMinSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::types::TSize< int > >::To( static_cast<ThisType*>(mNative)->getMinSize( ) );
		}



   	public:
		void SetMinSize( Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMinSize( Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		void SetMinSize( Convert<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMinSize( Convert<const MyGUI::types::TSize< int > &>::From(_size) );
		}



   	public:
		void SetMinMax( Convert<int>::Type _min_h, Convert<int>::Type _min_v, Convert<int>::Type _max_h, Convert<int>::Type _max_v )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMinMax( Convert<int>::From(_min_h), Convert<int>::From(_min_v), Convert<int>::From(_max_h), Convert<int>::From(_max_v) );
		}



   	public:
		property Convert<const MyGUI::types::TRect< int > &>::Type MinMax
		{
			Convert<const MyGUI::types::TRect< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TRect< int > &>::To( static_cast<ThisType*>(mNative)->getMinMax() );
			}
			void set(Convert<const MyGUI::types::TRect< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMinMax( Convert<const MyGUI::types::TRect< int > &>::From(_value) );
			}
		}



   	public:
		property Convert<const Ogre::UTFString &>::Type Caption
		{
			Convert<const Ogre::UTFString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const Ogre::UTFString &>::To( static_cast<ThisType*>(mNative)->getCaption() );
			}
			void set(Convert<const Ogre::UTFString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCaption( Convert<const Ogre::UTFString &>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type AutoAlpha
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getAutoAlpha() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAutoAlpha( Convert<bool>::From(_value) );
			}
		}



   	public:
		void DestroySmooth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->destroySmooth( );
		}



   	public:
		void HideSmooth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->hideSmooth( );
		}



   	public:
		void ShowSmooth( Convert<bool>::Type _reset )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showSmooth( Convert<bool>::From(_reset) );
		}



   	public:
		Convert<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
