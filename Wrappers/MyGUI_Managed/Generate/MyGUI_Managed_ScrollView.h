/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI_Managed_Widget.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class ScrollView : public Widget
		{

		private:
			typedef MyGUI::ScrollView ThisType;

		public:
			ScrollView() : Widget() { }

		internal:
			ScrollView( MyGUI::ScrollView* _native ) : Widget(_native) { }
			ScrollView( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew ScrollView(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

	public:
		void SetCanvasSize(
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCanvasSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	public:
		void SetCoord(
			Convert<int>::Type _left ,
			Convert<int>::Type _top ,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord(
				Convert<int>::From(_left) ,
				Convert<int>::From(_top) ,
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	public:
		void SetSize(
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	public:
		void SetPosition(
			Convert<int>::Type _left ,
			Convert<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition(
				Convert<int>::From(_left) ,
				Convert<int>::From(_top) );
		}
	public:
		property Convert<MyGUI::types::TPoint < int >>::Type ViewOffset
		{
			Convert<MyGUI::types::TPoint < int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TPoint < int >>::To( static_cast<ThisType*>(mNative)->getViewOffset() );
			}
			void set(Convert<const MyGUI::types::TPoint < int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setViewOffset( Convert<const MyGUI::types::TPoint < int > &>::From(_value) );
			}
		}
	
	public:
		property Convert<MyGUI::types::TCoord < int >>::Type ViewCoord
		{
			Convert<MyGUI::types::TCoord < int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TCoord < int >>::To( static_cast<ThisType*>(mNative)->getViewCoord() );
			}
		}
	
	public:
		property Convert<MyGUI::types::TSize < int >>::Type CanvasSize
		{
			Convert<MyGUI::types::TSize < int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TSize < int >>::To( static_cast<ThisType*>(mNative)->getCanvasSize() );
			}
			void set(Convert<const MyGUI::types::TSize < int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCanvasSize( Convert<const MyGUI::types::TSize < int > &>::From(_value) );
			}
		}
	
	public:
		property Convert<MyGUI::Align>::Type CanvasAlign
		{
			Convert<MyGUI::Align>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::Align>::To( static_cast<ThisType*>(mNative)->getCanvasAlign() );
			}
			void set(Convert<MyGUI::Align>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCanvasAlign( Convert<MyGUI::Align>::From(_value) );
			}
		}
	
	public:
		property Convert<bool>::Type IsVisibleHScroll
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isVisibleHScroll() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVisibleHScroll( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<bool>::Type IsVisibleVScroll
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isVisibleVScroll() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVisibleVScroll( Convert<bool>::From(_value) );
			}
		}
	
		};

	} // namespace Managed
} // namespace MyGUI
