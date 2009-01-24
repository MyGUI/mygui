﻿/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"


namespace MMyGUI
{

	public ref class MenuCtrl : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( MenuCtrl, Widget );

		//InsertPoint

   	public:
		delegate void HandleMenuCtrlClose( Convert<MyGUI::MenuCtrl *>::Type _sender );
		event HandleMenuCtrlClose^ MenuCtrlClose
		{
			void add(HandleMenuCtrlClose^ _value)
			{
				mDelegateMenuCtrlClose += _value;
				static_cast<ThisType*>(mNative)->eventMenuCtrlClose = new Delegate1< HandleMenuCtrlClose^, MyGUI::MenuCtrl * >(mDelegateMenuCtrlClose);
			}
			void remove(HandleMenuCtrlClose^ _value)
			{
				mDelegateMenuCtrlClose -= _value;
				if (mDelegateMenuCtrlClose == nullptr)
					static_cast<ThisType*>(mNative)->eventMenuCtrlClose = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMenuCtrlClose = new Delegate1< HandleMenuCtrlClose^, MyGUI::MenuCtrl * >(mDelegateMenuCtrlClose);
			}
		}
	private:
		HandleMenuCtrlClose^ mDelegateMenuCtrlClose;




   	public:
		delegate void HandleMenuCtrlAccept( Convert<MyGUI::MenuCtrl *>::Type _sender, Convert<MyGUI::MenuItem *>::Type _item );
		event HandleMenuCtrlAccept^ MenuCtrlAccept
		{
			void add(HandleMenuCtrlAccept^ _value)
			{
				mDelegateMenuCtrlAccept += _value;
				static_cast<ThisType*>(mNative)->eventMenuCtrlAccept = new Delegate2< HandleMenuCtrlAccept^, MyGUI::MenuCtrl *, MyGUI::MenuItem * >(mDelegateMenuCtrlAccept);
			}
			void remove(HandleMenuCtrlAccept^ _value)
			{
				mDelegateMenuCtrlAccept -= _value;
				if (mDelegateMenuCtrlAccept == nullptr)
					static_cast<ThisType*>(mNative)->eventMenuCtrlAccept = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMenuCtrlAccept = new Delegate2< HandleMenuCtrlAccept^, MyGUI::MenuCtrl *, MyGUI::MenuItem * >(mDelegateMenuCtrlAccept);
			}
		}
	private:
		HandleMenuCtrlAccept^ mDelegateMenuCtrlAccept;




   	public:
		Convert< MyGUI::MenuItem * >::Type GetMenuItemParent( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->getMenuItemParent( ) );
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
		void SetItemType( Convert< MyGUI::MenuItem * >::Type _item, Convert< MyGUI::MenuItemType >::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemType( Convert< MyGUI::MenuItem * >::From(_item), Convert< MyGUI::MenuItemType >::From(_type) );
		}



   	public:
		void SetItemTypeAt( Convert<size_t>::Type _index, Convert< MyGUI::MenuItemType >::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemTypeAt( Convert<size_t>::From(_index), Convert< MyGUI::MenuItemType >::From(_type) );
		}



   	public:
		Convert< MyGUI::MenuItemType >::Type GetItemType( Convert< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItemType >::To( static_cast<ThisType*>(mNative)->getItemType( Convert< MyGUI::MenuItem * >::From(_item) ) );
		}



