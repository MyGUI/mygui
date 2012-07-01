/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI_Managed_Widget.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class MenuItem : public Widget
		{

		private:
			typedef MyGUI::MenuItem ThisType;

		public:
			MenuItem() : Widget() { }

		internal:
			MenuItem( MyGUI::MenuItem* _native ) : Widget(_native) { }
			MenuItem( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew MenuItem(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

	public:
		void SetItemChildVisible(
			Convert<bool>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemChildVisible(
				Convert<bool>::From(_value) );
		}
	public:
		Convert<MyGUI::MenuControl *>::Type CreateItemChild( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuControl *>::To(
				static_cast<ThisType*>(mNative)->createItemChild( ) );
		}
	public:
		void RemoveItem( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItem( );
		}
	public:
		property Convert<MyGUI::Any>::Type ItemData
		{
			Convert<MyGUI::Any>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				ObjectHolder* data = static_cast<ThisType*>(mNative)->getItemData< ObjectHolder >(false);
				return data ? data->toObject() : nullptr;
			}
			void set(Convert<MyGUI::Any>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemData( Convert<MyGUI::Any>::From(_value) );
			}
		}
	
	public:
		property Convert<bool>::Type ItemChecked
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getItemChecked() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemChecked( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<MyGUI::MenuControl *>::Type ItemChild
		{
			Convert<MyGUI::MenuControl *>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::MenuControl *>::To( static_cast<ThisType*>(mNative)->getItemChild() );
			}
		}
	
	public:
		property Convert<MyGUI::MenuControl *>::Type MenuCtrlParent
		{
			Convert<MyGUI::MenuControl *>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::MenuControl *>::To( static_cast<ThisType*>(mNative)->getMenuCtrlParent() );
			}
		}
	
	public:
		property Convert<MyGUI::MenuItemType>::Type ItemType
		{
			Convert<MyGUI::MenuItemType>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::MenuItemType>::To( static_cast<ThisType*>(mNative)->getItemType() );
			}
			void set(Convert<MyGUI::MenuItemType>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemType( Convert<MyGUI::MenuItemType>::From(_value) );
			}
		}
	
	public:
		property Convert<size_t>::Type ItemIndex
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getItemIndex() );
			}
		}
	
	public:
		property Convert<const std::string &>::Type ItemId
		{
			Convert<const std::string &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getItemId() );
			}
			void set(Convert<const std::string &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemId( Convert<const std::string &>::From(_value) );
			}
		}
	
	public:
		property Convert<const MyGUI::UString &>::Type ItemName
		{
			Convert<const MyGUI::UString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::UString &>::To( static_cast<ThisType*>(mNative)->getItemName() );
			}
			void set(Convert<const MyGUI::UString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemName( Convert<const MyGUI::UString &>::From(_value) );
			}
		}
	
		};

	} // namespace Managed
} // namespace MyGUI
