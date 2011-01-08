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
		Convert<int>::Type GetHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getHeight( ) );
		}



   	public:
		Convert<int>::Type GetWidth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getWidth( ) );
		}



   	public:
		Convert<int>::Type GetBottom( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getBottom( ) );
		}



   	public:
		Convert<int>::Type GetTop( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getTop( ) );
		}



   	public:
		Convert<int>::Type GetRight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getRight( ) );
		}



   	public:
		Convert<int>::Type GetLeft( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getLeft( ) );
		}



   	public:
		Convert<int>::Type GetAbsoluteTop( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getAbsoluteTop( ) );
		}



   	public:
		Convert<int>::Type GetAbsoluteLeft( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getAbsoluteLeft( ) );
		}



   	public:
		Convert<MyGUI::types::TCoord< int >>::Type GetAbsoluteCoord( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::types::TCoord< int >>::To(
				static_cast<ThisType*>(mNative)->getAbsoluteCoord( ) );
		}



   	public:
		Convert<MyGUI::types::TRect< int >>::Type GetAbsoluteRect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::types::TRect< int >>::To(
				static_cast<ThisType*>(mNative)->getAbsoluteRect( ) );
		}



   	public:
		Convert<const MyGUI::types::TPoint< int > &>::Type GetAbsolutePosition( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::types::TPoint< int > &>::To(
				static_cast<ThisType*>(mNative)->getAbsolutePosition( ) );
		}



   	public:
		property Convert<const MyGUI::types::TCoord< int > &>::Type Coord
		{
			Convert<const MyGUI::types::TCoord< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TCoord< int > &>::To( static_cast<ThisType*>(mNative)->getCoord() );
			}
			void set(Convert<const MyGUI::types::TCoord< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCoord( Convert<const MyGUI::types::TCoord< int > &>::From(_value) );
			}
		}
	


   	public:
		property Convert<MyGUI::types::TSize< int >>::Type Size
		{
			Convert<MyGUI::types::TSize< int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TSize< int >>::To( static_cast<ThisType*>(mNative)->getSize() );
			}
			void set(Convert<const MyGUI::types::TSize< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setSize( Convert<const MyGUI::types::TSize< int > &>::From(_value) );
			}
		}
	


   	public:
		property Convert<MyGUI::types::TPoint< int >>::Type Position
		{
			Convert<MyGUI::types::TPoint< int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TPoint< int >>::To( static_cast<ThisType*>(mNative)->getPosition() );
			}
			void set(Convert<const MyGUI::types::TPoint< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPosition( Convert<const MyGUI::types::TPoint< int > &>::From(_value) );
			}
		}
	


   


		};

	} // namespace Managed
} // namespace MyGUI
