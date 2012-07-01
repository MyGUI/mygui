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
		void SetButtonWidth(
			Convert<int>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setButtonWidth(
				Convert<int>::From(_value) );
		}
		};

	} // namespace Managed
} // namespace MyGUI
