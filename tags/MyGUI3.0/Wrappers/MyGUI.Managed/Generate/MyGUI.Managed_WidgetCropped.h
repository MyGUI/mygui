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
		Convert<int>::Type GetLeft( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getLeft( ) );
		}



   	public:
		property Convert<MyGUI::Align>::Type Align
		{
			Convert<MyGUI::Align>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::Align>::To( static_cast<ThisType*>(mNative)->getAlign() );
			}
			void set(Convert<MyGUI::Align>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAlign( Convert<MyGUI::Align>::From(_value) );
			}
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
