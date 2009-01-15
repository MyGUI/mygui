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
		ConvertToType<int>::Type GetTrackSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getTrackSize( ) );
		}



   	public:
		void SetTrackSize( ConvertToType<size_t>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTrackSize( ConvertToType<size_t>::ConvertFromValue(_size) );
		}



   	public:
		ConvertToType<int>::Type GetLineSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getLineSize( ) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
