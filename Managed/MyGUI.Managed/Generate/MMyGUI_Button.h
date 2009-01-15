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
		MMYGUI_DECLARE_DERIVED( Button, Widget );

		//InsertPoint

   	public:
		void SetEnabled( ConvertToType<bool>::Type _enabled )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setEnabled( ConvertToType<bool>::ConvertFromValue(_enabled) );
		}



   	public:
		ConvertToType< MyGUI::StaticImage * >::Type GetStaticImage( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::StaticImage * >::ConvertToValue( static_cast<ThisType*>(mNative)->getStaticImage( ) );
		}



   	public:
		property ConvertToType<size_t>::Type ImageIndex
		{
			ConvertToType<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getImageIndex() );
			}
			void set(ConvertToType<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setImageIndex( ConvertToType<size_t>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<bool>::Type StateCheck
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getStateCheck() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setStateCheck( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<bool>::Type ButtonPressed
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getButtonPressed() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setButtonPressed( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
