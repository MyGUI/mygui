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



		};

	} // namespace Managed
} // namespace MyGUI
