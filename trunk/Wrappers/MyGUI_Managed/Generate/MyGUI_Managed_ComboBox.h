/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI_Managed_EditBox.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class ComboBox : public EditBox
		{

		private:
			typedef MyGUI::ComboBox ThisType;

		public:
			ComboBox() : EditBox() { }

		internal:
			ComboBox( MyGUI::ComboBox* _native ) : EditBox(_native) { }
			ComboBox( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew ComboBox(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

	public:
		delegate void HandleComboChangePosition(
			Convert<MyGUI::ComboBox *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleComboChangePosition^ EventComboChangePosition
		{
			void add(HandleComboChangePosition^ _value)
			{
				bool empty = mDelegateComboChangePosition == nullptr;

				mDelegateComboChangePosition += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventComboChangePosition +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ComboBox * ,
							size_t > *>(
								new Delegate2< HandleComboChangePosition^ ,
								MyGUI::ComboBox * ,
								size_t >(mDelegateComboChangePosition) );
			}
			void remove(HandleComboChangePosition^ _value)
			{
				mDelegateComboChangePosition -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateComboChangePosition == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventComboChangePosition -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ComboBox * ,
							size_t > *>(
								new Delegate2< HandleComboChangePosition^ ,
									MyGUI::ComboBox * ,
									size_t >(mDelegateComboChangePosition) );
			}
		}
	private:
		HandleComboChangePosition^ mDelegateComboChangePosition;
	public:
		delegate void HandleComboAccept(
			Convert<MyGUI::ComboBox *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleComboAccept^ EventComboAccept
		{
			void add(HandleComboAccept^ _value)
			{
				bool empty = mDelegateComboAccept == nullptr;

				mDelegateComboAccept += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventComboAccept +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ComboBox * ,
							size_t > *>(
								new Delegate2< HandleComboAccept^ ,
								MyGUI::ComboBox * ,
								size_t >(mDelegateComboAccept) );
			}
			void remove(HandleComboAccept^ _value)
			{
				mDelegateComboAccept -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateComboAccept == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventComboAccept -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ComboBox * ,
							size_t > *>(
								new Delegate2< HandleComboAccept^ ,
									MyGUI::ComboBox * ,
									size_t >(mDelegateComboAccept) );
			}
		}
	private:
		HandleComboAccept^ mDelegateComboAccept;
	public:
		void BeginToItemSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToItemSelected( );
		}
	public:
		void BeginToItemLast( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToItemLast( );
		}
	public:
		void BeginToItemFirst( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToItemFirst( );
		}
	public:
		void BeginToItemAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToItemAt(
				Convert<size_t>::From(_index) );
		}
	public:
		Convert<const MyGUI::UString &>::Type GetItemNameAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::UString &>::To(
				static_cast<ThisType*>(mNative)->getItemNameAt(
					Convert<size_t>::From(_index) ) );
		}
	public:
		void SetItemNameAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemNameAt(
				Convert<size_t>::From(_index) ,
				Convert<const MyGUI::UString &>::From(_name) );
		}
	public:
		Convert<MyGUI::Any>::Type GetItemDataAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			ObjectHolder* data = static_cast<ThisType*>(mNative)->getItemDataAt< ObjectHolder >(
				Convert<size_t>::From(_index) , false );
			return data ? data->toObject() : nullptr;
		}
	public:
		void ClearItemDataAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemDataAt(
				Convert<size_t>::From(_index) );
		}
	public:
		void SetItemDataAt(
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemDataAt(
				Convert<size_t>::From(_index) ,
				Convert<MyGUI::Any>::From(_data) );
		}
	public:
		void ClearIndexSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearIndexSelected( );
		}
	public:
		Convert<size_t>::Type FindItemIndexWith(
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->findItemIndexWith(
					Convert<const MyGUI::UString &>::From(_name) ) );
		}
	public:
		void RemoveAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllItems( );
		}
	public:
		void RemoveItemAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemAt(
				Convert<size_t>::From(_index) );
		}
	public:
		void AddItem(
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem(
				Convert<const MyGUI::UString &>::From(_name) ,
				Convert<MyGUI::Any>::From(_data) );
		}

		void AddItem(
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem(
				Convert<const MyGUI::UString &>::From(_name) );
		}
	public:
		void InsertItemAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt(
				Convert<size_t>::From(_index) ,
				Convert<const MyGUI::UString &>::From(_name) ,
				Convert<MyGUI::Any>::From(_data) );
		}

		void InsertItemAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt(
				Convert<size_t>::From(_index) ,
				Convert<const MyGUI::UString &>::From(_name) );
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
		property Convert<int>::Type MaxListLength
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getMaxListLength() );
			}
			void set(Convert<int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMaxListLength( Convert<int>::From(_value) );
			}
		}
	
	public:
		property Convert<bool>::Type SmoothShow
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getSmoothShow() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setSmoothShow( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<bool>::Type ComboModeDrop
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getComboModeDrop() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setComboModeDrop( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<size_t>::Type IndexSelected
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getIndexSelected() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setIndexSelected( Convert<size_t>::From(_value) );
			}
		}
	
	public:
		property Convert<size_t>::Type ItemCount
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getItemCount() );
			}
		}
	
		};

	} // namespace Managed
} // namespace MyGUI
