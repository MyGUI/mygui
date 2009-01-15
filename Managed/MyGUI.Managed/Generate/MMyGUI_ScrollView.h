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
		ConvertToType<const MyGUI::types::TCoord< int > &>::Type GetClientCoord( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertToValue( static_cast<ThisType*>(mNative)->getClientCoord( ) );
		}



   	public:
		void SetCanvasSize( ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCanvasSize( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void SetCanvasSize( ConvertToType<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCanvasSize( ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_size) );
		}



   	public:
		ConvertToType< MyGUI::types::TSize< int > >::Type GetCanvasSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::types::TSize< int > >::ConvertToValue( static_cast<ThisType*>(mNative)->getCanvasSize( ) );
		}



   	public:
		property ConvertToType< MyGUI::Align >::Type CanvasAlign
		{
			ConvertToType< MyGUI::Align >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType< MyGUI::Align >::ConvertToValue( static_cast<ThisType*>(mNative)->getCanvasAlign() );
			}
			void set(ConvertToType< MyGUI::Align >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCanvasAlign( ConvertToType< MyGUI::Align >::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType<bool>::Type IsShowHScroll( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isShowHScroll( ) );
		}



   	public:
		void ShowHScroll( ConvertToType<bool>::Type _show )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showHScroll( ConvertToType<bool>::ConvertFromValue(_show) );
		}



   	public:
		ConvertToType<bool>::Type IsShowVScroll( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isShowVScroll( ) );
		}



   	public:
		void ShowVScroll( ConvertToType<bool>::Type _show )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showVScroll( ConvertToType<bool>::ConvertFromValue(_show) );
		}



   	public:
		void SetPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void SetPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void SetCoord( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void SetSize( ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void SetPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top) );
		}



   	public:
		void SetCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void SetSize( ConvertToType<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_size) );
		}



   	public:
		void SetPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::Type _point )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::ConvertFromValue(_point) );
		}



   	public:
		void SetTextAlign( ConvertToType< MyGUI::Align >::Type _align )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextAlign( ConvertToType< MyGUI::Align >::ConvertFromValue(_align) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
