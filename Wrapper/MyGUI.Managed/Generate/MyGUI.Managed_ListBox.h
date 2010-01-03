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

		public ref class ListBox : public Widget
		{

		private:
			typedef MyGUI::List ThisType;

		public:
			ListBox() : Widget() { }

		internal:
			ListBox( MyGUI::List* _native ) : Widget(_native) { }
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
			Convert<MyGUI::List *>::Type _sender ,
			Convert<size_t>::Type _position );
		event HandleListChangeScroll^ EventListChangeScroll
		{
			void add(HandleListChangeScroll^ _value)
			{
				mDelegateListChangeScroll += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventListChangeScroll =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::List * ,
						size_t > *>(
							new Delegate2< HandleListChangeScroll^ ,
							MyGUI::List * ,
							size_t >(mDelegateListChangeScroll) );
			}
			void remove(HandleListChangeScroll^ _value)
			{
				mDelegateListChangeScroll -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateListChangeScroll == nullptr)
					static_cast<ThisType*>(mNative)->eventListChangeScroll = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListChangeScroll =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::List * ,
							size_t > *>(
								new Delegate2< HandleListChangeScroll^ ,
									MyGUI::List * ,
									size_t >(mDelegateListChangeScroll) );
			}
		}
	private:
		HandleListChangeScroll^ mDelegateListChangeScroll;



   	public:
		delegate void HandleListMouseItemFocus(
			Convert<MyGUI::List *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleListMouseItemFocus^ EventListMouseItemFocus
		{
			void add(HandleListMouseItemFocus^ _value)
			{
				mDelegateListMouseItemFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventListMouseItemFocus =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::List * ,
						size_t > *>(
							new Delegate2< HandleListMouseItemFocus^ ,
							MyGUI::List * ,
							size_t >(mDelegateListMouseItemFocus) );
			}
			void remove(HandleListMouseItemFocus^ _value)
			{
				mDelegateListMouseItemFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateListMouseItemFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventListMouseItemFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListMouseItemFocus =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::List * ,
							size_t > *>(
								new Delegate2< HandleListMouseItemFocus^ ,
									MyGUI::List * ,
									size_t >(mDelegateListMouseItemFocus) );
			}
		}
	private:
		HandleListMouseItemFocus^ mDelegateListMouseItemFocus;



   	public:
		delegate void HandleListMouseItemActivate(
			Convert<MyGUI::List *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleListMouseItemActivate^ EventListMouseItemActivate
		{
			void add(HandleListMouseItemActivate^ _value)
			{
				mDelegateListMouseItemActivate += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventListMouseItemActivate =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::List * ,
						size_t > *>(
							new Delegate2< HandleListMouseItemActivate^ ,
							MyGUI::List * ,
							size_t >(mDelegateListMouseItemActivate) );
			}
			void remove(HandleListMouseItemActivate^ _value)
			{
				mDelegateListMouseItemActivate -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateListMouseItemActivate == nullptr)
					static_cast<ThisType*>(mNative)->eventListMouseItemActivate = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListMouseItemActivate =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::List * ,
							size_t > *>(
								new Delegate2< HandleListMouseItemActivate^ ,
									MyGUI::List * ,
									size_t >(mDelegateListMouseItemActivate) );
			}
		}
	private:
		HandleListMouseItemActivate^ mDelegateListMouseItemActivate;



   	public:
		delegate void HandleListChangePosition(
			Convert<MyGUI::List *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleListChangePosition^ EventListChangePosition
		{
			void add(HandleListChangePosition^ _value)
			{
				mDelegateListChangePosition += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventListChangePosition =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::List * ,
						size_t > *>(
							new Delegate2< HandleListChangePosition^ ,
							MyGUI::List * ,
							size_t >(mDelegateListChangePosition) );
			}
			void remove(HandleListChangePosition^ _value)
			{
				mDelegateListChangePosition -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateListChangePosition == nullptr)
					static_cast<ThisType*>(mNative)->eventListChangePosition = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListChangePosition =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::List * ,
							size_t > *>(
								new Delegate2< HandleListChangePosition^ ,
									MyGUI::List * ,
									size_t >(mDelegateListChangePosition) );
			}
		}
	private:
		HandleListChangePosition^ mDelegateListChangePosition;



   	public:
		delegate void HandleListSelectAccept(
			Convert<MyGUI::List *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleListSelectAccept^ EventListSelectAccept
		{
			void add(HandleListSelectAccept^ _value)
			{
				mDelegateListSelectAccept += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventListSelectAccept =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::List * ,
						size_t > *>(
							new Delegate2< HandleListSelectAccept^ ,
							MyGUI::List * ,
							size_t >(mDelegateListSelectAccept) );
			}
			void remove(HandleListSelectAccept^ _value)
			{
				mDelegateListSelectAccept -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateListSelectAccept == nullptr)
					static_cast<ThisType*>(mNative)->eventListSelectAccept = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListSelectAccept =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::List * ,
							size_t > *>(
								new Delegate2< HandleListSelectAccept^ ,
									MyGUI::List * ,
									size_t >(mDelegateListSelectAccept) );
			}
		}
	private:
		HandleListSelectAccept^ mDelegateListSelectAccept;



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
		Convert<size_t>::Type GetOptimalHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getOptimalHeight( ) );
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
