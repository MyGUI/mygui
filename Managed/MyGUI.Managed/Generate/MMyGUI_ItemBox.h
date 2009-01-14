/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"
#include "MMyGUI_DDContainer.h"

namespace MMyGUI
{

	public ref class ItemBox : public DDContainer
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( ItemBox, DDContainer );

		//InsertPoint

   	public:
		delegate void HandleNotifyItem( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<const MyGUI::NotifyItemData &>::Type _value2 );
		event HandleNotifyItem^ NotifyItem
		{
			void add(HandleNotifyItem^ _value)
			{
				mDelegateNotifyItem += _value;
				static_cast<ThisType*>(mNative)->eventNotifyItem = new Delegate2< HandleNotifyItem^, MyGUI::Widget *, const MyGUI::NotifyItemData & >(mDelegateNotifyItem);
			}
			void remove(HandleNotifyItem^ _value)
			{
				mDelegateNotifyItem -= _value;
				if (mDelegateNotifyItem == nullptr)
					static_cast<ThisType*>(mNative)->eventNotifyItem = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventNotifyItem = new Delegate2< HandleNotifyItem^, MyGUI::Widget *, const MyGUI::NotifyItemData & >(mDelegateNotifyItem);
			}
		}
	private:
		HandleNotifyItem^ mDelegateNotifyItem;




