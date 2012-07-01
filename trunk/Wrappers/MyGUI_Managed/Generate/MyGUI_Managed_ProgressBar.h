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

		public ref class ProgressBar : public Widget
		{

		private:
			typedef MyGUI::ProgressBar ThisType;

		public:
			ProgressBar() : Widget() { }

		internal:
			ProgressBar( MyGUI::ProgressBar* _native ) : Widget(_native) { }
			ProgressBar( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew ProgressBar(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

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
		property Convert<MyGUI::FlowDirection>::Type FlowDirection
		{
			Convert<MyGUI::FlowDirection>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::FlowDirection>::To( static_cast<ThisType*>(mNative)->getFlowDirection() );
			}
			void set(Convert<MyGUI::FlowDirection>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setFlowDirection( Convert<MyGUI::FlowDirection>::From(_value) );
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

	} // namespace Managed
} // namespace MyGUI
