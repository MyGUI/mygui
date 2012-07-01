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

		public ref class ScrollBar : public Widget
		{

		private:
			typedef MyGUI::ScrollBar ThisType;

		public:
			ScrollBar() : Widget() { }

		internal:
			ScrollBar( MyGUI::ScrollBar* _native ) : Widget(_native) { }
			ScrollBar( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew ScrollBar(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

	public:
		delegate void HandleScrollChangePosition(
			Convert<MyGUI::ScrollBar *>::Type _sender ,
			Convert<size_t>::Type _position );
		event HandleScrollChangePosition^ EventScrollChangePosition
		{
			void add(HandleScrollChangePosition^ _value)
			{
				bool empty = mDelegateScrollChangePosition == nullptr;

				mDelegateScrollChangePosition += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventScrollChangePosition +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ScrollBar * ,
							size_t > *>(
								new Delegate2< HandleScrollChangePosition^ ,
								MyGUI::ScrollBar * ,
								size_t >(mDelegateScrollChangePosition) );
			}
			void remove(HandleScrollChangePosition^ _value)
			{
				mDelegateScrollChangePosition -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateScrollChangePosition == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventScrollChangePosition -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ScrollBar * ,
							size_t > *>(
								new Delegate2< HandleScrollChangePosition^ ,
									MyGUI::ScrollBar * ,
									size_t >(mDelegateScrollChangePosition) );
			}
		}
	private:
		HandleScrollChangePosition^ mDelegateScrollChangePosition;
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
		property Convert<bool>::Type MoveToClick
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getMoveToClick() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMoveToClick( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<int>::Type MinTrackSize
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getMinTrackSize() );
			}
			void set(Convert<int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMinTrackSize( Convert<int>::From(_value) );
			}
		}
	
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
		property Convert<int>::Type LineSize
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getLineSize() );
			}
		}
	
	public:
		property Convert<size_t>::Type ScrollViewPage
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getScrollViewPage() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setScrollViewPage( Convert<size_t>::From(_value) );
			}
		}
	
	public:
		property Convert<size_t>::Type ScrollPage
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getScrollPage() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setScrollPage( Convert<size_t>::From(_value) );
			}
		}
	
	public:
		property Convert<size_t>::Type ScrollPosition
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getScrollPosition() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setScrollPosition( Convert<size_t>::From(_value) );
			}
		}
	
	public:
		property Convert<size_t>::Type ScrollRange
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getScrollRange() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setScrollRange( Convert<size_t>::From(_value) );
			}
		}
	
	public:
		property Convert<bool>::Type VerticalAlignment
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getVerticalAlignment() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVerticalAlignment( Convert<bool>::From(_value) );
			}
		}
	
		};

	} // namespace Managed
} // namespace MyGUI
