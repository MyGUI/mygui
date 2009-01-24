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
		delegate void HandleNotifyItem( Convert<MyGUI::Widget *>::Type _sender, Convert<const MyGUI::NotifyItemData &>::Type _info );
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
		delegate void HandleMouseItemActivate( Convert<MyGUI::Widget *>::Type _sender, Convert<size_t>::Type _index );
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
		delegate void HandleChangeItemPosition( Convert<MyGUI::Widget *>::Type _sender, Convert<size_t>::Type _index );
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
		delegate void HandleSelectItemAccept( Convert<MyGUI::Widget *>::Type _sender, Convert<size_t>::Type _index );
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
		delegate void HandleUpdateWidgetItem( Convert<MyGUI::Widget *>::Type _sender, Convert<MyGUI::Widget *>::Type _item, Convert<const MyGUI::ItemInfo &>::Type _info );
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
		delegate void HandleCoordWidgetItem( Convert<MyGUI::Widget *>::Type _sender, Convert<MyGUI::types::TCoord< int > &>::Type _coord, Convert<bool>::Type _drop );
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
		delegate void HandleCreateWidgetItem( Convert<MyGUI::Widget *>::Type _sender, Convert<MyGUI::Widget *>::Type _item );
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
		void ResetDrop( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->resetDrop( );
		}



   	public:
		Convert< MyGUI::Widget * >::Type GetWidgetByIndex( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::Widget * >::To( static_cast<ThisType*>(mNative)->getWidgetByIndex( Convert<size_t>::From(_index) ) );
		}



   	public:
		Convert< MyGUI::Widget * >::Type GetWidgetDrop( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::Widget * >::To( static_cast<ThisType*>(mNative)->getWidgetDrop( ) );
		}



   	public:
		Convert<size_t>::Type GetIndexByWidget( Convert< MyGUI::Widget * >::Type _widget )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getIndexByWidget( Convert< MyGUI::Widget * >::From(_widget) ) );
		}



   	public:
		property Convert<bool>::Type ItemBoxAlignVert
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getItemBoxAlignVert() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemBoxAlignVert( Convert<bool>::From(_value) );
			}
		}



   	public:
		void ClearItemDataAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemDataAt( Convert<size_t>::From(_index) );
		}



   	public:
		void SetItemDataAt( Convert<size_t>::Type _index, Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemDataAt( Convert<size_t>::From(_index), Convert< MyGUI::Any >::From(_data) );
		}



   	public:
		void ClearIndexSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearIndexSelected( );
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
		void RedrawAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->redrawAllItems( );
		}



   	public:
		void RedrawItemAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->redrawItemAt( Convert<size_t>::From(_index) );
		}



   	public:
		void RemoveAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllItems( );
		}



   	public:
		void RemoveItemAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemAt( Convert<size_t>::From(_index) );
		}



   	public:
		void AddItem( Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem( Convert< MyGUI::Any >::From(_data) );
		}


   	public:
		void AddItem( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem( );
		}



   	public:
		void InsertItemAt( Convert<size_t>::Type _index, Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt( Convert<size_t>::From(_index), Convert< MyGUI::Any >::From(_data) );
		}


   	public:
		void InsertItemAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt( Convert<size_t>::From(_index) );
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

} // namespace MMyGUI
