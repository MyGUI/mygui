/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"


namespace MyGUI
{
	namespace Managed
	{

		public ref class TabItem : public Widget
		{

			//--------------------------------------------------------------------
			// объявление типов и конструкторов
			MMYGUI_DECLARE_DERIVED( TabItem, TabItem, Widget );

			#include "../MMyGUI_GetItemData.h"

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
		void SetItemData( Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemData( Convert< MyGUI::Any >::From(_data) );
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
