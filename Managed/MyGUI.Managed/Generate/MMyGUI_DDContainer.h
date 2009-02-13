/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"


MMYGUI_BEGIN_NAMESPACE

public ref class DDContainer : public Widget
{

	//--------------------------------------------------------------------
	// объявление типов и конструкторов
	MMYGUI_DECLARE_DERIVED( DDContainer, DDContainer, Widget );

	

	//InsertPoint



   	public:
		delegate void HandleUpdateDropState( Convert<MyGUI::DDContainer *>::Type _sender, Convert<MyGUI::Widget *>::Type _items, Convert<const MyGUI::DropWidgetState &>::Type _state );
		event HandleUpdateDropState^ EventUpdateDropState
		{
			void add(HandleUpdateDropState^ _value)
			{
				mDelegateUpdateDropState += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventUpdateDropState =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, MyGUI::Widget *, const MyGUI::DropWidgetState & > *>(
						new Delegate3< HandleUpdateDropState^, MyGUI::DDContainer *, MyGUI::Widget *, const MyGUI::DropWidgetState & >(mDelegateUpdateDropState) );
			}
			void remove(HandleUpdateDropState^ _value)
			{
				mDelegateUpdateDropState -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateUpdateDropState == nullptr)
					static_cast<ThisType*>(mNative)->eventUpdateDropState = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventUpdateDropState =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, MyGUI::Widget *, const MyGUI::DropWidgetState & > *>(
							new Delegate3< HandleUpdateDropState^, MyGUI::DDContainer *, MyGUI::Widget *, const MyGUI::DropWidgetState & >(mDelegateUpdateDropState) );
			}
		}
	private:
		HandleUpdateDropState^ mDelegateUpdateDropState;




	public:
		delegate void HandleDropWidgetInfo( Convert<MyGUI::DDContainer *>::Type _sender, Convert<MyGUI::Widget * &>::Type _items, Convert<MyGUI::types::TCoord< int > &>::Type _dimension );
		event HandleDropWidgetInfo^ RequestDropWidgetInfo
		{
			void add(HandleDropWidgetInfo^ _value)
			{
				mDelegateDropWidgetInfo += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->requestDropWidgetInfo =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, MyGUI::Widget * &, MyGUI::types::TCoord< int > & > *>(
						new Delegate3< HandleDropWidgetInfo^, MyGUI::DDContainer *, MyGUI::Widget * &, MyGUI::types::TCoord< int > & >(mDelegateDropWidgetInfo) );
			}
			void remove(HandleDropWidgetInfo^ _value)
			{
				mDelegateDropWidgetInfo -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateDropWidgetInfo == nullptr)
					static_cast<ThisType*>(mNative)->requestDropWidgetInfo = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestDropWidgetInfo =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, MyGUI::Widget * &, MyGUI::types::TCoord< int > & > *>(
							new Delegate3< HandleDropWidgetInfo^, MyGUI::DDContainer *, MyGUI::Widget * &, MyGUI::types::TCoord< int > & >(mDelegateDropWidgetInfo) );
			}
		}
	private:
		HandleDropWidgetInfo^ mDelegateDropWidgetInfo;




   	public:
		delegate void HandleDropState( Convert<MyGUI::DDContainer *>::Type _sender, Convert<MyGUI::DropItemState>::Type _state );
		event HandleDropState^ EventDropState
		{
			void add(HandleDropState^ _value)
			{
				mDelegateDropState += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventDropState =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::DDContainer *, MyGUI::DropItemState > *>(
						new Delegate2< HandleDropState^, MyGUI::DDContainer *, MyGUI::DropItemState >(mDelegateDropState) );
			}
			void remove(HandleDropState^ _value)
			{
				mDelegateDropState -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateDropState == nullptr)
					static_cast<ThisType*>(mNative)->eventDropState = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventDropState =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::DDContainer *, MyGUI::DropItemState > *>(
							new Delegate2< HandleDropState^, MyGUI::DDContainer *, MyGUI::DropItemState >(mDelegateDropState) );
			}
		}
	private:
		HandleDropState^ mDelegateDropState;




   	public:
		delegate void HandleEndDrop( Convert<MyGUI::DDContainer *>::Type _sender, Convert<const MyGUI::ItemDropInfo &>::Type _info, Convert<bool>::Type _result );
		event HandleEndDrop^ EventEndDrop
		{
			void add(HandleEndDrop^ _value)
			{
				mDelegateEndDrop += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventEndDrop =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool > *>(
						new Delegate3< HandleEndDrop^, MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool >(mDelegateEndDrop) );
			}
			void remove(HandleEndDrop^ _value)
			{
				mDelegateEndDrop -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateEndDrop == nullptr)
					static_cast<ThisType*>(mNative)->eventEndDrop = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventEndDrop =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool > *>(
							new Delegate3< HandleEndDrop^, MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool >(mDelegateEndDrop) );
			}
		}
	private:
		HandleEndDrop^ mDelegateEndDrop;




   	public:
		delegate void HandleRequestDrop( Convert<MyGUI::DDContainer *>::Type _sender, Convert<const MyGUI::ItemDropInfo &>::Type _info, Convert<bool &>::Type _result );
		event HandleRequestDrop^ EventRequestDrop
		{
			void add(HandleRequestDrop^ _value)
			{
				mDelegateRequestDrop += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventRequestDrop =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool & > *>(
						new Delegate3< HandleRequestDrop^, MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool & >(mDelegateRequestDrop) );
			}
			void remove(HandleRequestDrop^ _value)
			{
				mDelegateRequestDrop -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateRequestDrop == nullptr)
					static_cast<ThisType*>(mNative)->eventRequestDrop = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventRequestDrop =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool & > *>(
							new Delegate3< HandleRequestDrop^, MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool & >(mDelegateRequestDrop) );
			}
		}
	private:
		HandleRequestDrop^ mDelegateRequestDrop;




   	public:
		delegate void HandleStartDrop( Convert<MyGUI::DDContainer *>::Type _sender, Convert<const MyGUI::ItemDropInfo &>::Type _info, Convert<bool &>::Type _result );
		event HandleStartDrop^ EventStartDrop
		{
			void add(HandleStartDrop^ _value)
			{
				mDelegateStartDrop += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventStartDrop =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool & > *>(
						new Delegate3< HandleStartDrop^, MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool & >(mDelegateStartDrop) );
			}
			void remove(HandleStartDrop^ _value)
			{
				mDelegateStartDrop -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateStartDrop == nullptr)
					static_cast<ThisType*>(mNative)->eventStartDrop = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventStartDrop =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool & > *>(
							new Delegate3< HandleStartDrop^, MyGUI::DDContainer *, const MyGUI::ItemDropInfo &, bool & >(mDelegateStartDrop) );
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

MMYGUI_END_NAMESPACE
