/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_Widget.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class MultiListBox : public Widget
		{

		private:
			typedef MyGUI::MultiList ThisType;

		public:
			MultiListBox() : Widget() { }

		internal:
			MultiListBox( MyGUI::MultiList* _native ) : Widget(_native) { }
			MultiListBox( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew MultiListBox(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint



   	public:
		delegate void HandleListChangePosition(
			Convert<MyGUI::MultiList *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleListChangePosition^ EventListChangePosition
		{
			void add(HandleListChangePosition^ _value)
			{
				mDelegateListChangePosition += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventListChangePosition =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::MultiList * ,
						size_t > *>(
							new Delegate2< HandleListChangePosition^ ,
							MyGUI::MultiList * ,
							size_t >(mDelegateListChangePosition) );
			}
			void remove(HandleListChangePosition^ _value)
			{
				mDelegateListChangePosition -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateListChangePosition == nullptr)
					static_cast<ThisType*>(mNative)->eventListChangePosition = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListChangePosition =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::MultiList * ,
							size_t > *>(
								new Delegate2< HandleListChangePosition^ ,
									MyGUI::MultiList * ,
									size_t >(mDelegateListChangePosition) );
			}
		}
	private:
		HandleListChangePosition^ mDelegateListChangePosition;



   	public:
		delegate void HandleListSelectAccept(
			Convert<MyGUI::MultiList *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleListSelectAccept^ EventListSelectAccept
		{
			void add(HandleListSelectAccept^ _value)
			{
				mDelegateListSelectAccept += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventListSelectAccept =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::MultiList * ,
						size_t > *>(
							new Delegate2< HandleListSelectAccept^ ,
							MyGUI::MultiList * ,
							size_t >(mDelegateListSelectAccept) );
			}
			void remove(HandleListSelectAccept^ _value)
			{
				mDelegateListSelectAccept -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateListSelectAccept == nullptr)
					static_cast<ThisType*>(mNative)->eventListSelectAccept = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventListSelectAccept =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::MultiList * ,
							size_t > *>(
								new Delegate2< HandleListSelectAccept^ ,
									MyGUI::MultiList * ,
									size_t >(mDelegateListSelectAccept) );
			}
		}
	private:
		HandleListSelectAccept^ mDelegateListSelectAccept;



   	public:
		Convert<MyGUI::Any>::Type GetSubItemDataAt(
			Convert<size_t>::Type _column ,
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			ObjectHolder* data = static_cast<ThisType*>(mNative)->getSubItemDataAt< ObjectHolder >(
					Convert<size_t>::From(_column) ,
					Convert<size_t>::From(_index) , false );
			return data ? data->toObject() : nullptr;
		}



   	public:
		void ClearSubItemDataAt(
			Convert<size_t>::Type _column ,
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearSubItemDataAt(
				Convert<size_t>::From(_column) ,
				Convert<size_t>::From(_index) );
		}



   	public:
		void SetSubItemDataAt(
			Convert<size_t>::Type _column ,
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSubItemDataAt(
				Convert<size_t>::From(_column) ,
				Convert<size_t>::From(_index) ,
				Convert<MyGUI::Any>::From(_data) );
		}



   	public:
		Convert<size_t>::Type FindSubItemWith(
			Convert<size_t>::Type _column ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->findSubItemWith(
					Convert<size_t>::From(_column) ,
					Convert<const MyGUI::UString &>::From(_name) ) );
		}



   	public:
		Convert<const MyGUI::UString &>::Type GetSubItemNameAt(
			Convert<size_t>::Type _column ,
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::UString &>::To(
				static_cast<ThisType*>(mNative)->getSubItemNameAt(
					Convert<size_t>::From(_column) ,
					Convert<size_t>::From(_index) ) );
		}



   	public:
		void SetSubItemNameAt(
			Convert<size_t>::Type _column ,
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSubItemNameAt(
				Convert<size_t>::From(_column) ,
				Convert<size_t>::From(_index) ,
				Convert<const MyGUI::UString &>::From(_name) );
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
		Convert<const MyGUI::UString &>::Type GetItemNameAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::UString &>::To(
				static_cast<ThisType*>(mNative)->getItemNameAt(
					Convert<size_t>::From(_index) ) );
		}



   	public:
		void SetItemNameAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemNameAt(
				Convert<size_t>::From(_index) ,
				Convert<const MyGUI::UString &>::From(_name) );
		}



   	public:
		void SwapItemsAt(
			Convert<size_t>::Type _index1 ,
			Convert<size_t>::Type _index2 )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->swapItemsAt(
				Convert<size_t>::From(_index1) ,
				Convert<size_t>::From(_index2) );
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
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem(
				Convert<const MyGUI::UString &>::From(_name) ,
				Convert<MyGUI::Any>::From(_data) );
		}

		void AddItem(
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem(
				Convert<const MyGUI::UString &>::From(_name) );
		}



   	public:
		void InsertItemAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt(
				Convert<size_t>::From(_index) ,
				Convert<const MyGUI::UString &>::From(_name) ,
				Convert<MyGUI::Any>::From(_data) );
		}

		void InsertItemAt(
			Convert<size_t>::Type _index ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt(
				Convert<size_t>::From(_index) ,
				Convert<const MyGUI::UString &>::From(_name) );
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
	


   	public:
		Convert<MyGUI::Any>::Type GetColumnDataAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			ObjectHolder* data = static_cast<ThisType*>(mNative)->getColumnDataAt< ObjectHolder >(
				Convert<size_t>::From(_index) , false );
			return data ? data->toObject() : nullptr;
		}



   	public:
		void ClearColumnDataAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearColumnDataAt(
				Convert<size_t>::From(_index) );
		}



   	public:
		void SetColumnDataAt(
			Convert<size_t>::Type _index ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnDataAt(
				Convert<size_t>::From(_index) ,
				Convert<MyGUI::Any>::From(_data) );
		}



   	public:
		void SortByColumn(
			Convert<size_t>::Type _column ,
			Convert<bool>::Type _backward )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->sortByColumn(
				Convert<size_t>::From(_column) ,
				Convert<bool>::From(_backward) );
		}



   	public:
		Convert<int>::Type GetColumnWidthAt(
			Convert<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getColumnWidthAt(
					Convert<size_t>::From(_column) ) );
		}



   	public:
		Convert<const MyGUI::UString &>::Type GetColumnNameAt(
			Convert<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::UString &>::To(
				static_cast<ThisType*>(mNative)->getColumnNameAt(
					Convert<size_t>::From(_column) ) );
		}



   	public:
		void SetColumnWidthAt(
			Convert<size_t>::Type _column ,
			Convert<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnWidthAt(
				Convert<size_t>::From(_column) ,
				Convert<int>::From(_width) );
		}



   	public:
		void SetColumnNameAt(
			Convert<size_t>::Type _column ,
			Convert<const MyGUI::UString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColumnNameAt(
				Convert<size_t>::From(_column) ,
				Convert<const MyGUI::UString &>::From(_name) );
		}



   	public:
		void RemoveAllColumns( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeAllColumns( );
		}



   	public:
		void RemoveColumnAt(
			Convert<size_t>::Type _column )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeColumnAt(
				Convert<size_t>::From(_column) );
		}



   	public:
		void AddColumn(
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<int>::Type _width ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addColumn(
				Convert<const MyGUI::UString &>::From(_name) ,
				Convert<int>::From(_width) ,
				Convert<MyGUI::Any>::From(_data) );
		}

		void AddColumn(
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addColumn(
				Convert<const MyGUI::UString &>::From(_name) ,
				Convert<int>::From(_width) );
		}



   	public:
		void InsertColumnAt(
			Convert<size_t>::Type _column ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<int>::Type _width ,
			Convert<MyGUI::Any>::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertColumnAt(
				Convert<size_t>::From(_column) ,
				Convert<const MyGUI::UString &>::From(_name) ,
				Convert<int>::From(_width) ,
				Convert<MyGUI::Any>::From(_data) );
		}

		void InsertColumnAt(
			Convert<size_t>::Type _column ,
			Convert<const MyGUI::UString &>::Type _name ,
			Convert<int>::Type _width )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertColumnAt(
				Convert<size_t>::From(_column) ,
				Convert<const MyGUI::UString &>::From(_name) ,
				Convert<int>::From(_width) );
		}



   	public:
		property Convert<size_t>::Type ColumnCount
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getColumnCount() );
			}
		}
	


   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
