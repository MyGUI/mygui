/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI_Managed_DDContainer.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class ItemBox : public DDContainer
		{

		private:
			typedef MyGUI::ItemBox ThisType;

		public:
			ItemBox() : DDContainer() { }

		internal:
			ItemBox( MyGUI::ItemBox* _native ) : DDContainer(_native) { }
			ItemBox( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew ItemBox(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

	public:
		delegate void HandleNotifyItem(
			Convert<MyGUI::ItemBox *>::Type _sender ,
			Convert<const MyGUI::IBNotifyItemData &>::Type _info );
		event HandleNotifyItem^ EventNotifyItem
		{
			void add(HandleNotifyItem^ _value)
			{
				bool empty = mDelegateNotifyItem == nullptr;

				mDelegateNotifyItem += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventNotifyItem +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ItemBox * ,
							const MyGUI::IBNotifyItemData & > *>(
								new Delegate2< HandleNotifyItem^ ,
								MyGUI::ItemBox * ,
								const MyGUI::IBNotifyItemData & >(mDelegateNotifyItem) );
			}
			void remove(HandleNotifyItem^ _value)
			{
				mDelegateNotifyItem -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateNotifyItem == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventNotifyItem -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ItemBox * ,
							const MyGUI::IBNotifyItemData & > *>(
								new Delegate2< HandleNotifyItem^ ,
									MyGUI::ItemBox * ,
									const MyGUI::IBNotifyItemData & >(mDelegateNotifyItem) );
			}
		}
	private:
		HandleNotifyItem^ mDelegateNotifyItem;
	public:
		delegate void HandleMouseItemActivate(
			Convert<MyGUI::ItemBox *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleMouseItemActivate^ EventMouseItemActivate
		{
			void add(HandleMouseItemActivate^ _value)
			{
				bool empty = mDelegateMouseItemActivate == nullptr;

				mDelegateMouseItemActivate += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseItemActivate +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ItemBox * ,
							size_t > *>(
								new Delegate2< HandleMouseItemActivate^ ,
								MyGUI::ItemBox * ,
								size_t >(mDelegateMouseItemActivate) );
			}
			void remove(HandleMouseItemActivate^ _value)
			{
				mDelegateMouseItemActivate -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMouseItemActivate == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseItemActivate -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ItemBox * ,
							size_t > *>(
								new Delegate2< HandleMouseItemActivate^ ,
									MyGUI::ItemBox * ,
									size_t >(mDelegateMouseItemActivate) );
			}
		}
	private:
		HandleMouseItemActivate^ mDelegateMouseItemActivate;
	public:
		delegate void HandleChangeItemPosition(
			Convert<MyGUI::ItemBox *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleChangeItemPosition^ EventChangeItemPosition
		{
			void add(HandleChangeItemPosition^ _value)
			{
				bool empty = mDelegateChangeItemPosition == nullptr;

				mDelegateChangeItemPosition += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventChangeItemPosition +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ItemBox * ,
							size_t > *>(
								new Delegate2< HandleChangeItemPosition^ ,
								MyGUI::ItemBox * ,
								size_t >(mDelegateChangeItemPosition) );
			}
			void remove(HandleChangeItemPosition^ _value)
			{
				mDelegateChangeItemPosition -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateChangeItemPosition == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventChangeItemPosition -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ItemBox * ,
							size_t > *>(
								new Delegate2< HandleChangeItemPosition^ ,
									MyGUI::ItemBox * ,
									size_t >(mDelegateChangeItemPosition) );
			}
		}
	private:
		HandleChangeItemPosition^ mDelegateChangeItemPosition;
	public:
		delegate void HandleSelectItemAccept(
			Convert<MyGUI::ItemBox *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleSelectItemAccept^ EventSelectItemAccept
		{
			void add(HandleSelectItemAccept^ _value)
			{
				bool empty = mDelegateSelectItemAccept == nullptr;

				mDelegateSelectItemAccept += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventSelectItemAccept +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ItemBox * ,
							size_t > *>(
								new Delegate2< HandleSelectItemAccept^ ,
								MyGUI::ItemBox * ,
								size_t >(mDelegateSelectItemAccept) );
			}
			void remove(HandleSelectItemAccept^ _value)
			{
				mDelegateSelectItemAccept -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateSelectItemAccept == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventSelectItemAccept -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ItemBox * ,
							size_t > *>(
								new Delegate2< HandleSelectItemAccept^ ,
									MyGUI::ItemBox * ,
									size_t >(mDelegateSelectItemAccept) );
			}
		}
	private:
		HandleSelectItemAccept^ mDelegateSelectItemAccept;
	public:
		delegate void HandleDrawItem(
			Convert<MyGUI::ItemBox *>::Type _sender ,
			Convert<MyGUI::Widget *>::Type _item ,
			Convert<const MyGUI::IBDrawItemInfo &>::Type _info );
		event HandleDrawItem^ RequestDrawItem
		{
			void add(HandleDrawItem^ _value)
			{
				mDelegateDrawItem += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->requestDrawItem =
					static_cast< MyGUI::delegates::IDelegate3<
						MyGUI::ItemBox * ,
						MyGUI::Widget * ,
						const MyGUI::IBDrawItemInfo & > *>(
							new Delegate3< HandleDrawItem^ ,
							MyGUI::ItemBox * ,
							MyGUI::Widget * ,
							const MyGUI::IBDrawItemInfo & >(mDelegateDrawItem) );
			}
			void remove(HandleDrawItem^ _value)
			{
				mDelegateDrawItem -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateDrawItem == nullptr)
					static_cast<ThisType*>(mNative)->requestDrawItem = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestDrawItem =
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::ItemBox * ,
							MyGUI::Widget * ,
							const MyGUI::IBDrawItemInfo & > *>(
								new Delegate3< HandleDrawItem^ ,
									MyGUI::ItemBox * ,
									MyGUI::Widget * ,
									const MyGUI::IBDrawItemInfo & >(mDelegateDrawItem) );
			}
		}
	private:
		HandleDrawItem^ mDelegateDrawItem;
	public:
		delegate void HandleCoordItem(
			Convert<MyGUI::ItemBox *>::Type _sender ,
			Convert<MyGUI::types::TCoord < int > &>::Type _coord ,
			Convert<bool>::Type _drag );
		event HandleCoordItem^ RequestCoordItem
		{
			void add(HandleCoordItem^ _value)
			{
				mDelegateCoordItem += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->requestCoordItem =
					static_cast< MyGUI::delegates::IDelegate3<
						MyGUI::ItemBox * ,
						MyGUI::types::TCoord < int > & ,
						bool > *>(
							new Delegate3< HandleCoordItem^ ,
							MyGUI::ItemBox * ,
							MyGUI::types::TCoord < int > & ,
							bool >(mDelegateCoordItem) );
			}
			void remove(HandleCoordItem^ _value)
			{
				mDelegateCoordItem -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateCoordItem == nullptr)
					static_cast<ThisType*>(mNative)->requestCoordItem = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestCoordItem =
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::ItemBox * ,
							MyGUI::types::TCoord < int > & ,
							bool > *>(
								new Delegate3< HandleCoordItem^ ,
									MyGUI::ItemBox * ,
									MyGUI::types::TCoord < int > & ,
									bool >(mDelegateCoordItem) );
			}
		}
	private:
		HandleCoordItem^ mDelegateCoordItem;
	public:
		delegate void HandleCreateWidgetItem(
			Convert<MyGUI::ItemBox *>::Type _sender ,
			Convert<MyGUI::Widget *>::Type _item );
		event HandleCreateWidgetItem^ RequestCreateWidgetItem
		{
			void add(HandleCreateWidgetItem^ _value)
			{
				mDelegateCreateWidgetItem += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->requestCreateWidgetItem =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::ItemBox * ,
						MyGUI::Widget * > *>(
							new Delegate2< HandleCreateWidgetItem^ ,
							MyGUI::ItemBox * ,
							MyGUI::Widget * >(mDelegateCreateWidgetItem) );
			}
			void remove(HandleCreateWidgetItem^ _value)
			{
				mDelegateCreateWidgetItem -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateCreateWidgetItem == nullptr)
					static_cast<ThisType*>(mNative)->requestCreateWidgetItem = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestCreateWidgetItem =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ItemBox * ,
							MyGUI::Widget * > *>(
								new Delegate2< HandleCreateWidgetItem^ ,
									MyGUI::ItemBox * ,
									MyGUI::Widget * >(mDelegateCreateWidgetItem) );
			}
		}
	private:
		HandleCreateWidgetItem^ mDelegateCreateWidgetItem;
	public:
		void SetCoord(
			Convert<int>::Type _left ,
			Convert<int>::Type _top ,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord(
				Convert<int>::From(_left) ,
				Convert<int>::From(_top) ,
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	public:
		void SetSize(
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	public:
		void SetPosition(
			Convert<int>::Type _left ,
			Convert<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition(
				Convert<int>::From(_left) ,
				Convert<int>::From(_top) );
		}
	public:
		void ResetDrag( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->resetDrag( );
		}
	public:
		Convert<MyGUI::Widget *>::Type GetWidgetByIndex(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::Widget *>::To(
				static_cast<ThisType*>(mNative)->getWidgetByIndex(
					Convert<size_t>::From(_index) ) );
		}
	public:
		Convert<size_t>::Type GetIndexByWidget(
			Convert<MyGUI::Widget *>::Type _widget )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getIndexByWidget(
					Convert<MyGUI::Widget *>::From(_widget) ) );
		}
	public:
		Convert<MyGUI::Any>::Type GetItemDataAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			ObjectHolder* data = static_cast<ThisType*>(mNative)->getItemDataAt< ObjectHolder >(
				Convert<size_t>::From(_index) , false );
			return data ? data->toObject() : nullptr;
		}
	public:
		void ClearItemDataAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemDataAt(
				Convert<size_t>::From(_index) );
		}
	public:
		void SetItemDataAt(
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemDataAt(
				Convert<size_t>::From(_index) ,
				Convert<MyGUI::Any>::From(_data) );
		}
	public:
		void ClearIndexSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearIndexSelected( );
		}
	public:
		void RedrawAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->redrawAllItems( );
		}
	public:
		void RedrawItemAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->redrawItemAt(
				Convert<size_t>::From(_index) );
		}
	public:
		void RemoveAllItems( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllItems( );
		}
	public:
		void RemoveItemAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItemAt(
				Convert<size_t>::From(_index) );
		}
	public:
		void AddItem(
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem(
				Convert<MyGUI::Any>::From(_data) );
		}

		void AddItem( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem( );
		}
	public:
		void InsertItemAt(
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt(
				Convert<size_t>::From(_index) ,
				Convert<MyGUI::Any>::From(_data) );
		}

		void InsertItemAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt(
				Convert<size_t>::From(_index) );
		}
	public:
		property Convert<MyGUI::Widget *>::Type WidgetDrag
		{
			Convert<MyGUI::Widget *>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::Widget *>::To( static_cast<ThisType*>(mNative)->getWidgetDrag() );
			}
		}
	
	public:
		property Convert<bool>::Type VerticalAlignment
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getVerticalAlignment() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVerticalAlignment( Convert<bool>::From(_value) );
			}
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
		property Convert<size_t>::Type ItemCount
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getItemCount() );
			}
		}
	
		};

	} // namespace Managed
} // namespace MyGUI
