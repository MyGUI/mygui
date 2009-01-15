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
		Convert<const MyGUI::types::TCoord< int > &>::Type GetClientCoord( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::types::TCoord< int > &>::To( static_cast<ThisType*>(mNative)->getClientCoord( ) );
		}



   	public:
		void SetCanvasSize( Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCanvasSize( Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		void SetCanvasSize( Convert<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCanvasSize( Convert<const MyGUI::types::TSize< int > &>::From(_size) );
		}



   	public:
		Convert< MyGUI::types::TSize< int > >::Type GetCanvasSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::types::TSize< int > >::To( static_cast<ThisType*>(mNative)->getCanvasSize( ) );
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
		Convert<bool>::Type IsShowHScroll( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->isShowHScroll( ) );
		}



   	public:
		void ShowHScroll( Convert<bool>::Type _show )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showHScroll( Convert<bool>::From(_show) );
		}



   	public:
		Convert<bool>::Type IsShowVScroll( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->isShowVScroll( ) );
		}



   	public:
		void ShowVScroll( Convert<bool>::Type _show )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showVScroll( Convert<bool>::From(_show) );
		}



   	public:
		void SetCoord( Convert<int>::Type _left, Convert<int>::Type _top, Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( Convert<int>::From(_left), Convert<int>::From(_top), Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		void SetSize( Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		void SetPosition( Convert<int>::Type _left, Convert<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( Convert<int>::From(_left), Convert<int>::From(_top) );
		}



   	public:
		void SetCoord( Convert<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( Convert<const MyGUI::types::TCoord< int > &>::From(_coord) );
		}



   	public:
		void SetSize( Convert<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( Convert<const MyGUI::types::TSize< int > &>::From(_size) );
		}



   	public:
		void SetPosition( Convert<const MyGUI::types::TPoint< int > &>::Type _point )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( Convert<const MyGUI::types::TPoint< int > &>::From(_point) );
		}



   	public:
		void SetTextAlign( Convert< MyGUI::Align >::Type _align )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextAlign( Convert< MyGUI::Align >::From(_align) );
		}



   	public:
		Convert<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
