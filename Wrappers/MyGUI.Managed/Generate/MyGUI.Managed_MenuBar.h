/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_MenuCtrl.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class MenuBar : public MenuCtrl
		{

		private:
			typedef MyGUI::MenuBar ThisType;

		public:
			MenuBar() : MenuCtrl() { }

		internal:
			MenuBar( MyGUI::MenuBar* _native ) : MenuCtrl(_native) { }
			MenuBar( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew MenuBar(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
