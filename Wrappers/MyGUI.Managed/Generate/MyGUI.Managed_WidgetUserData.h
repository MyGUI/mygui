/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_WidgetCropped.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class WidgetUserData abstract : public WidgetCropped
		{

		private:
			typedef MyGUI::Widget ThisType;

		public:
			WidgetUserData() : WidgetCropped() { }

		internal:
			WidgetUserData( MyGUI::Widget* _native ) : WidgetCropped(_native) { }

			//InsertPoint

   


   


   	public:
		void ClearUserStrings( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearUserStrings( );
		}



   	public:
		Convert<bool>::Type IsUserString(
			Convert<const std::string &>::Type _key )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->isUserString(
					Convert<const std::string &>::From(_key) ) );
		}



   	public:
		Convert<bool>::Type ClearUserString(
			Convert<const std::string &>::Type _key )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->clearUserString(
					Convert<const std::string &>::From(_key) ) );
		}



   	public:
		Convert<const std::string &>::Type GetUserString(
			Convert<const std::string &>::Type _key )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To(
				static_cast<ThisType*>(mNative)->getUserString(
					Convert<const std::string &>::From(_key) ) );
		}



   	public:
		void SetUserString(
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setUserString(
				Convert<const std::string &>::From(_key) ,
				Convert<const std::string &>::From(_value) );
		}



		};

	} // namespace Managed
} // namespace MyGUI