   	public:
		delegate void HandleMouseItemActivate( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<size_t>::Type _value2 );
		event HandleMouseItemActivate^ MouseItemActivate
		{
			void add(HandleMouseItemActivate^ _value)
			{
				mDelegateMouseItemActivate += _value;
				static_cast<ThisType*>(mNative)->eventMouseItemActivate = new Delegate2< HandleMouseItemActivate^, MyGUI::Widget *, size_t >(mDelegateMouseItemActivate);
			}
			void remove(HandleMouseItemActivate^ _value)
			{
				mDelegateMouseItemActivate -= _value;
				if (mDelegateMouseItemActivate == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseItemActivate = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseItemActivate = new Delegate2< HandleMouseItemActivate^, MyGUI::Widget *, size_t >(mDelegateMouseItemActivate);
			}
		}
	private:
		HandleMouseItemActivate^ mDelegateMouseItemActivate;




   	public:
		delegate void HandleChangeItemPosition( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<size_t>::Type _value2 );
		event HandleChangeItemPosition^ ChangeItemPosition
		{
			void add(HandleChangeItemPosition^ _value)
			{
				mDelegateChangeItemPosition += _value;
				static_cast<ThisType*>(mNative)->eventChangeItemPosition = new Delegate2< HandleChangeItemPosition^, MyGUI::Widget *, size_t >(mDelegateChangeItemPosition);
			}
			void remove(HandleChangeItemPosition^ _value)
			{
				mDelegateChangeItemPosition -= _value;
				if (mDelegateChangeItemPosition == nullptr)
					static_cast<ThisType*>(mNative)->eventChangeItemPosition = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventChangeItemPosition = new Delegate2< HandleChangeItemPosition^, MyGUI::Widget *, size_t >(mDelegateChangeItemPosition);
			}
		}
	private:
		HandleChangeItemPosition^ mDelegateChangeItemPosition;




   	public:
		delegate void HandleSelectItemAccept( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<size_t>::Type _value2 );
		event HandleSelectItemAccept^ SelectItemAccept
		{
			void add(HandleSelectItemAccept^ _value)
			{
				mDelegateSelectItemAccept += _value;
				static_cast<ThisType*>(mNative)->eventSelectItemAccept = new Delegate2< HandleSelectItemAccept^, MyGUI::Widget *, size_t >(mDelegateSelectItemAccept);
			}
			void remove(HandleSelectItemAccept^ _value)
			{
				mDelegateSelectItemAccept -= _value;
				if (mDelegateSelectItemAccept == nullptr)
					static_cast<ThisType*>(mNative)->eventSelectItemAccept = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventSelectItemAccept = new Delegate2< HandleSelectItemAccept^, MyGUI::Widget *, size_t >(mDelegateSelectItemAccept);
			}
		}
	private:
		HandleSelectItemAccept^ mDelegateSelectItemAccept;




   	public:
		delegate void HandleUpdateWidgetItem( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<MyGUI::Widget *>::Type _value2, ConvertToType<const MyGUI::ItemInfo &>::Type _value3 );
		event HandleUpdateWidgetItem^ UpdateWidgetItem
		{
			void add(HandleUpdateWidgetItem^ _value)
			{
				mDelegateUpdateWidgetItem += _value;
				static_cast<ThisType*>(mNative)->requestUpdateWidgetItem = new Delegate3< HandleUpdateWidgetItem^, MyGUI::Widget *, MyGUI::Widget *, const MyGUI::ItemInfo & >(mDelegateUpdateWidgetItem);
			}
			void remove(HandleUpdateWidgetItem^ _value)
			{
				mDelegateUpdateWidgetItem -= _value;
				if (mDelegateUpdateWidgetItem == nullptr)
					static_cast<ThisType*>(mNative)->requestUpdateWidgetItem = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestUpdateWidgetItem = new Delegate3< HandleUpdateWidgetItem^, MyGUI::Widget *, MyGUI::Widget *, const MyGUI::ItemInfo & >(mDelegateUpdateWidgetItem);
			}
		}
	private:
		HandleUpdateWidgetItem^ mDelegateUpdateWidgetItem;




   	public:
		delegate void HandleCoordWidgetItem( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<MyGUI::types::TCoord< int > &>::Type _value2, ConvertToType<bool>::Type _value3 );
		event HandleCoordWidgetItem^ CoordWidgetItem
		{
			void add(HandleCoordWidgetItem^ _value)
			{
				mDelegateCoordWidgetItem += _value;
				static_cast<ThisType*>(mNative)->requestCoordWidgetItem = new Delegate3< HandleCoordWidgetItem^, MyGUI::Widget *, MyGUI::types::TCoord< int > &, bool >(mDelegateCoordWidgetItem);
			}
			void remove(HandleCoordWidgetItem^ _value)
			{
				mDelegateCoordWidgetItem -= _value;
				if (mDelegateCoordWidgetItem == nullptr)
					static_cast<ThisType*>(mNative)->requestCoordWidgetItem = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestCoordWidgetItem = new Delegate3< HandleCoordWidgetItem^, MyGUI::Widget *, MyGUI::types::TCoord< int > &, bool >(mDelegateCoordWidgetItem);
			}
		}
	private:
		HandleCoordWidgetItem^ mDelegateCoordWidgetItem;




   	public:
		delegate void HandleCreateWidgetItem( ConvertToType<MyGUI::Widget *>::Type _value1, ConvertToType<MyGUI::Widget *>::Type _value2 );
		event HandleCreateWidgetItem^ CreateWidgetItem
		{
			void add(HandleCreateWidgetItem^ _value)
			{
				mDelegateCreateWidgetItem += _value;
				static_cast<ThisType*>(mNative)->requestCreateWidgetItem = new Delegate2< HandleCreateWidgetItem^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateCreateWidgetItem);
			}
			void remove(HandleCreateWidgetItem^ _value)
			{
				mDelegateCreateWidgetItem -= _value;
				if (mDelegateCreateWidgetItem == nullptr)
					static_cast<ThisType*>(mNative)->requestCreateWidgetItem = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestCreateWidgetItem = new Delegate2< HandleCreateWidgetItem^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateCreateWidgetItem);
			}
		}
	private:
		HandleCreateWidgetItem^ mDelegateCreateWidgetItem;




   	public:
		void setPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void setCoord( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setSize( ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top) );
		}



   	public:
		void setCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void setSize( ConvertToType<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_size) );
		}



   	public:
		void setPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::Type _point )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::ConvertFromValue(_point) );
		}



   	public:
		void resetDrop( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->resetDrop( );
		}



   	public:
		ConvertToType< MyGUI::Widget * >::Type getWidgetByIndex( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::Widget * >::ConvertToValue( static_cast<ThisType*>(mNative)->getWidgetByIndex( ConvertToType<size_t>::ConvertFromValue(_index) ) );
		}



   	public:
		ConvertToType< MyGUI::Widget * >::Type getWidgetDrop( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::Widget * >::ConvertToValue( static_cast<ThisType*>(mNative)->getWidgetDrop( ) );
		}



   	public:
		ConvertToType<size_t>::Type getIndexByWidget( ConvertToType< MyGUI::Widget * >::Type _widget )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getIndexByWidget( ConvertToType< MyGUI::Widget * >::ConvertFromValue(_widget) ) );
		}



   	public:
		property ConvertToType<bool>::Type ItemBoxAlignVert
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemBoxAlignVert() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemBoxAlignVert( ConvertToType<bool>::ConvertFromValue(_value) );
			}
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
		void setItemData( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemData( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void insertItem( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItem( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
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
		void clearItemSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemSelected( );
		}



   	public:
		void setItemSelectedAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemSelectedAt( ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType<size_t>::Type getItemIndexSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIndexSelected( ) );
		}



   	public:
		void redrawAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->redrawAllItems( );
		}



   	public:
		void redrawItemAt( ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->redrawItemAt( ConvertToType<size_t>::ConvertFromValue(_index) );
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
		void addItem( ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem( ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
		}



   	public:
		void insertItemAt( ConvertToType<size_t>::Type _index, ConvertToType< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt( ConvertToType<size_t>::ConvertFromValue(_index), ConvertToType< MyGUI::Any >::ConvertFromValue(_data) );
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
