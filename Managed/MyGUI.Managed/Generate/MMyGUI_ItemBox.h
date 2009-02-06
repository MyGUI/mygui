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

		#include "../MMyGUI_GetItemData.h"

		//InsertPoint

   	public:
		delegate void HandleNotifyItem( Convert<MyGUI::ItemBox *>::Type _sender, Convert<const MyGUI::NotifyItemData &>::Type _info );
		event HandleNotifyItem^ EventNotifyItem
		{
			void add(HandleNotifyItem^ _value)
			{
				mDelegateNotifyItem += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventNotifyItem =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::ItemBox *, const MyGUI::NotifyItemData & > *>(
						new Delegate2< HandleNotifyItem^, MyGUI::ItemBox *, const MyGUI::NotifyItemData & >(mDelegateNotifyItem) );
			}
			void remove(HandleNotifyItem^ _value)
			{
				mDelegateNotifyItem -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateNotifyItem == nullptr)
					static_cast<ThisType*>(mNative)->eventNotifyItem = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventNotifyItem =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::ItemBox *, const MyGUI::NotifyItemData & > *>(
							new Delegate2< HandleNotifyItem^, MyGUI::ItemBox *, const MyGUI::NotifyItemData & >(mDelegateNotifyItem) );
			}
		}
	private:
		HandleNotifyItem^ mDelegateNotifyItem;




   	public:
		delegate void HandleMouseItemActivate( Convert<MyGUI::ItemBox *>::Type _sender, Convert<size_t>::Type _index );
		event HandleMouseItemActivate^ EventMouseItemActivate
		{
			void add(HandleMouseItemActivate^ _value)
			{
				mDelegateMouseItemActivate += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMouseItemActivate =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::ItemBox *, size_t > *>(
						new Delegate2< HandleMouseItemActivate^, MyGUI::ItemBox *, size_t >(mDelegateMouseItemActivate) );
			}
			void remove(HandleMouseItemActivate^ _value)
			{
				mDelegateMouseItemActivate -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMouseItemActivate == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseItemActivate = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseItemActivate =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::ItemBox *, size_t > *>(
							new Delegate2< HandleMouseItemActivate^, MyGUI::ItemBox *, size_t >(mDelegateMouseItemActivate) );
			}
		}
	private:
		HandleMouseItemActivate^ mDelegateMouseItemActivate;




   	public:
		delegate void HandleChangeItemPosition( Convert<MyGUI::ItemBox *>::Type _sender, Convert<size_t>::Type _index );
		event HandleChangeItemPosition^ EventChangeItemPosition
		{
			void add(HandleChangeItemPosition^ _value)
			{
				mDelegateChangeItemPosition += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventChangeItemPosition =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::ItemBox *, size_t > *>(
						new Delegate2< HandleChangeItemPosition^, MyGUI::ItemBox *, size_t >(mDelegateChangeItemPosition) );
			}
			void remove(HandleChangeItemPosition^ _value)
			{
				mDelegateChangeItemPosition -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateChangeItemPosition == nullptr)
					static_cast<ThisType*>(mNative)->eventChangeItemPosition = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventChangeItemPosition =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::ItemBox *, size_t > *>(
							new Delegate2< HandleChangeItemPosition^, MyGUI::ItemBox *, size_t >(mDelegateChangeItemPosition) );
			}
		}
	private:
		HandleChangeItemPosition^ mDelegateChangeItemPosition;




   	public:
		delegate void HandleSelectItemAccept( Convert<MyGUI::ItemBox *>::Type _sender, Convert<size_t>::Type _index );
		event HandleSelectItemAccept^ EventSelectItemAccept
		{
			void add(HandleSelectItemAccept^ _value)
			{
				mDelegateSelectItemAccept += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventSelectItemAccept =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::ItemBox *, size_t > *>(
						new Delegate2< HandleSelectItemAccept^, MyGUI::ItemBox *, size_t >(mDelegateSelectItemAccept) );
			}
			void remove(HandleSelectItemAccept^ _value)
			{
				mDelegateSelectItemAccept -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateSelectItemAccept == nullptr)
					static_cast<ThisType*>(mNative)->eventSelectItemAccept = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventSelectItemAccept =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::ItemBox *, size_t > *>(
							new Delegate2< HandleSelectItemAccept^, MyGUI::ItemBox *, size_t >(mDelegateSelectItemAccept) );
			}
		}
	private:
		HandleSelectItemAccept^ mDelegateSelectItemAccept;




	public:
		delegate void HandleUpdateWidgetItem( Convert<MyGUI::ItemBox *>::Type _sender, Convert<MyGUI::Widget *>::Type _item, Convert<const MyGUI::ItemInfo &>::Type _info );
		event HandleUpdateWidgetItem^ RequestUpdateWidgetItem
		{
			void add(HandleUpdateWidgetItem^ _value)
			{
				mDelegateUpdateWidgetItem += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->requestUpdateWidgetItem =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::ItemBox *, MyGUI::Widget *, const MyGUI::ItemInfo & > *>(
						new Delegate3< HandleUpdateWidgetItem^, MyGUI::ItemBox *, MyGUI::Widget *, const MyGUI::ItemInfo & >(mDelegateUpdateWidgetItem) );
			}
			void remove(HandleUpdateWidgetItem^ _value)
			{
				mDelegateUpdateWidgetItem -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateUpdateWidgetItem == nullptr)
					static_cast<ThisType*>(mNative)->requestUpdateWidgetItem = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestUpdateWidgetItem =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::ItemBox *, MyGUI::Widget *, const MyGUI::ItemInfo & > *>(
							new Delegate3< HandleUpdateWidgetItem^, MyGUI::ItemBox *, MyGUI::Widget *, const MyGUI::ItemInfo & >(mDelegateUpdateWidgetItem) );
			}
		}
	private:
		HandleUpdateWidgetItem^ mDelegateUpdateWidgetItem;




	public:
		delegate void HandleCoordWidgetItem( Convert<MyGUI::ItemBox *>::Type _sender, Convert<MyGUI::types::TCoord< int > &>::Type _coord, Convert<bool>::Type _drop );
		event HandleCoordWidgetItem^ RequestCoordWidgetItem
		{
			void add(HandleCoordWidgetItem^ _value)
			{
				mDelegateCoordWidgetItem += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->requestCoordWidgetItem =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::ItemBox *, MyGUI::types::TCoord< int > &, bool > *>(
						new Delegate3< HandleCoordWidgetItem^, MyGUI::ItemBox *, MyGUI::types::TCoord< int > &, bool >(mDelegateCoordWidgetItem) );
			}
			void remove(HandleCoordWidgetItem^ _value)
			{
				mDelegateCoordWidgetItem -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateCoordWidgetItem == nullptr)
					static_cast<ThisType*>(mNative)->requestCoordWidgetItem = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestCoordWidgetItem =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::ItemBox *, MyGUI::types::TCoord< int > &, bool > *>(
							new Delegate3< HandleCoordWidgetItem^, MyGUI::ItemBox *, MyGUI::types::TCoord< int > &, bool >(mDelegateCoordWidgetItem) );
			}
		}
	private:
		HandleCoordWidgetItem^ mDelegateCoordWidgetItem;




   	public:
		delegate void HandleCreateWidgetItem( Convert<MyGUI::ItemBox *>::Type _sender, Convert<MyGUI::Widget *>::Type _item );
		event HandleCreateWidgetItem^ RequestCreateWidgetItem
		{
			void add(HandleCreateWidgetItem^ _value)
			{
				mDelegateCreateWidgetItem += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->requestCreateWidgetItem =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::ItemBox *, MyGUI::Widget * > *>(
						new Delegate2< HandleCreateWidgetItem^, MyGUI::ItemBox *, MyGUI::Widget * >(mDelegateCreateWidgetItem) );
			}
			void remove(HandleCreateWidgetItem^ _value)
			{
				mDelegateCreateWidgetItem -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateCreateWidgetItem == nullptr)
					static_cast<ThisType*>(mNative)->requestCreateWidgetItem = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestCreateWidgetItem =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::ItemBox *, MyGUI::Widget * > *>(
							new Delegate2< HandleCreateWidgetItem^, MyGUI::ItemBox *, MyGUI::Widget * >(mDelegateCreateWidgetItem) );
			}
		}
	private:
		HandleCreateWidgetItem^ mDelegateCreateWidgetItem;




   	public:
		void ResetDrag( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->resetDrag( );
		}



   	public:
		Convert< MyGUI::Widget * >::Type GetWidgetByIndex( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::Widget * >::To( static_cast<ThisType*>(mNative)->getWidgetByIndex( Convert<size_t>::From(_index) ) );
		}



   	public:
		Convert< MyGUI::Widget * >::Type GetWidgetDrag( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::Widget * >::To( static_cast<ThisType*>(mNative)->getWidgetDrag( ) );
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
