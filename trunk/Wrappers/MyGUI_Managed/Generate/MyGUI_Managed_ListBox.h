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

		public ref class ListBox : public Widget
		{

		private:
			typedef MyGUI::ListBox ThisType;

		public:
			ListBox() : Widget() { }

		internal:
			ListBox( MyGUI::ListBox* _native ) : Widget(_native) { }
			ListBox( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew ListBox(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

	public:
		delegate void HandleListChangeScroll(
			Convert<MyGUI::ListBox *>::Type _sender ,
			Convert<size_t>::Type _position );
		event HandleListChangeScroll^ EventListChangeScroll
		{
			void add(HandleListChangeScroll^ _value)
			{
				bool empty = mDelegateListChangeScroll == nullptr;

				mDelegateListChangeScroll += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventListChangeScroll +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ListBox * ,
							size_t > *>(
								new Delegate2< HandleListChangeScroll^ ,
								MyGUI::ListBox * ,
								size_t >(mDelegateListChangeScroll) );
			}
			void remove(HandleListChangeScroll^ _value)
			{
				mDelegateListChangeScroll -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateListChangeScroll == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventListChangeScroll -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ListBox * ,
							size_t > *>(
								new Delegate2< HandleListChangeScroll^ ,
									MyGUI::ListBox * ,
									size_t >(mDelegateListChangeScroll) );
			}
		}
	private:
		HandleListChangeScroll^ mDelegateListChangeScroll;
	public:
		delegate void HandleListMouseItemFocus(
			Convert<MyGUI::ListBox *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleListMouseItemFocus^ EventListMouseItemFocus
		{
			void add(HandleListMouseItemFocus^ _value)
			{
				bool empty = mDelegateListMouseItemFocus == nullptr;

				mDelegateListMouseItemFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventListMouseItemFocus +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ListBox * ,
							size_t > *>(
								new Delegate2< HandleListMouseItemFocus^ ,
								MyGUI::ListBox * ,
								size_t >(mDelegateListMouseItemFocus) );
			}
			void remove(HandleListMouseItemFocus^ _value)
			{
				mDelegateListMouseItemFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateListMouseItemFocus == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventListMouseItemFocus -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ListBox * ,
							size_t > *>(
								new Delegate2< HandleListMouseItemFocus^ ,
									MyGUI::ListBox * ,
									size_t >(mDelegateListMouseItemFocus) );
			}
		}
	private:
		HandleListMouseItemFocus^ mDelegateListMouseItemFocus;
	public:
		delegate void HandleListMouseItemActivate(
			Convert<MyGUI::ListBox *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleListMouseItemActivate^ EventListMouseItemActivate
		{
			void add(HandleListMouseItemActivate^ _value)
			{
				bool empty = mDelegateListMouseItemActivate == nullptr;

				mDelegateListMouseItemActivate += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventListMouseItemActivate +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ListBox * ,
							size_t > *>(
								new Delegate2< HandleListMouseItemActivate^ ,
								MyGUI::ListBox * ,
								size_t >(mDelegateListMouseItemActivate) );
			}
			void remove(HandleListMouseItemActivate^ _value)
			{
				mDelegateListMouseItemActivate -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateListMouseItemActivate == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventListMouseItemActivate -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ListBox * ,
							size_t > *>(
								new Delegate2< HandleListMouseItemActivate^ ,
									MyGUI::ListBox * ,
									size_t >(mDelegateListMouseItemActivate) );
			}
		}
	private:
		HandleListMouseItemActivate^ mDelegateListMouseItemActivate;
	public:
		delegate void HandleListChangePosition(
			Convert<MyGUI::ListBox *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleListChangePosition^ EventListChangePosition
		{
			void add(HandleListChangePosition^ _value)
			{
				bool empty = mDelegateListChangePosition == nullptr;

				mDelegateListChangePosition += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventListChangePosition +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ListBox * ,
							size_t > *>(
								new Delegate2< HandleListChangePosition^ ,
								MyGUI::ListBox * ,
								size_t >(mDelegateListChangePosition) );
			}
			void remove(HandleListChangePosition^ _value)
			{
				mDelegateListChangePosition -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateListChangePosition == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventListChangePosition -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ListBox * ,
							size_t > *>(
								new Delegate2< HandleListChangePosition^ ,
									MyGUI::ListBox * ,
									size_t >(mDelegateListChangePosition) );
			}
		}
	private:
		HandleListChangePosition^ mDelegateListChangePosition;
	public:
		delegate void HandleListSelectAccept(
			Convert<MyGUI::ListBox *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleListSelectAccept^ EventListSelectAccept
		{
			void add(HandleListSelectAccept^ _value)
			{
				bool empty = mDelegateListSelectAccept == nullptr;

				mDelegateListSelectAccept += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventListSelectAccept +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ListBox * ,
							size_t > *>(
								new Delegate2< HandleListSelectAccept^ ,
								MyGUI::ListBox * ,
								size_t >(mDelegateListSelectAccept) );
			}
			void remove(HandleListSelectAccept^ _value)
			{
				mDelegateListSelectAccept -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateListSelectAccept == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventListSelectAccept -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ListBox * ,
							size_t > *>(
								new Delegate2< HandleListSelectAccept^ ,
									MyGUI::ListBox * ,
									size_t >(mDelegateListSelectAccept) );
			}
		}
	private:
		HandleListSelectAccept^ mDelegateListSelectAccept;
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
		void SetScrollPosition(
			Convert<size_t>::Type _position )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setScrollPosition(
				Convert<size_t>::From(_position) );
		}
	public:
		void SetScrollVisible(
			Convert<bool>::Type _visible )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setScrollVisible(
				Convert<bool>::From(_visible) );
		}
	public:
		Convert<bool>::Type IsItemSelectedVisible(
			Convert<bool>::Type _fill )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->isItemSelectedVisible(
					Convert<bool>::From(_fill) ) );
		}
	public:
		Convert<bool>::Type IsItemVisibleAt(
			Convert<size_t>::Type _index ,
			Convert<bool>::Type _fill )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->isItemVisibleAt(
					Convert<size_t>::From(_index) ,
					Convert<bool>::From(_fill) ) );
		}
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
		void SwapItemsAt(
			Convert<size_t>::Type _index1 ,
			Convert<size_t>::Type _index2 )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->swapItemsAt(
				Convert<size_t>::From(_index1) ,
				Convert<size_t>::From(_index2) );
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
		property Convert<int>::Type OptimalHeight
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getOptimalHeight() );
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
