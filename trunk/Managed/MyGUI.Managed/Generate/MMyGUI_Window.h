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
		void SetMaxSize( Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaxSize( Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		property Convert<const MyGUI::types::TSize< int > &>::Type MaxSize
		{
			Convert<const MyGUI::types::TSize< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TSize< int > &>::To( static_cast<ThisType*>(mNative)->getMaxSize() );
			}
			void set(Convert<const MyGUI::types::TSize< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMaxSize( Convert<const MyGUI::types::TSize< int > &>::From(_value) );
			}
		}



   	public:
		void SetMinSize( Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMinSize( Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		property Convert<const MyGUI::types::TSize< int > &>::Type MinSize
		{
			Convert<const MyGUI::types::TSize< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TSize< int > &>::To( static_cast<ThisType*>(mNative)->getMinSize() );
			}
			void set(Convert<const MyGUI::types::TSize< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMinSize( Convert<const MyGUI::types::TSize< int > &>::From(_value) );
			}
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
		void ShowSmooth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showSmooth( );
		}



	};

} // namespace MMyGUI
