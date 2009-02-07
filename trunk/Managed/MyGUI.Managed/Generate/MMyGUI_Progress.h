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

	public ref class ProgressBar : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( ProgressBar, Progress, Widget );

		

		//InsertPoint

   	public:
		property Convert< MyGUI::Align >::Type ProgressStartPoint
		{
			Convert< MyGUI::Align >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert< MyGUI::Align >::To( static_cast<ThisType*>(mNative)->getProgressStartPoint() );
			}
			void set(Convert< MyGUI::Align >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setProgressStartPoint( Convert< MyGUI::Align >::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type ProgressAutoTrack
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getProgressAutoTrack() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setProgressAutoTrack( Convert<bool>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type ProgressFillTrack
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getProgressFillTrack() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setProgressFillTrack( Convert<bool>::From(_value) );
			}
		}



   	public:
		property Convert<size_t>::Type ProgressPosition
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getProgressPosition() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setProgressPosition( Convert<size_t>::From(_value) );
			}
		}



   	public:
		property Convert<size_t>::Type ProgressRange
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getProgressRange() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setProgressRange( Convert<size_t>::From(_value) );
			}
		}



	};

} // namespace MMyGUI