   	public:
		Convert< MyGUI::MenuItemType >::Type GetItemTypeAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItemType >::To( static_cast<ThisType*>(mNative)->getItemTypeAt( Convert<size_t>::From(_index) ) );
		}



   	public:
		void RemoveItemChild( Convert< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemChild( Convert< MyGUI::MenuItem * >::From(_item) );
		}



   	public:
		void RemoveItemChildAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemChildAt( Convert<size_t>::From(_index) );
		}



   	public:
		Convert< MyGUI::MenuCtrl * >::Type CreateItemChild( Convert< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuCtrl * >::To( static_cast<ThisType*>(mNative)->createItemChild( Convert< MyGUI::MenuItem * >::From(_item) ) );
		}



   	public:
		Convert< MyGUI::MenuCtrl * >::Type CreateItemChildAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuCtrl * >::To( static_cast<ThisType*>(mNative)->createItemChildAt( Convert<size_t>::From(_index) ) );
		}



   	public:
		Convert< MyGUI::MenuCtrl * >::Type GetItemChild( Convert< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuCtrl * >::To( static_cast<ThisType*>(mNative)->getItemChild( Convert< MyGUI::MenuItem * >::From(_item) ) );
		}



   	public:
		Convert< MyGUI::MenuCtrl * >::Type GetItemChildAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuCtrl * >::To( static_cast<ThisType*>(mNative)->getItemChildAt( Convert<size_t>::From(_index) ) );
		}



   	public:
		void SetItemChildVisible( Convert< MyGUI::MenuItem * >::Type _item, Convert<bool>::Type _visible )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemChildVisible( Convert< MyGUI::MenuItem * >::From(_item), Convert<bool>::From(_visible) );
		}



   	public:
		void SetItemChildVisibleAt( Convert<size_t>::Type _index, Convert<bool>::Type _visible )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemChildVisibleAt( Convert<size_t>::From(_index), Convert<bool>::From(_visible) );
		}



   	public:
		Convert<size_t>::Type FindItemIndexWith( Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->findItemIndexWith( Convert<const Ogre::UTFString &>::From(_name) ) );
		}



   	public:
		Convert<const Ogre::UTFString &>::Type GetItemName( Convert< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const Ogre::UTFString &>::To( static_cast<ThisType*>(mNative)->getItemName( Convert< MyGUI::MenuItem * >::From(_item) ) );
		}



   	public:
		Convert<const Ogre::UTFString &>::Type GetItemNameAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const Ogre::UTFString &>::To( static_cast<ThisType*>(mNative)->getItemNameAt( Convert<size_t>::From(_index) ) );
		}



   	public:
		void SetItemName( Convert< MyGUI::MenuItem * >::Type _item, Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemName( Convert< MyGUI::MenuItem * >::From(_item), Convert<const Ogre::UTFString &>::From(_name) );
		}



   	public:
		void SetItemNameAt( Convert<size_t>::Type _index, Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemNameAt( Convert<size_t>::From(_index), Convert<const Ogre::UTFString &>::From(_name) );
		}



   	public:
		Convert<size_t>::Type GetItemIndexById( Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getItemIndexById( Convert<const std::string &>::From(_id) ) );
		}



   	public:
		Convert< MyGUI::MenuItem * >::Type GetItemById( Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->getItemById( Convert<const std::string &>::From(_id) ) );
		}



   	public:
		Convert<const std::string &>::Type GetItemId( Convert< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getItemId( Convert< MyGUI::MenuItem * >::From(_item) ) );
		}



   	public:
		Convert<const std::string &>::Type GetItemIdAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getItemIdAt( Convert<size_t>::From(_index) ) );
		}



   	public:
		void SetItemId( Convert< MyGUI::MenuItem * >::Type _item, Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemId( Convert< MyGUI::MenuItem * >::From(_item), Convert<const std::string &>::From(_id) );
		}



   	public:
		void SetItemIdAt( Convert<size_t>::Type _index, Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemIdAt( Convert<size_t>::From(_index), Convert<const std::string &>::From(_id) );
		}



   	public:
		void ClearItemData( Convert< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemData( Convert< MyGUI::MenuItem * >::From(_item) );
		}



   	public:
		void ClearItemDataAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemDataAt( Convert<size_t>::From(_index) );
		}



   	public:
		void SetItemData( Convert< MyGUI::MenuItem * >::Type _item, Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemData( Convert< MyGUI::MenuItem * >::From(_item), Convert< MyGUI::Any >::From(_data) );
		}



   	public:
		void SetItemDataAt( Convert<size_t>::Type _index, Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemDataAt( Convert<size_t>::From(_index), Convert< MyGUI::Any >::From(_data) );
		}



   	public:
		Convert< MyGUI::MenuItem * >::Type FindItemWith( Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->findItemWith( Convert<const Ogre::UTFString &>::From(_name) ) );
		}



   	public:
		Convert<size_t>::Type FindItemIndex( Convert< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->findItemIndex( Convert< MyGUI::MenuItem * >::From(_item) ) );
		}



   	public:
		Convert<size_t>::Type GetItemIndex( Convert< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getItemIndex( Convert< MyGUI::MenuItem * >::From(_item) ) );
		}



   	public:
		Convert< MyGUI::MenuItem * >::Type GetItemAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->getItemAt( Convert<size_t>::From(_index) ) );
		}



   	public:
		void RemoveAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllItems( );
		}



   	public:
		void RemoveItem( Convert< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItem( Convert< MyGUI::MenuItem * >::From(_item) );
		}



   	public:
		void RemoveItemAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemAt( Convert<size_t>::From(_index) );
		}



   	public:
		Convert< MyGUI::MenuItem * >::Type AddItem( Convert<const Ogre::UTFString &>::Type _name, Convert< MyGUI::MenuItemType >::Type _type, Convert<const std::string &>::Type _id, Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->addItem( Convert<const Ogre::UTFString &>::From(_name), Convert< MyGUI::MenuItemType >::From(_type), Convert<const std::string &>::From(_id), Convert< MyGUI::Any >::From(_data) ) );
		}


   	public:
		Convert< MyGUI::MenuItem * >::Type AddItem( Convert<const Ogre::UTFString &>::Type _name, Convert< MyGUI::MenuItemType >::Type _type, Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->addItem( Convert<const Ogre::UTFString &>::From(_name), Convert< MyGUI::MenuItemType >::From(_type), Convert<const std::string &>::From(_id) ) );
		}


   	public:
		Convert< MyGUI::MenuItem * >::Type AddItem( Convert<const Ogre::UTFString &>::Type _name, Convert< MyGUI::MenuItemType >::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->addItem( Convert<const Ogre::UTFString &>::From(_name), Convert< MyGUI::MenuItemType >::From(_type) ) );
		}


   	public:
		Convert< MyGUI::MenuItem * >::Type AddItem( Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->addItem( Convert<const Ogre::UTFString &>::From(_name) ) );
		}



   	public:
		Convert< MyGUI::MenuItem * >::Type InsertItem( Convert< MyGUI::MenuItem * >::Type _to, Convert<const Ogre::UTFString &>::Type _name, Convert< MyGUI::MenuItemType >::Type _type, Convert<const std::string &>::Type _id, Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->insertItem( Convert< MyGUI::MenuItem * >::From(_to), Convert<const Ogre::UTFString &>::From(_name), Convert< MyGUI::MenuItemType >::From(_type), Convert<const std::string &>::From(_id), Convert< MyGUI::Any >::From(_data) ) );
		}


   	public:
		Convert< MyGUI::MenuItem * >::Type InsertItem( Convert< MyGUI::MenuItem * >::Type _to, Convert<const Ogre::UTFString &>::Type _name, Convert< MyGUI::MenuItemType >::Type _type, Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->insertItem( Convert< MyGUI::MenuItem * >::From(_to), Convert<const Ogre::UTFString &>::From(_name), Convert< MyGUI::MenuItemType >::From(_type), Convert<const std::string &>::From(_id) ) );
		}


   	public:
		Convert< MyGUI::MenuItem * >::Type InsertItem( Convert< MyGUI::MenuItem * >::Type _to, Convert<const Ogre::UTFString &>::Type _name, Convert< MyGUI::MenuItemType >::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->insertItem( Convert< MyGUI::MenuItem * >::From(_to), Convert<const Ogre::UTFString &>::From(_name), Convert< MyGUI::MenuItemType >::From(_type) ) );
		}


   	public:
		Convert< MyGUI::MenuItem * >::Type InsertItem( Convert< MyGUI::MenuItem * >::Type _to, Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->insertItem( Convert< MyGUI::MenuItem * >::From(_to), Convert<const Ogre::UTFString &>::From(_name) ) );
		}



   	public:
		Convert< MyGUI::MenuItem * >::Type InsertItemAt( Convert<size_t>::Type _index, Convert<const Ogre::UTFString &>::Type _name, Convert< MyGUI::MenuItemType >::Type _type, Convert<const std::string &>::Type _id, Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->insertItemAt( Convert<size_t>::From(_index), Convert<const Ogre::UTFString &>::From(_name), Convert< MyGUI::MenuItemType >::From(_type), Convert<const std::string &>::From(_id), Convert< MyGUI::Any >::From(_data) ) );
		}


   	public:
		Convert< MyGUI::MenuItem * >::Type InsertItemAt( Convert<size_t>::Type _index, Convert<const Ogre::UTFString &>::Type _name, Convert< MyGUI::MenuItemType >::Type _type, Convert<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->insertItemAt( Convert<size_t>::From(_index), Convert<const Ogre::UTFString &>::From(_name), Convert< MyGUI::MenuItemType >::From(_type), Convert<const std::string &>::From(_id) ) );
		}


   	public:
		Convert< MyGUI::MenuItem * >::Type InsertItemAt( Convert<size_t>::Type _index, Convert<const Ogre::UTFString &>::Type _name, Convert< MyGUI::MenuItemType >::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->insertItemAt( Convert<size_t>::From(_index), Convert<const Ogre::UTFString &>::From(_name), Convert< MyGUI::MenuItemType >::From(_type) ) );
		}


   	public:
		Convert< MyGUI::MenuItem * >::Type InsertItemAt( Convert<size_t>::Type _index, Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuItem * >::To( static_cast<ThisType*>(mNative)->insertItemAt( Convert<size_t>::From(_index), Convert<const Ogre::UTFString &>::From(_name) ) );
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



   	public:
		property bool Visible
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isVisible( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVisible(_value);
			}
		}



	};

} // namespace MMyGUI
