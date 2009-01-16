/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"
#include "MMyGUI_Edit.h"

namespace MMyGUI
{

	public ref class ComboBox : public Edit
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( ComboBox, Edit );

		//InsertPoint

   	public:
		delegate void HandleComboChangePosition( Convert<MyGUI::Widget *>::Type _value1, Convert<size_t>::Type _value2 );
		event HandleComboChangePosition^ ComboChangePosition
		{
			void add(HandleComboChangePosition^ _value)
			{
				mDelegateComboChangePosition += _value;
				static_cast<ThisType*>(mNative)->eventComboChangePosition = new Delegate2< HandleComboChangePosition^, MyGUI::Widget *, size_t >(mDelegateComboChangePosition);
			}
			void remove(HandleComboChangePosition^ _value)
			{
				mDelegateComboChangePosition -= _value;
				if (mDelegateComboChangePosition == nullptr)
					static_cast<ThisType*>(mNative)->eventComboChangePosition = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventComboChangePosition = new Delegate2< HandleComboChangePosition^, MyGUI::Widget *, size_t >(mDelegateComboChangePosition);
			}
		}
	private:
		HandleComboChangePosition^ mDelegateComboChangePosition;






   	public:
		property Convert<size_t>::Type MaxListHeight
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getMaxListHeight() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMaxListHeight( Convert<size_t>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type SmoothShow
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getSmoothShow() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setSmoothShow( Convert<bool>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type ComboModeDrop
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getComboModeDrop() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setComboModeDrop( Convert<bool>::From(_value) );
			}
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
		void BeginToItemAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->beginToItemAt( Convert<size_t>::From(_index) );
		}



   	public:
		Convert<const Ogre::UTFString &>::Type GetItemNameAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const Ogre::UTFString &>::To( static_cast<ThisType*>(mNative)->getItemNameAt( Convert<size_t>::From(_index) ) );
		}



   	public:
		void SetItemNameAt( Convert<size_t>::Type _index, Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemNameAt( Convert<size_t>::From(_index), Convert<const Ogre::UTFString &>::From(_name) );
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
		void ClearItemSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearItemSelected( );
		}



   	public:
		void SetItemSelectedAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemSelectedAt( Convert<size_t>::From(_index) );
		}



   	public:
		Convert<size_t>::Type GetItemIndexSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getItemIndexSelected( ) );
		}



   	public:
		Convert<size_t>::Type FindItemIndexWith( Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->findItemIndexWith( Convert<const Ogre::UTFString &>::From(_name) ) );
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
		void AddItem( Convert<const Ogre::UTFString &>::Type _name, Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem( Convert<const Ogre::UTFString &>::From(_name), Convert< MyGUI::Any >::From(_data) );
		}


   	public:
		void AddItem( Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addItem( Convert<const Ogre::UTFString &>::From(_name) );
		}



   	public:
		void InsertItemAt( Convert<size_t>::Type _index, Convert<const Ogre::UTFString &>::Type _name, Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt( Convert<size_t>::From(_index), Convert<const Ogre::UTFString &>::From(_name), Convert< MyGUI::Any >::From(_data) );
		}


   	public:
		void InsertItemAt( Convert<size_t>::Type _index, Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertItemAt( Convert<size_t>::From(_index), Convert<const Ogre::UTFString &>::From(_name) );
		}



   	public:
		Convert<size_t>::Type GetItemCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getItemCount( ) );
		}



	};

} // namespace MMyGUI
