/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_VScrollBar.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class HScrollBar : public VScrollBar
		{

		private:
			typedef MyGUI::HScroll ThisType;

		public:
			HScrollBar() : VScrollBar() { }

		internal:
			HScrollBar( MyGUI::HScroll* _native ) : VScrollBar(_native) { }
			HScrollBar( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew HScrollBar(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   


   


   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
