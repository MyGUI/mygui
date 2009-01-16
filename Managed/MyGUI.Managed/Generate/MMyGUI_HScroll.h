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
		property Convert<int>::Type TrackSize
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getTrackSize() );
			}
			void set(Convert<int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTrackSize( Convert<int>::From(_value) );
			}
		}



   	public:
		Convert<int>::Type GetLineSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getLineSize( ) );
		}



	};

} // namespace MMyGUI
