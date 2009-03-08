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

		public ref class TabItem : public Widget
		{

		private:
			typedef MyGUI::TabItem ThisType;

		public:
			TabItem() : Widget() { }

		internal:
			TabItem( MyGUI::TabItem* _native ) : Widget(_native) { }
			TabItem( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew TabItem(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   	public:
		void RemoveItem( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItem( );
		}



   	public:
		void SetItemSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemSelected( );
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
		property Convert<const Ogre::UTFString &>::Type ItemName
		{
			Convert<const Ogre::UTFString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const Ogre::UTFString &>::To( static_cast<ThisType*>(mNative)->getItemName() );
			}
			void set(Convert<const Ogre::UTFString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemName( Convert<const Ogre::UTFString &>::From(_value) );
			}
		}
	


   	public:
		property Convert<int>::Type ButtonWidth
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getButtonWidth() );
			}
			void set(Convert<int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setButtonWidth( Convert<int>::From(_value) );
			}
		}
	


   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
