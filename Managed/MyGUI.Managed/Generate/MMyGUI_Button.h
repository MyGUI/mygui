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

	public ref class Button : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( Button, Button, Widget );

		

		//InsertPoint

   	public:
		Convert< MyGUI::StaticImage * >::Type GetStaticImage( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::StaticImage * >::To( static_cast<ThisType*>(mNative)->getStaticImage( ) );
		}



   	public:
		property Convert<size_t>::Type ImageIndex
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getImageIndex() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setImageIndex( Convert<size_t>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type StateCheck
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getStateCheck() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setStateCheck( Convert<bool>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type ButtonPressed
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getButtonPressed() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setButtonPressed( Convert<bool>::From(_value) );
			}
		}



	};

} // namespace MMyGUI
