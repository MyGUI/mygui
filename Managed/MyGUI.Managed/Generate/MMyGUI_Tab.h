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

	public ref class Tab : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( Tab, Widget );

		//InsertPoint

   	public:
		delegate void HandleTabChangeSelect( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<size_t>::Type _value2 );
		event HandleTabChangeSelect^ TabChangeSelect
		{
			void add(HandleTabChangeSelect^ _value)
			{
				mDelegateTabChangeSelect += _value;
				static_cast<ThisType*>(mNative)->eventTabChangeSelect = new Delegate2< HandleTabChangeSelect^, MyGUI::Widget *, size_t >(mDelegateTabChangeSelect);
			}
			void remove(HandleTabChangeSelect^ _value)
			{
				mDelegateTabChangeSelect -= _value;
				if (mDelegateTabChangeSelect == nullptr)
					static_cast<ThisType*>(mNative)->eventTabChangeSelect = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventTabChangeSelect = new Delegate2< HandleTabChangeSelect^, MyGUI::Widget *, size_t >(mDelegateTabChangeSelect);
			}
		}
	private:
		HandleTabChangeSelect^ mDelegateTabChangeSelect;




   	public:
		ConvertToType<size_t>::Type GetSelectSheetIndex( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getSelectSheetIndex( ) );
		}



   	public:
		void SelectSheet( ConvertToType< MyGUI::TabItem * >::Type _sheet, ConvertToType<bool>::Type _smooth )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->selectSheet( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_sheet), ConvertToType<bool>::ConvertFromValue(_smooth) );
		}



   	public:
		void SelectSheetIndex( ConvertToType<size_t>::Type _index, ConvertToType<bool>::Type _smooth )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->selectSheetIndex( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<bool>::ConvertFromValue(_smooth) );
		}



   	public:
		void RemoveSheet( ConvertToType< MyGUI::TabItem * >::Type _sheet )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeSheet( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_sheet) );
		}



   	public:
		void RemoveSheetIndex( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeSheetIndex( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType< MyGUI::TabItem * >::Type InsertSheet( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::TabItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->insertSheet( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType<int>::ConvertFromValue(_width) ) );
		}



   	public:
		ConvertToType< MyGUI::TabItem * >::Type AddSheet( ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::TabItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->addSheet( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType<int>::ConvertFromValue(_width) ) );
		}



   	public:
		void SetSheetName( ConvertToType< MyGUI::TabItem * >::Type _sheet, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSheetName( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_sheet), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType<int>::ConvertFromValue(_width) );
		}



   	public:
		void SetSheetNameIndex( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSheetNameIndex( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType<int>::ConvertFromValue(_width) );
		}



   	public:
		ConvertToType< MyGUI::TabItem * >::Type GetSheet( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::TabItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->getSheet( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetSheetNameIndex( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getSheetNameIndex( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetSheetName( ConvertToType< MyGUI::TabItem * >::Type _sheet )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getSheetName( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_sheet) ) );
		}



   	public:
		ConvertToType<size_t>::Type GetSheetCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getSheetCount( ) );
		}



   	public:
		void ShowBarSelectButton( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showBarSelectButton( );
		}



   	public:
		void ShowBarButton( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showBarButton( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void SetSheetButtonWidth( ConvertToType< MyGUI::TabItem * >::Type _sheet, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSheetButtonWidth( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_sheet), ConvertToType<int>::ConvertFromValue(_width) );
		}



   	public:
		void SetSheetButtonWidthIndex( ConvertToType<size_t>::Type _index, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSheetButtonWidthIndex( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<int>::ConvertFromValue(_width) );
		}



   	public:
		ConvertToType<int>::Type GetSheetButtonWidth( ConvertToType< MyGUI::TabItem * >::Type _sheet )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getSheetButtonWidth( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_sheet) ) );
		}



   	public:
		ConvertToType<int>::Type GetSheetButtonWidthIndex( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getSheetButtonWidthIndex( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		property ConvertToType<bool>::Type SmoothShow
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getSmoothShow() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setSmoothShow( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<bool>::Type ButtonAutoWidth
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getButtonAutoWidth() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setButtonAutoWidth( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<int>::Type ButtonDefaultWidth
		{
			ConvertToType<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getButtonDefaultWidth() );
			}
			void set(ConvertToType<int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setButtonDefaultWidth( ConvertToType<int>::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType<int>::Type GetButtonWidth( ConvertToType< MyGUI::TabItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getButtonWidth( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType<int>::Type GetButtonWidthAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getButtonWidthAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void SetButtonWidth( ConvertToType< MyGUI::TabItem * >::Type _item, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setButtonWidth( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_item), ConvertToType<int>::ConvertFromValue(_width) );
		}



   	public:
		void SetButtonWidthAt( ConvertToType<size_t>::Type _index, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setButtonWidthAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<int>::ConvertFromValue(_width) );
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
		void BeginToItem( ConvertToType< MyGUI::TabItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToItem( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_item) );
		}



   	public:
		void BeginToItemAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToItemAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetItemName( ConvertToType< MyGUI::TabItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemName( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetItemNameAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemNameAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void SetItemName( ConvertToType< MyGUI::TabItem * >::Type _item, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemName( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_item), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void SetItemNameAt( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemNameAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void ClearItemData( ConvertToType< MyGUI::TabItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemData( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_item) );
		}



   	public:
		void ClearItemDataAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemDataAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void SetItemData( ConvertToType< MyGUI::TabItem * >::Type _item, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemData( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_item), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void SetItemDataAt( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemDataAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void ClearItemSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemSelected( );
		}



   	public:
		property ConvertToType< MyGUI::TabItem * >::Type ItemSelected
		{
			ConvertToType< MyGUI::TabItem * >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType< MyGUI::TabItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemSelected() );
			}
			void set(ConvertToType< MyGUI::TabItem * >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemSelected( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_value) );
			}
		}



   	public:
		void SetItemSelectedAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemSelectedAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType<size_t>::Type GetItemIndexSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIndexSelected( ) );
		}



   	public:
		ConvertToType< MyGUI::TabItem * >::Type FindItemWith( ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::TabItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->findItemWith( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
		}



   	public:
		ConvertToType<size_t>::Type FindItemIndexWith( ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findItemIndexWith( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
		}



   	public:
		ConvertToType<size_t>::Type FindItemIndex( ConvertToType< MyGUI::TabItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findItemIndex( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType<size_t>::Type GetItemIndex( ConvertToType< MyGUI::TabItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIndex( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_item) ) );
		}



   	public:
		ConvertToType< MyGUI::TabItem * >::Type GetItemAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::TabItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void RemoveAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllItems( );
		}



   	public:
		void RemoveItem( ConvertToType< MyGUI::TabItem * >::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItem( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_item) );
		}



   	public:
		void RemoveItemAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType< MyGUI::TabItem * >::Type AddItem( ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::TabItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->addItem( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) ) );
		}



   	public:
		ConvertToType< MyGUI::TabItem * >::Type InsertItem( ConvertToType< MyGUI::TabItem * >::Type _to, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::TabItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->insertItem( ConvertToType< MyGUI::TabItem * >::ConvertFromValue(_to), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) ) );
		}



   	public:
		ConvertToType< MyGUI::TabItem * >::Type InsertItemAt( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::TabItem * >::ConvertToValue( static_cast<ThisType*>(mNative)->insertItemAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) ) );
		}



   	public:
		ConvertToType<size_t>::Type GetItemCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemCount( ) );
		}



   	public:
		void SetPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void SetPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void SetCoord( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void SetSize( ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void SetPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top) );
		}



   	public:
		void SetCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void SetSize( ConvertToType<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_size) );
		}



   	public:
		void SetPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::Type _point )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::ConvertFromValue(_point) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
