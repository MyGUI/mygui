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

	public ref class VScroll : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( VScroll, Widget );

		//InsertPoint

   	public:
		delegate void HandleScrollChangePosition( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<size_t>::Type _value2 );
		event HandleScrollChangePosition^ ScrollChangePosition
		{
			void add(HandleScrollChangePosition^ _value)
			{
				mDelegateScrollChangePosition += _value;
				static_cast<ThisType*>(mNative)->eventScrollChangePosition = new Delegate2< HandleScrollChangePosition^, MyGUI::Widget *, size_t >(mDelegateScrollChangePosition);
			}
			void remove(HandleScrollChangePosition^ _value)
			{
				mDelegateScrollChangePosition -= _value;
				if (mDelegateScrollChangePosition == nullptr)
					static_cast<ThisType*>(mNative)->eventScrollChangePosition = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventScrollChangePosition = new Delegate2< HandleScrollChangePosition^, MyGUI::Widget *, size_t >(mDelegateScrollChangePosition);
			}
		}
	private:
		HandleScrollChangePosition^ mDelegateScrollChangePosition;




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
		property ConvertToType<int>::Type MinTrackSize
		{
			ConvertToType<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getMinTrackSize() );
			}
			void set(ConvertToType<int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMinTrackSize( ConvertToType<int>::ConvertFromValue(_value) );
			}
		}



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
		property ConvertToType<size_t>::Type ScrollViewPage
		{
			ConvertToType<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getScrollViewPage() );
			}
			void set(ConvertToType<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setScrollViewPage( ConvertToType<size_t>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<size_t>::Type ScrollPage
		{
			ConvertToType<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getScrollPage() );
			}
			void set(ConvertToType<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setScrollPage( ConvertToType<size_t>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<size_t>::Type ScrollPosition
		{
			ConvertToType<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getScrollPosition() );
			}
			void set(ConvertToType<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setScrollPosition( ConvertToType<size_t>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<size_t>::Type ScrollRange
		{
			ConvertToType<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getScrollRange() );
			}
			void set(ConvertToType<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setScrollRange( ConvertToType<size_t>::ConvertFromValue(_value) );
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
