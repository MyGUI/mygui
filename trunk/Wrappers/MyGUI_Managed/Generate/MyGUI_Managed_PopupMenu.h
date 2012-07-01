/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI_Managed_MenuControl.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class PopupMenu : public MenuControl
		{

		private:
			typedef MyGUI::PopupMenu ThisType;

		public:
			PopupMenu() : MenuControl() { }

		internal:
			PopupMenu( MyGUI::PopupMenu* _native ) : MenuControl(_native) { }
			PopupMenu( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew PopupMenu(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

		};

	} // namespace Managed
} // namespace MyGUI
