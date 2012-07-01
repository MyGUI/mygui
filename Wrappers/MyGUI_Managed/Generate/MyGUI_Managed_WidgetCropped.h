/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "../BaseWidget.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class WidgetCropped abstract : public BaseWidget
		{

		private:
			typedef MyGUI::Widget ThisType;

		public:
			WidgetCropped() : BaseWidget() { }

		internal:
			WidgetCropped( MyGUI::Widget* _native ) : BaseWidget(_native) { }

			//InsertPoint

	public:
		property Convert<int>::Type Height
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getHeight() );
			}
		}
	
	public:
		property Convert<int>::Type Width
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getWidth() );
			}
		}
	
	public:
		property Convert<int>::Type Bottom
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getBottom() );
			}
		}
	
	public:
		property Convert<int>::Type Top
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getTop() );
			}
		}
	
	public:
		property Convert<int>::Type Right
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getRight() );
			}
		}
	
	public:
		property Convert<int>::Type Left
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getLeft() );
			}
		}
	
	public:
		property Convert<int>::Type AbsoluteTop
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getAbsoluteTop() );
			}
		}
	
	public:
		property Convert<int>::Type AbsoluteLeft
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getAbsoluteLeft() );
			}
		}
	
	public:
		property Convert<MyGUI::types::TCoord < int >>::Type AbsoluteCoord
		{
			Convert<MyGUI::types::TCoord < int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TCoord < int >>::To( static_cast<ThisType*>(mNative)->getAbsoluteCoord() );
			}
		}
	
	public:
		property Convert<MyGUI::types::TRect < int >>::Type AbsoluteRect
		{
			Convert<MyGUI::types::TRect < int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TRect < int >>::To( static_cast<ThisType*>(mNative)->getAbsoluteRect() );
			}
		}
	
	public:
		property Convert<const MyGUI::types::TPoint < int > &>::Type AbsolutePosition
		{
			Convert<const MyGUI::types::TPoint < int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TPoint < int > &>::To( static_cast<ThisType*>(mNative)->getAbsolutePosition() );
			}
		}
	
	public:
		property Convert<const MyGUI::types::TCoord < int > &>::Type Coord
		{
			Convert<const MyGUI::types::TCoord < int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TCoord < int > &>::To( static_cast<ThisType*>(mNative)->getCoord() );
			}
			void set(Convert<const MyGUI::types::TCoord < int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCoord( Convert<const MyGUI::types::TCoord < int > &>::From(_value) );
			}
		}
	
	public:
		property Convert<MyGUI::types::TSize < int >>::Type Size
		{
			Convert<MyGUI::types::TSize < int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TSize < int >>::To( static_cast<ThisType*>(mNative)->getSize() );
			}
			void set(Convert<const MyGUI::types::TSize < int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setSize( Convert<const MyGUI::types::TSize < int > &>::From(_value) );
			}
		}
	
	public:
		property Convert<MyGUI::types::TPoint < int >>::Type Position
		{
			Convert<MyGUI::types::TPoint < int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TPoint < int >>::To( static_cast<ThisType*>(mNative)->getPosition() );
			}
			void set(Convert<const MyGUI::types::TPoint < int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPosition( Convert<const MyGUI::types::TPoint < int > &>::From(_value) );
			}
		}
	
		};

	} // namespace Managed
} // namespace MyGUI
