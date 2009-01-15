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

	public ref class MenuCtrl : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( MenuCtrl, Widget );

		//InsertPoint

   	public:
		delegate void HandleMenuCtrlClose( ConvertToType<MyGUI::MenuCtrl *>::Type _value1 );
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
		delegate void HandleMenuCtrlAccept( ConvertToType<MyGUI::MenuCtrl *>::Type _value1, ConvertToType<MyGUI::MenuItem *>::Type _value2 );
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
		ConvertToType< MyGUI::MenuItem * >::Type GetMenuItemParent( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->getMenuItemParent( ) );
		}



   	public:
		property ConvertToType<bool>::Type PopupAccept
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getPopupAccept() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPopupAccept( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType<bool>::Type IsShowMenu( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isShowMenu( ) );
		}



   	public:
		void HideMenu( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->hideMenu( );
		}



   	public:
		void ShowMenu( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showMenu( );
		}



   	public:
		void HideItemChild( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->hideItemChild( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) );
		}



   	public:
		void HideItemChildAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->hideItemChildAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void ShowItemChild( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showItemChild( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) );
		}



   	public:
		void ShowItemChildAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showItemChildAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void SetItemType( ConvertToType< MyGUI::MenuItem * >::Type _item, ConvertToType< MyGUI::MenuItemType >::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemType( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item), ConvertToType< MyGUI::MenuItemType >::ConvertFromValue(_type) );
		}



   	public:
		void SetItemTypeAt( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::MenuItemType >::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemTypeAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::MenuItemType >::ConvertFromValue(_type) );
		}



   	public:
		ConvertToType< MyGUI::MenuItemType >::Type GetItemType( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItemType >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemType( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuItemType >::Type GetItemTypeAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItemType >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemTypeAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void RemoveItemChild( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemChild( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) );
		}



   	public:
		void RemoveItemChildAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemChildAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType< MyGUI::MenuCtrl * >::Type CreateItemChild( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuCtrl * >::ConvertToValue( static_cast<ThisType*>(mNative)->createItemChild( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuCtrl * >::Type CreateItemChildAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuCtrl * >::ConvertToValue( static_cast<ThisType*>(mNative)->createItemChildAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuCtrl * >::Type GetItemChild( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuCtrl * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemChild( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuCtrl * >::Type GetItemChildAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuCtrl * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemChildAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		ConvertToType<size_t>::Type FindItemIndexWith( ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findItemIndexWith( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetItemName( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemName( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetItemNameAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemNameAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void SetItemName( ConvertToType< MyGUI::MenuItem * >::Type _item, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemName( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void SetItemNameAt( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemNameAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		ConvertToType<size_t>::Type GetItemIndexById( ConvertToType<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIndexById( ConvertToType<const std::string &>::ConvertFromValue(_id) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type GetItemById( ConvertToType<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemById( ConvertToType<const std::string &>::ConvertFromValue(_id) ) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetItemId( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemId( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetItemIdAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIdAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void SetItemId( ConvertToType< MyGUI::MenuItem * >::Type _item, ConvertToType<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemId( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item), ConvertToType<const std::string &>::ConvertFromValue(_id) );
		}



   	public:
		void SetItemIdAt( ConvertToType<size_t>::Type _index, ConvertToType<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemIdAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const std::string &>::ConvertFromValue(_id) );
		}



   	public:
		void ClearItemData( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemData( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) );
		}



   	public:
		void ClearItemDataAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemDataAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void SetItemData( ConvertToType< MyGUI::MenuItem * >::Type _item, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemData( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void SetItemDataAt( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemDataAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type FindItemWith( ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->findItemWith( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
		}



   	public:
		ConvertToType<size_t>::Type FindItemIndex( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findItemIndex( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType<size_t>::Type GetItemIndex( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIndex( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type GetItemAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void RemoveAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllItems( );
		}



   	public:
		void RemoveItem( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItem( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) );
		}



   	public:
		void RemoveItemAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type AddItem( ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::MenuItemType >::Type _type, ConvertToType<const std::string &>::Type _id, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->addItem( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::MenuItemType >::ConvertFromValue(_type), ConvertToType<const std::string &>::ConvertFromValue(_id), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type InsertItem( ConvertToType< MyGUI::MenuItem * >::Type _to, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::MenuItemType >::Type _type, ConvertToType<const std::string &>::Type _id, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->insertItem( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_to), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::MenuItemType >::ConvertFromValue(_type), ConvertToType<const std::string &>::ConvertFromValue(_id), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type InsertItemAt( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::MenuItemType >::Type _type, ConvertToType<const std::string &>::Type _id, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->insertItemAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::MenuItemType >::ConvertFromValue(_type), ConvertToType<const std::string &>::ConvertFromValue(_id), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) ) );
		}



   	public:
		ConvertToType<size_t>::Type GetItemCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemCount( ) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
