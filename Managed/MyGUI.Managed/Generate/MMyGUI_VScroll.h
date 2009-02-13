/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"


MMYGUI_BEGIN_NAMESPACE

public ref class VScrollBar : public Widget
{

	//--------------------------------------------------------------------
	// объявление типов и конструкторов
	MMYGUI_DECLARE_DERIVED( VScrollBar, VScroll, Widget );

	

	//InsertPoint

   	public:
		delegate void HandleScrollChangePosition( Convert<MyGUI::VScroll *>::Type _sender, Convert<size_t>::Type _position );
		event HandleScrollChangePosition^ EventScrollChangePosition
		{
			void add(HandleScrollChangePosition^ _value)
			{
				mDelegateScrollChangePosition += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventScrollChangePosition =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::VScroll *, size_t > *>(
						new Delegate2< HandleScrollChangePosition^, MyGUI::VScroll *, size_t >(mDelegateScrollChangePosition) );
			}
			void remove(HandleScrollChangePosition^ _value)
			{
				mDelegateScrollChangePosition -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateScrollChangePosition == nullptr)
					static_cast<ThisType*>(mNative)->eventScrollChangePosition = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventScrollChangePosition =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::VScroll *, size_t > *>(
							new Delegate2< HandleScrollChangePosition^, MyGUI::VScroll *, size_t >(mDelegateScrollChangePosition) );
			}
		}
	private:
		HandleScrollChangePosition^ mDelegateScrollChangePosition;




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
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getLineSize( ) );
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

MMYGUI_END_NAMESPACE
