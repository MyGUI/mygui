/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_Widget.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class VScrollBar : public Widget
		{

		private:
			typedef MyGUI::VScroll ThisType;

		public:
			VScrollBar() : Widget() { }

		internal:
			VScrollBar( MyGUI::VScroll* _native ) : Widget(_native) { }
			VScrollBar( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew VScrollBar(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   	public:
		delegate void HandleScrollChangePosition(
			Convert<MyGUI::VScroll *>::Type _sender ,
			Convert<size_t>::Type _position );
		event HandleScrollChangePosition^ EventScrollChangePosition
		{
			void add(HandleScrollChangePosition^ _value)
			{
				mDelegateScrollChangePosition += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventScrollChangePosition =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::VScroll * ,
						size_t > *>(
							new Delegate2< HandleScrollChangePosition^ ,
							MyGUI::VScroll * ,
							size_t >(mDelegateScrollChangePosition) );
			}
			void remove(HandleScrollChangePosition^ _value)
			{
				mDelegateScrollChangePosition -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateScrollChangePosition == nullptr)
					static_cast<ThisType*>(mNative)->eventScrollChangePosition = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventScrollChangePosition =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::VScroll * ,
							size_t > *>(
								new Delegate2< HandleScrollChangePosition^ ,
									MyGUI::VScroll * ,
									size_t >(mDelegateScrollChangePosition) );
			}
		}
	private:
		HandleScrollChangePosition^ mDelegateScrollChangePosition;



   	public:
		void SetProperty(
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setProperty(
				Convert<const std::string &>::From(_key) ,
				Convert<const std::string &>::From(_value) );
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
		Convert<int>::Type GetLineSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getLineSize( ) );
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
	


   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
