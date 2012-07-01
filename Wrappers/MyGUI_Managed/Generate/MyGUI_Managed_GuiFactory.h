/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>

#include "../Utility.h"
#include "../BaseWidget.h"

#include "MyGUI_Managed_MarshalingWidget.h"
#include "MyGUI_Managed_IncludeWidgets.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class GuiFactory abstract
		{

		protected:
			GuiFactory() { }

			Widget^ CreateWidget(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _type, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				std::string type = _type;
				if (type == "Sheet") type = "TabItem";
				return (Widget^)mCreators[ string_utility::utf8_to_managed(type) ] ( _parent, _style, _skin, _coord, _align, _layer, _name );
			}

		private:
			delegate BaseWidget^ HandleCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name);
			static System::Collections::Generic::Dictionary<System::String^, HandleCreator^>^ mCreators = gcnew System::Collections::Generic::Dictionary<System::String^, HandleCreator^>();

			static GuiFactory()
			{

				//InsertPoint






















						mCreators->Add(gcnew System::String("MultiListItem"), gcnew HandleCreator(MultiListItem::WidgetCreator));
						mCreators->Add(gcnew System::String("TabItem"), gcnew HandleCreator(TabItem::WidgetCreator));
						mCreators->Add(gcnew System::String("MenuItem"), gcnew HandleCreator(MenuItem::WidgetCreator));
						mCreators->Add(gcnew System::String("Window"), gcnew HandleCreator(Window::WidgetCreator));
						mCreators->Add(gcnew System::String("Widget"), gcnew HandleCreator(Widget::WidgetCreator));
						mCreators->Add(gcnew System::String("ScrollBar"), gcnew HandleCreator(ScrollBar::WidgetCreator));
						mCreators->Add(gcnew System::String("TabControl"), gcnew HandleCreator(TabControl::WidgetCreator));
						mCreators->Add(gcnew System::String("TextBox"), gcnew HandleCreator(TextBox::WidgetCreator));
						mCreators->Add(gcnew System::String("ImageBox"), gcnew HandleCreator(ImageBox::WidgetCreator));
						mCreators->Add(gcnew System::String("ScrollView"), gcnew HandleCreator(ScrollView::WidgetCreator));
						mCreators->Add(gcnew System::String("ProgressBar"), gcnew HandleCreator(ProgressBar::WidgetCreator));
						mCreators->Add(gcnew System::String("PopupMenu"), gcnew HandleCreator(PopupMenu::WidgetCreator));
						mCreators->Add(gcnew System::String("MultiListBox"), gcnew HandleCreator(MultiListBox::WidgetCreator));
						mCreators->Add(gcnew System::String("MenuControl"), gcnew HandleCreator(MenuControl::WidgetCreator));
						mCreators->Add(gcnew System::String("MenuBar"), gcnew HandleCreator(MenuBar::WidgetCreator));
						mCreators->Add(gcnew System::String("ListBox"), gcnew HandleCreator(ListBox::WidgetCreator));
						mCreators->Add(gcnew System::String("ItemBox"), gcnew HandleCreator(ItemBox::WidgetCreator));
						mCreators->Add(gcnew System::String("EditBox"), gcnew HandleCreator(EditBox::WidgetCreator));
						mCreators->Add(gcnew System::String("DDContainer"), gcnew HandleCreator(DDContainer::WidgetCreator));
						mCreators->Add(gcnew System::String("ComboBox"), gcnew HandleCreator(ComboBox::WidgetCreator));
						mCreators->Add(gcnew System::String("Button"), gcnew HandleCreator(Button::WidgetCreator));
			}

		};

	} // namespace Managed
} // namespace MyGUI
