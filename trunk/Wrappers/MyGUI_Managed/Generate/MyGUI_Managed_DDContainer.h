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

		public ref class DDContainer : public Widget
		{

		private:
			typedef MyGUI::DDContainer ThisType;

		public:
			DDContainer() : Widget() { }

		internal:
			DDContainer( MyGUI::DDContainer* _native ) : Widget(_native) { }
			DDContainer( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew DDContainer(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

	public:
		delegate void HandleUpdateDropState(
			Convert<MyGUI::DDContainer *>::Type _sender ,
			Convert<MyGUI::Widget *>::Type _items ,
			Convert<const MyGUI::DDWidgetState &>::Type _state );
		event HandleUpdateDropState^ EventUpdateDropState
		{
			void add(HandleUpdateDropState^ _value)
			{
				bool empty = mDelegateUpdateDropState == nullptr;

				mDelegateUpdateDropState += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventUpdateDropState +=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::DDContainer * ,
							MyGUI::Widget * ,
							const MyGUI::DDWidgetState & > *>(
								new Delegate3< HandleUpdateDropState^ ,
								MyGUI::DDContainer * ,
								MyGUI::Widget * ,
								const MyGUI::DDWidgetState & >(mDelegateUpdateDropState) );
			}
			void remove(HandleUpdateDropState^ _value)
			{
				mDelegateUpdateDropState -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateUpdateDropState == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventUpdateDropState -=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::DDContainer * ,
							MyGUI::Widget * ,
							const MyGUI::DDWidgetState & > *>(
								new Delegate3< HandleUpdateDropState^ ,
									MyGUI::DDContainer * ,
									MyGUI::Widget * ,
									const MyGUI::DDWidgetState & >(mDelegateUpdateDropState) );
			}
		}
	private:
		HandleUpdateDropState^ mDelegateUpdateDropState;
	public:
		delegate void HandleDragWidgetInfo(
			Convert<MyGUI::DDContainer *>::Type _sender ,
			Convert<MyGUI::Widget * &>::Type _item ,
			Convert<MyGUI::types::TCoord < int > &>::Type _dimension );
		event HandleDragWidgetInfo^ RequestDragWidgetInfo
		{
			void add(HandleDragWidgetInfo^ _value)
			{
				mDelegateDragWidgetInfo += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->requestDragWidgetInfo =
					static_cast< MyGUI::delegates::IDelegate3<
						MyGUI::DDContainer * ,
						MyGUI::Widget * & ,
						MyGUI::types::TCoord < int > & > *>(
							new Delegate3_Ref2< HandleDragWidgetInfo^ ,
							MyGUI::DDContainer * ,
							MyGUI::Widget * & ,
							MyGUI::types::TCoord < int > & >(mDelegateDragWidgetInfo) );
			}
			void remove(HandleDragWidgetInfo^ _value)
			{
				mDelegateDragWidgetInfo -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateDragWidgetInfo == nullptr)
					static_cast<ThisType*>(mNative)->requestDragWidgetInfo = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestDragWidgetInfo =
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::DDContainer * ,
							MyGUI::Widget * & ,
							MyGUI::types::TCoord < int > & > *>(
								new Delegate3_Ref2< HandleDragWidgetInfo^ ,
									MyGUI::DDContainer * ,
									MyGUI::Widget * & ,
									MyGUI::types::TCoord < int > & >(mDelegateDragWidgetInfo) );
			}
		}
	private:
		HandleDragWidgetInfo^ mDelegateDragWidgetInfo;
	public:
		delegate void HandleChangeDDState(
			Convert<MyGUI::DDContainer *>::Type _sender ,
			Convert<MyGUI::DDItemState>::Type _state );
		event HandleChangeDDState^ EventChangeDDState
		{
			void add(HandleChangeDDState^ _value)
			{
				bool empty = mDelegateChangeDDState == nullptr;

				mDelegateChangeDDState += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventChangeDDState +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::DDContainer * ,
							MyGUI::DDItemState > *>(
								new Delegate2< HandleChangeDDState^ ,
								MyGUI::DDContainer * ,
								MyGUI::DDItemState >(mDelegateChangeDDState) );
			}
			void remove(HandleChangeDDState^ _value)
			{
				mDelegateChangeDDState -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateChangeDDState == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventChangeDDState -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::DDContainer * ,
							MyGUI::DDItemState > *>(
								new Delegate2< HandleChangeDDState^ ,
									MyGUI::DDContainer * ,
									MyGUI::DDItemState >(mDelegateChangeDDState) );
			}
		}
	private:
		HandleChangeDDState^ mDelegateChangeDDState;
	public:
		delegate void HandleDropResult(
			Convert<MyGUI::DDContainer *>::Type _sender ,
			Convert<const MyGUI::DDItemInfo &>::Type _info ,
			Convert<bool>::Type _result );
		event HandleDropResult^ EventDropResult
		{
			void add(HandleDropResult^ _value)
			{
				bool empty = mDelegateDropResult == nullptr;

				mDelegateDropResult += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventDropResult +=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::DDContainer * ,
							const MyGUI::DDItemInfo & ,
							bool > *>(
								new Delegate3< HandleDropResult^ ,
								MyGUI::DDContainer * ,
								const MyGUI::DDItemInfo & ,
								bool >(mDelegateDropResult) );
			}
			void remove(HandleDropResult^ _value)
			{
				mDelegateDropResult -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateDropResult == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventDropResult -=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::DDContainer * ,
							const MyGUI::DDItemInfo & ,
							bool > *>(
								new Delegate3< HandleDropResult^ ,
									MyGUI::DDContainer * ,
									const MyGUI::DDItemInfo & ,
									bool >(mDelegateDropResult) );
			}
		}
	private:
		HandleDropResult^ mDelegateDropResult;
	public:
		delegate void HandleRequestDrop(
			Convert<MyGUI::DDContainer *>::Type _sender ,
			Convert<const MyGUI::DDItemInfo &>::Type _info ,
			Convert<bool &>::Type _result );
		event HandleRequestDrop^ EventRequestDrop
		{
			void add(HandleRequestDrop^ _value)
			{
				bool empty = mDelegateRequestDrop == nullptr;

				mDelegateRequestDrop += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventRequestDrop +=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::DDContainer * ,
							const MyGUI::DDItemInfo & ,
							bool & > *>(
								new Delegate3< HandleRequestDrop^ ,
								MyGUI::DDContainer * ,
								const MyGUI::DDItemInfo & ,
								bool & >(mDelegateRequestDrop) );
			}
			void remove(HandleRequestDrop^ _value)
			{
				mDelegateRequestDrop -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateRequestDrop == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventRequestDrop -=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::DDContainer * ,
							const MyGUI::DDItemInfo & ,
							bool & > *>(
								new Delegate3< HandleRequestDrop^ ,
									MyGUI::DDContainer * ,
									const MyGUI::DDItemInfo & ,
									bool & >(mDelegateRequestDrop) );
			}
		}
	private:
		HandleRequestDrop^ mDelegateRequestDrop;
	public:
		delegate void HandleStartDrag(
			Convert<MyGUI::DDContainer *>::Type _sender ,
			Convert<const MyGUI::DDItemInfo &>::Type _info ,
			Convert<bool &>::Type _result );
		event HandleStartDrag^ EventStartDrag
		{
			void add(HandleStartDrag^ _value)
			{
				bool empty = mDelegateStartDrag == nullptr;

				mDelegateStartDrag += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventStartDrag +=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::DDContainer * ,
							const MyGUI::DDItemInfo & ,
							bool & > *>(
								new Delegate3< HandleStartDrag^ ,
								MyGUI::DDContainer * ,
								const MyGUI::DDItemInfo & ,
								bool & >(mDelegateStartDrag) );
			}
			void remove(HandleStartDrag^ _value)
			{
				mDelegateStartDrag -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateStartDrag == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventStartDrag -=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::DDContainer * ,
							const MyGUI::DDItemInfo & ,
							bool & > *>(
								new Delegate3< HandleStartDrag^ ,
									MyGUI::DDContainer * ,
									const MyGUI::DDItemInfo & ,
									bool & >(mDelegateStartDrag) );
			}
		}
	private:
		HandleStartDrag^ mDelegateStartDrag;
	public:
		void ResetDrag( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->resetDrag( );
		}
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

	} // namespace Managed
} // namespace MyGUI
