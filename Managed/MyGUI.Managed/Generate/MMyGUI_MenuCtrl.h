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
		ConvertToType< MyGUI::MenuItem * >::Type getMenuItemParent( )
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
		ConvertToType<bool>::Type isShowMenu( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isShowMenu( ) );
		}



   	public:
		void hideMenu( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->hideMenu( );
		}



   	public:
		void showMenu( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showMenu( );
		}



   	public:
		void hideItemChild( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->hideItemChild( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) );
		}



   	public:
		void hideItemChildAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->hideItemChildAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void showItemChild( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showItemChild( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) );
		}



   	public:
		void showItemChildAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showItemChildAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void setItemType( ConvertToType< MyGUI::MenuItem * >::Type _item, ConvertToType< MyGUI::MenuItemType >::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemType( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item), ConvertToType< MyGUI::MenuItemType >::ConvertFromValue(_type) );
		}



   	public:
		void setItemTypeAt( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::MenuItemType >::Type _type )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemTypeAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::MenuItemType >::ConvertFromValue(_type) );
		}



   	public:
		ConvertToType< MyGUI::MenuItemType >::Type getItemType( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItemType >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemType( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuItemType >::Type getItemTypeAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItemType >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemTypeAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void removeItemChild( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemChild( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) );
		}



   	public:
		void removeItemChildAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemChildAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType< MyGUI::MenuCtrl * >::Type createItemChild( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuCtrl * >::ConvertToValue( static_cast<ThisType*>(mNative)->createItemChild( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuCtrl * >::Type createItemChildAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuCtrl * >::ConvertToValue( static_cast<ThisType*>(mNative)->createItemChildAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuCtrl * >::Type getItemChild( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuCtrl * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemChild( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuCtrl * >::Type getItemChildAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuCtrl * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemChildAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		ConvertToType<size_t>::Type findItemIndexWith( ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findItemIndexWith( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type getItemName( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemName( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type getItemNameAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemNameAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void setItemName( ConvertToType< MyGUI::MenuItem * >::Type _item, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemName( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void setItemNameAt( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemNameAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		ConvertToType<size_t>::Type getItemIndexById( ConvertToType<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIndexById( ConvertToType<const std::string &>::ConvertFromValue(_id) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type getItemById( ConvertToType<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemById( ConvertToType<const std::string &>::ConvertFromValue(_id) ) );
		}



   	public:
		ConvertToType<const std::string &>::Type getItemId( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemId( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType<const std::string &>::Type getItemIdAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIdAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void setItemId( ConvertToType< MyGUI::MenuItem * >::Type _item, ConvertToType<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemId( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item), ConvertToType<const std::string &>::ConvertFromValue(_id) );
		}



   	public:
		void setItemIdAt( ConvertToType<size_t>::Type _index, ConvertToType<const std::string &>::Type _id )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemIdAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const std::string &>::ConvertFromValue(_id) );
		}



   	public:
		void clearItemData( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemData( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) );
		}



   	public:
		void clearItemDataAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemDataAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void setItemData( ConvertToType< MyGUI::MenuItem * >::Type _item, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemData( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void setItemDataAt( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemDataAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type findItemWith( ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->findItemWith( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
		}



   	public:
		ConvertToType<size_t>::Type findItemIndex( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findItemIndex( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType<size_t>::Type getItemIndex( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIndex( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type getItemAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void removeAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllItems( );
		}



   	public:
		void removeItem( ConvertToType< MyGUI::MenuItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItem( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_item) );
		}



   	public:
		void removeItemAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type addItem( ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::MenuItemType >::Type _type, ConvertToType<const std::string &>::Type _id, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->addItem( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::MenuItemType >::ConvertFromValue(_type), ConvertToType<const std::string &>::ConvertFromValue(_id), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type insertItem( ConvertToType< MyGUI::MenuItem * >::Type _to, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::MenuItemType >::Type _type, ConvertToType<const std::string &>::Type _id, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->insertItem( ConvertToType< MyGUI::MenuItem * >::ConvertFromValue(_to), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::MenuItemType >::ConvertFromValue(_type), ConvertToType<const std::string &>::ConvertFromValue(_id), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) ) );
		}



   	public:
		ConvertToType< MyGUI::MenuItem * >::Type insertItemAt( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::MenuItemType >::Type _type, ConvertToType<const std::string &>::Type _id, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->insertItemAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::MenuItemType >::ConvertFromValue(_type), ConvertToType<const std::string &>::ConvertFromValue(_id), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) ) );
		}



   	public:
		ConvertToType<size_t>::Type getItemCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemCount( ) );
		}



   	public:
		ConvertToType<const std::string &>::Type getTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



   	public:
		ConvertToType<bool>::Type isType( ConvertToType<const std::type_info &>::Type t )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isType( ConvertToType<const std::type_info &>::ConvertFromValue(t) ) );
		}



	};

} // namespace MMyGUI
