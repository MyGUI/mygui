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
		void deleteAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteAllItems( );
		}



   	public:
		void deleteItem( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteItem( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type getItem( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItem( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void setItem( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItem( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void insertItem( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
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
		void resetItemSelect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->resetItemSelect( );
		}



   	public:
		void insertColumn( ConvertToType<size_t>::Type _column, ConvertToType<int>::Type _width, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertColumn( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void setColumnName( ConvertToType<size_t>::Type _column, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnName( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void addColumn( ConvertToType<int>::Type _width, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addColumn( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void setColumnWidth( ConvertToType<size_t>::Type _column, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnWidth( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<int>::ConvertFromValue(_width) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type getColumnName( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getColumnName( ConvertToType<size_t>::ConvertFromValue(_column) ) );
		}



   	public:
		ConvertToType<int>::Type getColumnWidth( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getColumnWidth( ConvertToType<size_t>::ConvertFromValue(_column) ) );
		}



   	public:
		void deleteAllColumns( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteAllColumns( );
		}



   	public:
		void deleteColumn( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteColumn( ConvertToType<size_t>::ConvertFromValue(_column) );
		}



   	public:
		void setSubItem( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSubItem( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type getSubItem( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getSubItem( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		ConvertToType<size_t>::Type findItem( ConvertToType<size_t>::Type _column, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findItem( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
		}



   	public:
		void clearSubItemDataAt( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearSubItemDataAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void setSubItemDataAt( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSubItemDataAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		ConvertToType<size_t>::Type findSubItemWith( ConvertToType<size_t>::Type _column, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findSubItemWith( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type getSubItemNameAt( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getSubItemNameAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void setSubItemNameAt( ConvertToType<size_t>::Type _column, ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSubItemNameAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void clearItemDataAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemDataAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void setItemDataAt( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemDataAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void setItemSelectedAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemSelectedAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void clearItemSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemSelected( );
		}



   	public:
		ConvertToType<size_t>::Type getItemIndexSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIndexSelected( ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type getItemNameAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemNameAt( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		void setItemNameAt( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemNameAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void swapItemsAt( ConvertToType<size_t>::Type _index1, ConvertToType<size_t>::Type _index2 )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->swapItemsAt( ConvertToType<size_t>::ConvertFromValue(_index1), ConvertToType<size_t>::ConvertFromValue(_index2) );
		}



   	public:
		void removeAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllItems( );
		}



   	public:
		void removeItemAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void addItem( ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void insertItemAt( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		ConvertToType<size_t>::Type getItemCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemCount( ) );
		}



   	public:
		void clearColumnDataAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearColumnDataAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		void setColumnDataAt( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnDataAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void sortByColumn( ConvertToType<size_t>::Type _column, ConvertToType<bool>::Type _backward )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->sortByColumn( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<bool>::ConvertFromValue(_backward) );
		}



   	public:
		ConvertToType<int>::Type getColumnWidthAt( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getColumnWidthAt( ConvertToType<size_t>::ConvertFromValue(_column) ) );
		}



   	public:
		ConvertToType<const Ogre::UTFString &>::Type getColumnNameAt( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getColumnNameAt( ConvertToType<size_t>::ConvertFromValue(_column) ) );
		}



   	public:
		void setColumnWidthAt( ConvertToType<size_t>::Type _column, ConvertToType<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnWidthAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<int>::ConvertFromValue(_width) );
		}



   	public:
		void setColumnNameAt( ConvertToType<size_t>::Type _column, ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnNameAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) );
		}



   	public:
		void removeAllColumns( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllColumns( );
		}



   	public:
		void removeColumnAt( ConvertToType<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeColumnAt( ConvertToType<size_t>::ConvertFromValue(_column) );
		}



   	public:
		void addColumn( ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType<int>::Type _width, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addColumn( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType<int>::ConvertFromValue(_width), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void insertColumnAt( ConvertToType<size_t>::Type _column, ConvertToType<const Ogre::UTFString &>::Type _name, ConvertToType<int>::Type _width, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertColumnAt( ConvertToType<size_t>::ConvertFromValue(_column), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name), ConvertToType<int>::ConvertFromValue(_width), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		ConvertToType<size_t>::Type getColumnCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getColumnCount( ) );
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
