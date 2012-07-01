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

		public ref class MenuControl : public Widget
		{

		private:
			typedef MyGUI::MenuControl ThisType;

		public:
			MenuControl() : Widget() { }

		internal:
			MenuControl( MyGUI::MenuControl* _native ) : Widget(_native) { }
			MenuControl( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew MenuControl(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

	public:
		delegate void HandleMenuCtrlClose(
			Convert<MyGUI::MenuControl *>::Type _sender );
		event HandleMenuCtrlClose^ EventMenuCtrlClose
		{
			void add(HandleMenuCtrlClose^ _value)
			{
				bool empty = mDelegateMenuCtrlClose == nullptr;

				mDelegateMenuCtrlClose += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMenuCtrlClose +=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::MenuControl * > *>(
								new Delegate1< HandleMenuCtrlClose^ ,
								MyGUI::MenuControl * >(mDelegateMenuCtrlClose) );
			}
			void remove(HandleMenuCtrlClose^ _value)
			{
				mDelegateMenuCtrlClose -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMenuCtrlClose == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventMenuCtrlClose -=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::MenuControl * > *>(
								new Delegate1< HandleMenuCtrlClose^ ,
									MyGUI::MenuControl * >(mDelegateMenuCtrlClose) );
			}
		}
	private:
		HandleMenuCtrlClose^ mDelegateMenuCtrlClose;
	public:
		delegate void HandleMenuCtrlAccept(
			Convert<MyGUI::MenuControl *>::Type _sender ,
			Convert<MyGUI::MenuItem *>::Type _item );
		event HandleMenuCtrlAccept^ EventMenuCtrlAccept
		{
			void add(HandleMenuCtrlAccept^ _value)
			{
				bool empty = mDelegateMenuCtrlAccept == nullptr;

				mDelegateMenuCtrlAccept += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMenuCtrlAccept +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::MenuControl * ,
							MyGUI::MenuItem * > *>(
								new Delegate2< HandleMenuCtrlAccept^ ,
								MyGUI::MenuControl * ,
								MyGUI::MenuItem * >(mDelegateMenuCtrlAccept) );
			}
			void remove(HandleMenuCtrlAccept^ _value)
			{
				mDelegateMenuCtrlAccept -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMenuCtrlAccept == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventMenuCtrlAccept -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::MenuControl * ,
							MyGUI::MenuItem * > *>(
								new Delegate2< HandleMenuCtrlAccept^ ,
									MyGUI::MenuControl * ,
									MyGUI::MenuItem * >(mDelegateMenuCtrlAccept) );
			}
		}
	private:
		HandleMenuCtrlAccept^ mDelegateMenuCtrlAccept;
	public:
		void SetItemType(
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemType(
				Convert<MyGUI::MenuItem *>::From(_item) ,
				Convert<MyGUI::MenuItemType>::From(_type) );
		}
	public:
		void SetItemTypeAt(
			Convert<size_t>::Type _index ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemTypeAt(
				Convert<size_t>::From(_index) ,
				Convert<MyGUI::MenuItemType>::From(_type) );
		}
	public:
		Convert<MyGUI::MenuItemType>::Type GetItemType(
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItemType>::To(
				static_cast<ThisType*>(mNative)->getItemType(
					Convert<MyGUI::MenuItem *>::From(_item) ) );
		}
	public:
		Convert<MyGUI::MenuItemType>::Type GetItemTypeAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItemType>::To(
				static_cast<ThisType*>(mNative)->getItemTypeAt(
					Convert<size_t>::From(_index) ) );
		}
	public:
		void RemoveItemChild(
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemChild(
				Convert<MyGUI::MenuItem *>::From(_item) );
		}
	public:
		void RemoveItemChildAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemChildAt(
				Convert<size_t>::From(_index) );
		}
	public:
		Convert<MyGUI::MenuControl *>::Type CreateItemChild(
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuControl *>::To(
				static_cast<ThisType*>(mNative)->createItemChild(
					Convert<MyGUI::MenuItem *>::From(_item) ) );
		}
	public:
		Convert<MyGUI::MenuControl *>::Type CreateItemChildAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuControl *>::To(
				static_cast<ThisType*>(mNative)->createItemChildAt(
					Convert<size_t>::From(_index) ) );
		}
	public:
		Convert<MyGUI::MenuControl *>::Type GetItemChild(
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuControl *>::To(
				static_cast<ThisType*>(mNative)->getItemChild(
					Convert<MyGUI::MenuItem *>::From(_item) ) );
		}
	public:
		Convert<MyGUI::MenuControl *>::Type GetItemChildAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuControl *>::To(
				static_cast<ThisType*>(mNative)->getItemChildAt(
					Convert<size_t>::From(_index) ) );
		}
	public:
		void SetItemChildVisible(
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<bool>::Type _visible )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemChildVisible(
				Convert<MyGUI::MenuItem *>::From(_item) ,
				Convert<bool>::From(_visible) );
		}
	public:
		void SetItemChildVisibleAt(
			Convert<size_t>::Type _index ,
			Convert<bool>::Type _visible )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemChildVisibleAt(
				Convert<size_t>::From(_index) ,
				Convert<bool>::From(_visible) );
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
		Convert<const MyGUI::UString &>::Type GetItemName(
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::UString &>::To(
				static_cast<ThisType*>(mNative)->getItemName(
					Convert<MyGUI::MenuItem *>::From(_item) ) );
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
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemName(
				Convert<MyGUI::MenuItem *>::From(_item) ,
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
		Convert<size_t>::Type GetItemIndexById(
			Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getItemIndexById(
					Convert<const std::string &>::From(_id) ) );
		}
	public:
		Convert<MyGUI::MenuItem *>::Type FindItemById(
			Convert<const std::string &>::Type _id ,
			Convert<bool>::Type _recursive )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->findItemById(
					Convert<const std::string &>::From(_id) ,
					Convert<bool>::From(_recursive) ) );
		}
	public:
		Convert<MyGUI::MenuItem *>::Type GetItemById(
			Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->getItemById(
					Convert<const std::string &>::From(_id) ) );
		}
	public:
		Convert<const std::string &>::Type GetItemId(
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To(
				static_cast<ThisType*>(mNative)->getItemId(
					Convert<MyGUI::MenuItem *>::From(_item) ) );
		}
	public:
		Convert<const std::string &>::Type GetItemIdAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To(
				static_cast<ThisType*>(mNative)->getItemIdAt(
					Convert<size_t>::From(_index) ) );
		}
	public:
		void SetItemId(
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemId(
				Convert<MyGUI::MenuItem *>::From(_item) ,
				Convert<const std::string &>::From(_id) );
		}
	public:
		void SetItemIdAt(
			Convert<size_t>::Type _index ,
			Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemIdAt(
				Convert<size_t>::From(_index) ,
				Convert<const std::string &>::From(_id) );
		}
	public:
		Convert<MyGUI::Any>::Type GetItemData(
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			ObjectHolder* data = static_cast<ThisType*>(mNative)->getItemData< ObjectHolder >(
				Convert<MyGUI::MenuItem *>::From(_item) , false );
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
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemData(
				Convert<MyGUI::MenuItem *>::From(_item) );
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
			Convert<MyGUI::MenuItem *>::Type _item ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemData(
				Convert<MyGUI::MenuItem *>::From(_item) ,
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
		Convert<MyGUI::MenuItem *>::Type FindItemWith(
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->findItemWith(
					Convert<const MyGUI::UString &>::From(_name) ) );
		}
	public:
		Convert<size_t>::Type FindItemIndex(
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->findItemIndex(
					Convert<MyGUI::MenuItem *>::From(_item) ) );
		}
	public:
		Convert<size_t>::Type GetItemIndex(
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getItemIndex(
					Convert<MyGUI::MenuItem *>::From(_item) ) );
		}
	public:
		Convert<MyGUI::MenuItem *>::Type GetItemAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
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
			Convert<MyGUI::MenuItem *>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItem(
				Convert<MyGUI::MenuItem *>::From(_item) );
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
		Convert<MyGUI::MenuItem *>::Type AddItem(
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->addItem(
					Convert<const MyGUI::UString &>::From(_name) ,
					Convert<MyGUI::MenuItemType>::From(_type) ,
					Convert<const std::string &>::From(_id) ,
					Convert<MyGUI::Any>::From(_data) ) );
		}

		Convert<MyGUI::MenuItem *>::Type AddItem(
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->addItem(
					Convert<const MyGUI::UString &>::From(_name) ,
					Convert<MyGUI::MenuItemType>::From(_type) ,
					Convert<const std::string &>::From(_id) ) );
		}

		Convert<MyGUI::MenuItem *>::Type AddItem(
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->addItem(
					Convert<const MyGUI::UString &>::From(_name) ,
					Convert<MyGUI::MenuItemType>::From(_type) ) );
		}

		Convert<MyGUI::MenuItem *>::Type AddItem(
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->addItem(
					Convert<const MyGUI::UString &>::From(_name) ) );
		}
	public:
		Convert<MyGUI::MenuItem *>::Type InsertItem(
			Convert<MyGUI::MenuItem *>::Type _to ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->insertItem(
					Convert<MyGUI::MenuItem *>::From(_to) ,
					Convert<const MyGUI::UString &>::From(_name) ,
					Convert<MyGUI::MenuItemType>::From(_type) ,
					Convert<const std::string &>::From(_id) ,
					Convert<MyGUI::Any>::From(_data) ) );
		}
	public:
		Convert<MyGUI::MenuItem *>::Type InsertItemAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->insertItemAt(
					Convert<size_t>::From(_index) ,
					Convert<const MyGUI::UString &>::From(_name) ,
					Convert<MyGUI::MenuItemType>::From(_type) ,
					Convert<const std::string &>::From(_id) ,
					Convert<MyGUI::Any>::From(_data) ) );
		}

		Convert<MyGUI::MenuItem *>::Type InsertItemAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type ,
			Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->insertItemAt(
					Convert<size_t>::From(_index) ,
					Convert<const MyGUI::UString &>::From(_name) ,
					Convert<MyGUI::MenuItemType>::From(_type) ,
					Convert<const std::string &>::From(_id) ) );
		}

		Convert<MyGUI::MenuItem *>::Type InsertItemAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::MenuItemType>::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->insertItemAt(
					Convert<size_t>::From(_index) ,
					Convert<const MyGUI::UString &>::From(_name) ,
					Convert<MyGUI::MenuItemType>::From(_type) ) );
		}

		Convert<MyGUI::MenuItem *>::Type InsertItemAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuItem *>::To(
				static_cast<ThisType*>(mNative)->insertItemAt(
					Convert<size_t>::From(_index) ,
					Convert<const MyGUI::UString &>::From(_name) ) );
		}
	public:
		void SetVisibleSmooth(
			Convert<bool>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setVisibleSmooth(
				Convert<bool>::From(_value) );
		}
	public:
		property Convert<bool>::Type VerticalAlignment
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getVerticalAlignment() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVerticalAlignment( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<MyGUI::MenuItem *>::Type MenuItemParent
		{
			Convert<MyGUI::MenuItem *>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::MenuItem *>::To( static_cast<ThisType*>(mNative)->getMenuItemParent() );
			}
		}
	
	public:
		property Convert<bool>::Type PopupAccept
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getPopupAccept() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPopupAccept( Convert<bool>::From(_value) );
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
