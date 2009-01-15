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
		delegate void HandleInvalideContainer( ConvertToType<MyGUI::Widget *>::Type _value1 );
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
		delegate void HandleUpdateDropState( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<std::vector<  MyGUI::DropWidgetInfo  > &>::Type _value2, ConvertToType<const MyGUI::DropWidgetState &>::Type _value3 );
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
		delegate void HandleDropWidgetInfo( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<std::vector<  MyGUI::DropWidgetInfo  > &>::Type _value2 );
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
		delegate void HandleDropState( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<MyGUI::DropItemState>::Type _value2 );
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
		void SetContainerItemInfo( ConvertToType<size_t>::Type _index, ConvertToType<bool>::Type _set, ConvertToType<bool>::Type _accept )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setContainerItemInfo( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<bool>::ConvertFromValue(_set), ConvertToType<bool>::ConvertFromValue(_accept) );
		}



   	public:
		property ConvertToType<bool>::Type NeedDragDrop
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getNeedDragDrop() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedDragDrop( ConvertToType<bool>::ConvertFromValue(_value) );
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
