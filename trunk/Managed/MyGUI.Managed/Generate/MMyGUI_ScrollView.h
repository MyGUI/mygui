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

	public ref class ScrollView : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( ScrollView, Widget );

		//InsertPoint

   	public:
		void SetCanvasSize( Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCanvasSize( Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		property Convert<const MyGUI::types::TSize< int > &>::Type CanvasSize
		{
			Convert<const MyGUI::types::TSize< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TSize< int > &>::To( static_cast<ThisType*>(mNative)->getCanvasSize() );
			}
			void set(Convert<const MyGUI::types::TSize< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCanvasSize( Convert<const MyGUI::types::TSize< int > &>::From(_value) );
			}
		}



   	public:
		property Convert< MyGUI::Align >::Type CanvasAlign
		{
			Convert< MyGUI::Align >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert< MyGUI::Align >::To( static_cast<ThisType*>(mNative)->getCanvasAlign() );
			}
			void set(Convert< MyGUI::Align >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCanvasAlign( Convert< MyGUI::Align >::From(_value) );
			}
		}



   	public:
		property bool VisibleHScroll
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isVisibleHScroll( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVisibleHScroll(_value);
			}
		}



   	public:
		property bool VisibleVScroll
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isVisibleVScroll( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVisibleVScroll(_value);
			}
		}



	};

} // namespace MMyGUI
