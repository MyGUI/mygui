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

	public ref class List : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( List, Widget );

		//InsertPoint

   	public:
		delegate void HandleListChangeScroll( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<size_t>::Type _value2 );
		event HandleListChangeScroll^ ListChangeScroll
		{
			void add(HandleListChangeScroll^ _value)
			{
				mDelegateListChangeScroll += _value;
				static_cast<ThisType*>(mNative)->eventListChangeScroll = new Delegate2< HandleListChangeScroll^, MyGUI::Widget *, size_t >(mDelegateListChangeScroll);
			}
			void remove(HandleListChangeScroll^ _value)
			{
				mDelegateListChangeScroll -= _value;
				if (mDelegateListChangeScroll == nullptr)
					static_cast<ThisType*>(mNative)->eventListChangeScroll = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListChangeScroll = new Delegate2< HandleListChangeScroll^, MyGUI::Widget *, size_t >(mDelegateListChangeScroll);
			}
		}
	private:
		HandleListChangeScroll^ mDelegateListChangeScroll;




   	public:
		delegate void HandleListMouseItemFocus( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<size_t>::Type _value2 );
		event HandleListMouseItemFocus^ ListMouseItemFocus
		{
			void add(HandleListMouseItemFocus^ _value)
			{
				mDelegateListMouseItemFocus += _value;
				static_cast<ThisType*>(mNative)->eventListMouseItemFocus = new Delegate2< HandleListMouseItemFocus^, MyGUI::Widget *, size_t >(mDelegateListMouseItemFocus);
			}
			void remove(HandleListMouseItemFocus^ _value)
			{
				mDelegateListMouseItemFocus -= _value;
				if (mDelegateListMouseItemFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventListMouseItemFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListMouseItemFocus = new Delegate2< HandleListMouseItemFocus^, MyGUI::Widget *, size_t >(mDelegateListMouseItemFocus);
			}
		}
	private:
		HandleListMouseItemFocus^ mDelegateListMouseItemFocus;




   	public:
		delegate void HandleListMouseItemActivate( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<size_t>::Type _value2 );
		event HandleListMouseItemActivate^ ListMouseItemActivate
		{
			void add(HandleListMouseItemActivate^ _value)
			{
				mDelegateListMouseItemActivate += _value;
				static_cast<ThisType*>(mNative)->eventListMouseItemActivate = new Delegate2< HandleListMouseItemActivate^, MyGUI::Widget *, size_t >(mDelegateListMouseItemActivate);
			}
			void remove(HandleListMouseItemActivate^ _value)
			{
				mDelegateListMouseItemActivate -= _value;
				if (mDelegateListMouseItemActivate == nullptr)
					static_cast<ThisType*>(mNative)->eventListMouseItemActivate = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListMouseItemActivate = new Delegate2< HandleListMouseItemActivate^, MyGUI::Widget *, size_t >(mDelegateListMouseItemActivate);
			}
		}
	private:
		HandleListMouseItemActivate^ mDelegateListMouseItemActivate;




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
		ConvertToType<size_t>::Type GetOptimalHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getOptimalHeight( ) );
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
		void SetScrollPosition( ConvertToType<size_t>::Type _position )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setScrollPosition( ConvertToType<size_t>::ConvertFromValue(_position) );
		}



   	public:
		void SetScrollVisible( ConvertToType<bool>::Type _visible )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setScrollVisible( ConvertToType<bool>::ConvertFromValue(_visible) );
		}



   	public:
		ConvertToType<bool>::Type IsItemSelectVisible( ConvertToType<bool>::Type _fill )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isItemSelectVisible( ConvertToType<bool>::ConvertFromValue(_fill) ) );
		}



   	public:
		ConvertToType<bool>::Type IsItemVisible( ConvertToType<size_t>::Type _index, ConvertToType<bool>::Type _fill )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isItemVisible( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<bool>::ConvertFromValue(_fill) ) );
		}



   	public:
		void BeginToSelect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToSelect( );
		}



   	public:
		void BeginToEnd( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToEnd( );
		}



   	public:
		void BeginToStart( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToStart( );
		}



   	public:
		void BeginToIndex( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToIndex( ConvertToType<size_t>::ConvertFromValue(_index) );
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
		ConvertToType<size_t>::Type FindItem( ConvertToType<const Ogre::UTFString &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findItem( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_item) ) );
		}



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
		void SetItem( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItem( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_item) );
		}



   	public:
		void InsertItem( ConvertToType<size_t>::Type _index, ConvertToType<const Ogre::UTFString &>::Type _item )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItem( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_item) );
		}



   	public:
		ConvertToType<bool>::Type IsItemSelectedVisible( ConvertToType<bool>::Type _fill )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isItemSelectedVisible( ConvertToType<bool>::ConvertFromValue(_fill) ) );
		}



   	public:
		ConvertToType<bool>::Type IsItemVisibleAt( ConvertToType<size_t>::Type _index, ConvertToType<bool>::Type _fill )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isItemVisibleAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType<bool>::ConvertFromValue(_fill) ) );
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
		void BeginToItemAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToItemAt( ConvertToType<size_t>::ConvertFromValue(_index) );
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
		void ClearItemSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemSelected( );
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
		ConvertToType<size_t>::Type FindItemIndexWith( ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->findItemIndexWith( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
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
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
