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

		public ref class TabBar : public Widget
		{

		private:
			typedef MyGUI::Tab ThisType;

		public:
			TabBar() : Widget() { }

		internal:
			TabBar( MyGUI::Tab* _native ) : Widget(_native) { }
			TabBar( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew TabBar(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   	public:
		delegate void HandleTabChangeSelect(
			Convert<MyGUI::Tab *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleTabChangeSelect^ EventTabChangeSelect
		{
			void add(HandleTabChangeSelect^ _value)
			{
				mDelegateTabChangeSelect += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventTabChangeSelect =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::Tab * ,
						size_t > *>(
							new Delegate2< HandleTabChangeSelect^ ,
							MyGUI::Tab * ,
							size_t >(mDelegateTabChangeSelect) );
			}
			void remove(HandleTabChangeSelect^ _value)
			{
				mDelegateTabChangeSelect -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateTabChangeSelect == nullptr)
					static_cast<ThisType*>(mNative)->eventTabChangeSelect = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventTabChangeSelect =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Tab * ,
							size_t > *>(
								new Delegate2< HandleTabChangeSelect^ ,
									MyGUI::Tab * ,
									size_t >(mDelegateTabChangeSelect) );
			}
		}
	private:
		HandleTabChangeSelect^ mDelegateTabChangeSelect;



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
		property Convert<bool>::Type ButtonAutoWidth
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getButtonAutoWidth() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setButtonAutoWidth( Convert<bool>::From(_value) );
			}
		}
	


   	public:
		property Convert<int>::Type ButtonDefaultWidth
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getButtonDefaultWidth() );
			}
			void set(Convert<int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setButtonDefaultWidth( Convert<int>::From(_value) );
			}
		}
	


   	public:
		Convert<int>::Type GetButtonWidth(
			Convert<MyGUI::TabItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getButtonWidth(
					Convert<MyGUI::TabItem *>::From(_item) ) );
		}



   	public:
		Convert<int>::Type GetButtonWidthAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getButtonWidthAt(
					Convert<size_t>::From(_index) ) );
		}



   	public:
		void SetButtonWidth(
			Convert<MyGUI::TabItem *>::Type _item ,
			Convert<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setButtonWidth(
				Convert<MyGUI::TabItem *>::From(_item) ,
				Convert<int>::From(_width) );
		}



   	public:
		void SetButtonWidthAt(
			Convert<size_t>::Type _index ,
			Convert<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setButtonWidthAt(
				Convert<size_t>::From(_index) ,
				Convert<int>::From(_width) );
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
		void BeginToItem(
			Convert<MyGUI::TabItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToItem(
				Convert<MyGUI::TabItem *>::From(_item) );
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
		Convert<const MyGUI::UString &>::Type GetItemName(
			Convert<MyGUI::TabItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::UString &>::To(
				static_cast<ThisType*>(mNative)->getItemName(
					Convert<MyGUI::TabItem *>::From(_item) ) );
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
		void SetItemName(
			Convert<MyGUI::TabItem *>::Type _item ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemName(
				Convert<MyGUI::TabItem *>::From(_item) ,
				Convert<const MyGUI::UString &>::From(_name) );
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
		Convert<MyGUI::Any>::Type GetItemData(
			Convert<MyGUI::TabItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			ObjectHolder* data = static_cast<ThisType*>(mNative)->getItemData< ObjectHolder >(
				Convert<MyGUI::TabItem *>::From(_item) , false );
			return data ? data->toObject() : nullptr;
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
		void ClearItemData(
			Convert<MyGUI::TabItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemData(
				Convert<MyGUI::TabItem *>::From(_item) );
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
		void SetItemData(
			Convert<MyGUI::TabItem *>::Type _item ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemData(
				Convert<MyGUI::TabItem *>::From(_item) ,
				Convert<MyGUI::Any>::From(_data) );
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
		property Convert<MyGUI::TabItem *>::Type ItemSelected
		{
			Convert<MyGUI::TabItem *>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::TabItem *>::To( static_cast<ThisType*>(mNative)->getItemSelected() );
			}
			void set(Convert<MyGUI::TabItem *>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemSelected( Convert<MyGUI::TabItem *>::From(_value) );
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
		Convert<MyGUI::TabItem *>::Type FindItemWith(
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::TabItem *>::To(
				static_cast<ThisType*>(mNative)->findItemWith(
					Convert<const MyGUI::UString &>::From(_name) ) );
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
		Convert<size_t>::Type FindItemIndex(
			Convert<MyGUI::TabItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->findItemIndex(
					Convert<MyGUI::TabItem *>::From(_item) ) );
		}



   	public:
		Convert<size_t>::Type GetItemIndex(
			Convert<MyGUI::TabItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getItemIndex(
					Convert<MyGUI::TabItem *>::From(_item) ) );
		}



   	public:
		Convert<MyGUI::TabItem *>::Type GetItemAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::TabItem *>::To(
				static_cast<ThisType*>(mNative)->getItemAt(
					Convert<size_t>::From(_index) ) );
		}



   	public:
		void RemoveAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllItems( );
		}



   	public:
		void RemoveItem(
			Convert<MyGUI::TabItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItem(
				Convert<MyGUI::TabItem *>::From(_item) );
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
		Convert<MyGUI::TabItem *>::Type AddItem(
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::TabItem *>::To(
				static_cast<ThisType*>(mNative)->addItem(
					Convert<const MyGUI::UString &>::From(_name) ,
					Convert<MyGUI::Any>::From(_data) ) );
		}

		Convert<MyGUI::TabItem *>::Type AddItem(
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::TabItem *>::To(
				static_cast<ThisType*>(mNative)->addItem(
					Convert<const MyGUI::UString &>::From(_name) ) );
		}



   	public:
		Convert<MyGUI::TabItem *>::Type InsertItem(
			Convert<MyGUI::TabItem *>::Type _to ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::TabItem *>::To(
				static_cast<ThisType*>(mNative)->insertItem(
					Convert<MyGUI::TabItem *>::From(_to) ,
					Convert<const MyGUI::UString &>::From(_name) ,
					Convert<MyGUI::Any>::From(_data) ) );
		}



   	public:
		Convert<MyGUI::TabItem *>::Type InsertItemAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::TabItem *>::To(
				static_cast<ThisType*>(mNative)->insertItemAt(
					Convert<size_t>::From(_index) ,
					Convert<const MyGUI::UString &>::From(_name) ,
					Convert<MyGUI::Any>::From(_data) ) );
		}

		Convert<MyGUI::TabItem *>::Type InsertItemAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::TabItem *>::To(
				static_cast<ThisType*>(mNative)->insertItemAt(
					Convert<size_t>::From(_index) ,
					Convert<const MyGUI::UString &>::From(_name) ) );
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
