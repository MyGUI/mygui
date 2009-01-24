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

	public ref class DDContainer : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( DDContainer, Widget );

		//InsertPoint

   	public:
		delegate void HandleInvalideContainer( Convert<MyGUI::Widget *>::Type _sender );
		event HandleInvalideContainer^ InvalideContainer
		{
			void add(HandleInvalideContainer^ _value)
			{
				mDelegateInvalideContainer += _value;
				static_cast<ThisType*>(mNative)->eventInvalideContainer = new Delegate1< HandleInvalideContainer^, MyGUI::Widget * >(mDelegateInvalideContainer);
			}
			void remove(HandleInvalideContainer^ _value)
			{
				mDelegateInvalideContainer -= _value;
				if (mDelegateInvalideContainer == nullptr)
					static_cast<ThisType*>(mNative)->eventInvalideContainer = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventInvalideContainer = new Delegate1< HandleInvalideContainer^, MyGUI::Widget * >(mDelegateInvalideContainer);
			}
		}
	private:
		HandleInvalideContainer^ mDelegateInvalideContainer;




   	public:
		delegate void HandleUpdateDropState( Convert<MyGUI::Widget *>::Type _sender, Convert<std::vector<  MyGUI::DropWidgetInfo  > &>::Type _items, Convert<const MyGUI::DropWidgetState &>::Type _state );
		event HandleUpdateDropState^ UpdateDropState
		{
			void add(HandleUpdateDropState^ _value)
			{
				mDelegateUpdateDropState += _value;
				static_cast<ThisType*>(mNative)->eventUpdateDropState = new Delegate3< HandleUpdateDropState^, MyGUI::Widget *, std::vector<  MyGUI::DropWidgetInfo  > &, const MyGUI::DropWidgetState & >(mDelegateUpdateDropState);
			}
			void remove(HandleUpdateDropState^ _value)
			{
				mDelegateUpdateDropState -= _value;
				if (mDelegateUpdateDropState == nullptr)
					static_cast<ThisType*>(mNative)->eventUpdateDropState = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventUpdateDropState = new Delegate3< HandleUpdateDropState^, MyGUI::Widget *, std::vector<  MyGUI::DropWidgetInfo  > &, const MyGUI::DropWidgetState & >(mDelegateUpdateDropState);
			}
		}
	private:
		HandleUpdateDropState^ mDelegateUpdateDropState;




   	public:
		delegate void HandleDropWidgetInfo( Convert<MyGUI::Widget *>::Type _sender, Convert<std::vector<  MyGUI::DropWidgetInfo  > &>::Type _items );
		event HandleDropWidgetInfo^ DropWidgetInfo
		{
			void add(HandleDropWidgetInfo^ _value)
			{
				mDelegateDropWidgetInfo += _value;
				static_cast<ThisType*>(mNative)->requestDropWidgetInfo = new Delegate2< HandleDropWidgetInfo^, MyGUI::Widget *, std::vector<  MyGUI::DropWidgetInfo  > & >(mDelegateDropWidgetInfo);
			}
			void remove(HandleDropWidgetInfo^ _value)
			{
				mDelegateDropWidgetInfo -= _value;
				if (mDelegateDropWidgetInfo == nullptr)
					static_cast<ThisType*>(mNative)->requestDropWidgetInfo = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestDropWidgetInfo = new Delegate2< HandleDropWidgetInfo^, MyGUI::Widget *, std::vector<  MyGUI::DropWidgetInfo  > & >(mDelegateDropWidgetInfo);
			}
		}
	private:
		HandleDropWidgetInfo^ mDelegateDropWidgetInfo;




   	public:
		delegate void HandleDropState( Convert<MyGUI::Widget *>::Type _sender, Convert<MyGUI::DropItemState>::Type _state );
		event HandleDropState^ DropState
		{
			void add(HandleDropState^ _value)
			{
				mDelegateDropState += _value;
				static_cast<ThisType*>(mNative)->eventDropState = new Delegate2< HandleDropState^, MyGUI::Widget *, MyGUI::DropItemState >(mDelegateDropState);
			}
			void remove(HandleDropState^ _value)
			{
				mDelegateDropState -= _value;
				if (mDelegateDropState == nullptr)
					static_cast<ThisType*>(mNative)->eventDropState = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventDropState = new Delegate2< HandleDropState^, MyGUI::Widget *, MyGUI::DropItemState >(mDelegateDropState);
			}
		}
	private:
		HandleDropState^ mDelegateDropState;






   	public:
		delegate void HandleRequestDrop( Convert<MyGUI::Widget*>::Type _sender, Convert<const MyGUI::ItemDropInfo&>::Type _info, Convert<bool&>::Type _result );
		event HandleRequestDrop^ RequestDrop
		{
			void add(HandleRequestDrop^ _value)
			{
				mDelegateRequestDrop += _value;
				static_cast<ThisType*>(mNative)->eventRequestDrop = new Delegate3< HandleRequestDrop^, MyGUI::Widget*, const MyGUI::ItemDropInfo&, bool& >(mDelegateRequestDrop);
			}
			void remove(HandleRequestDrop^ _value)
			{
				mDelegateRequestDrop -= _value;
				if (mDelegateRequestDrop == nullptr)
					static_cast<ThisType*>(mNative)->eventRequestDrop = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventRequestDrop = new Delegate3< HandleRequestDrop^, MyGUI::Widget*, const MyGUI::ItemDropInfo&, bool& >(mDelegateRequestDrop);
			}
		}
	private:
		HandleRequestDrop^ mDelegateRequestDrop;




   	public:
		delegate void HandleStartDrop( Convert<MyGUI::Widget*>::Type _sender, Convert<const MyGUI::ItemDropInfo&>::Type _info, Convert<bool&>::Type _result );
		event HandleStartDrop^ StartDrop
		{
			void add(HandleStartDrop^ _value)
			{
				mDelegateStartDrop += _value;
				static_cast<ThisType*>(mNative)->eventStartDrop = new Delegate3< HandleStartDrop^, MyGUI::Widget*, const MyGUI::ItemDropInfo&, bool& >(mDelegateStartDrop);
			}
			void remove(HandleStartDrop^ _value)
			{
				mDelegateStartDrop -= _value;
				if (mDelegateStartDrop == nullptr)
					static_cast<ThisType*>(mNative)->eventStartDrop = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventStartDrop = new Delegate3< HandleStartDrop^, MyGUI::Widget*, const MyGUI::ItemDropInfo&, bool& >(mDelegateStartDrop);
			}
		}
	private:
		HandleStartDrop^ mDelegateStartDrop;




   	public:
		property Convert<bool>::Type NeedDragDrop
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getNeedDragDrop() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedDragDrop( Convert<bool>::From(_value) );
			}
		}



	};

} // namespace MMyGUI
