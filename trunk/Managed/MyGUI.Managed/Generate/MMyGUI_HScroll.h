/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"
#include "MMyGUI_VScroll.h"

namespace MMyGUI
{

	public ref class HScroll : public VScroll
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( HScroll, VScroll );

		//InsertPoint

   	public:
		ConvertToType<int>::Type getTrackSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getTrackSize( ) );
		}



   	public:
		void setTrackSize( ConvertToType<size_t>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTrackSize( ConvertToType<size_t>::ConvertFromValue(_size) );
		}



   	public:
		ConvertToType<int>::Type getLineSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getLineSize( ) );
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
