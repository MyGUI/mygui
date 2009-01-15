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

	public ref class MultiList : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( MultiList, Widget );

		//InsertPoint



   	public:
		delegate void HandleListChangePosition( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<size_t>::Type _value2 );
		event HandleListChangePosition^ ListChangePosition
		{
			void add(HandleListChangePosition^ _value)
			{
				mDelegateListChangePosition += _value;
				static_cast<ThisType*>(mNative)->eventListChangePosition = new Delegate2< HandleListChangePosition^, MyGUI::Widget *, size_t >(mDelegateListChangePosition);
			}
			void remove(HandleListChangePosition^ _value)
			{
				mDelegateListChangePosition -= _value;
				if (mDelegateListChangePosition == nullptr)
					static_cast<ThisType*>(mNative)->eventListChangePosition = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListChangePosition = new Delegate2< HandleListChangePosition^, MyGUI::Widget *, size_t >(mDelegateListChangePosition);
			}
		}
	private:
		HandleListChangePosition^ mDelegateListChangePosition;




   	public:
		delegate void HandleListSelectAccept( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<size_t>::Type _value2 );
		event HandleListSelectAccept^ ListSelectAccept
		{
			void add(HandleListSelectAccept^ _value)
			{
				mDelegateListSelectAccept += _value;
				static_cast<ThisType*>(mNative)->eventListSelectAccept = new Delegate2< HandleListSelectAccept^, MyGUI::Widget *, size_t >(mDelegateListSelectAccept);
			}
			void remove(HandleListSelectAccept^ _value)
			{
				mDelegateListSelectAccept -= _value;
				if (mDelegateListSelectAccept == nullptr)
					static_cast<ThisType*>(mNative)->eventListSelectAccept = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListSelectAccept = new Delegate2< HandleListSelectAccept^, MyGUI::Widget *, size_t >(mDelegateListSelectAccept);
			}
		}
	private:
		HandleListSelectAccept^ mDelegateListSelectAccept;




   	public:
		void DeleteAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteAllItems( );
		}



   	public:
		void DeleteItem( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteItem( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetItem( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItem( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void SetItem( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItem( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void InsertItem( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItem( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		property ConvertToType<size_t>::Type ItemSelect
		{
			ConvertToType<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemSelect() );
			}
			void set(ConvertToType<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemSelect( ConvertToType<size_t>::ConvertFromValue(_value) );
			}
		}



   	public:
		void ResetItemSelect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->resetItemSelect( );
		}



   	public:
		void InsertColumn( ConvertToType<size_t>::Type _column, ConvertToType<int>::Type _width, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertColumn( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void SetColumnName( ConvertToType<size_t>::Type _column, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnName( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void AddColumn( ConvertToType<int>::Type _width, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addColumn( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void SetColumnWidth( ConvertToType<size_t>::Type _column, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnWidth( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<int>::ConvertFromValue(_width) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetColumnName( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getColumnName( ConvertToType<size_t>::ConvertFromValue(_column) ) );
		}



   	public:
		ConvertToType<int>::Type GetColumnWidth( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getColumnWidth( ConvertToType<size_t>::ConvertFromValue(_column) ) );
		}



   	public:
		void DeleteAllColumns( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteAllColumns( );
		}



   	public:
		void DeleteColumn( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteColumn( ConvertToType<size_t>::ConvertFromValue(_column) );
		}



   	public:
		void SetSubItem( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSubItem( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetSubItem( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getSubItem( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		ConvertToType<size_t>::Type FindItem( ConvertToType<size_t>::Type _column, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findItem( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
		}



   	public:
		void ClearSubItemDataAt( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearSubItemDataAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void SetSubItemDataAt( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSubItemDataAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		ConvertToType<size_t>::Type FindSubItemWith( ConvertToType<size_t>::Type _column, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findSubItemWith( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetSubItemNameAt( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getSubItemNameAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void SetSubItemNameAt( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSubItemNameAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void ClearItemDataAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemDataAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void SetItemDataAt( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemDataAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void SetItemSelectedAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemSelectedAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void ClearItemSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemSelected( );
		}



   	public:
		ConvertToType<size_t>::Type GetItemIndexSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIndexSelected( ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetItemNameAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemNameAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void SetItemNameAt( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemNameAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void SwapItemsAt( ConvertToType<size_t>::Type _index1, ConvertToType<size_t>::Type _index2 )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->swapItemsAt( ConvertToType<size_t>::ConvertFromValue(_index1), ConvertToType<size_t>::ConvertFromValue(_index2) );
		}



   	public:
		void RemoveAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllItems( );
		}



   	public:
		void RemoveItemAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void AddItem( ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void InsertItemAt( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		ConvertToType<size_t>::Type GetItemCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemCount( ) );
		}



   	public:
		void ClearColumnDataAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearColumnDataAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void SetColumnDataAt( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnDataAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void SortByColumn( ConvertToType<size_t>::Type _column, ConvertToType<bool>::Type _backward )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->sortByColumn( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<bool>::ConvertFromValue(_backward) );
		}



   	public:
		ConvertToType<int>::Type GetColumnWidthAt( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getColumnWidthAt( ConvertToType<size_t>::ConvertFromValue(_column) ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type GetColumnNameAt( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getColumnNameAt( ConvertToType<size_t>::ConvertFromValue(_column) ) );
		}



   	public:
		void SetColumnWidthAt( ConvertToType<size_t>::Type _column, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnWidthAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<int>::ConvertFromValue(_width) );
		}



   	public:
		void SetColumnNameAt( ConvertToType<size_t>::Type _column, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnNameAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void RemoveAllColumns( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllColumns( );
		}



   	public:
		void RemoveColumnAt( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeColumnAt( ConvertToType<size_t>::ConvertFromValue(_column) );
		}



   	public:
		void AddColumn( ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType<int>::Type _width, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addColumn( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType<int>::ConvertFromValue(_width), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void InsertColumnAt( ConvertToType<size_t>::Type _column, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType<int>::Type _width, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertColumnAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType<int>::ConvertFromValue(_width), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		ConvertToType<size_t>::Type GetColumnCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getColumnCount( ) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
