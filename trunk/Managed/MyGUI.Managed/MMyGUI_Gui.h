/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>

#include "MMyGUI_Utility.h"
#include "MMyGUI_AllWidgets.h"

namespace MMyGUI
{

	public ref class Gui
	{
	private:
		Gui() { }

	public:
		static property Gui^ Instance
		{
			Gui^ get( )
			{
				if (m_native == nullptr) m_native = MyGUI::Gui::getInstancePtr();
				return m_instance;
			}
		}

	public:
		generic <typename T> where T : ref class
		T CreateWidget(System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name)
		{
			Widget^ child = (Widget^)(System::Activator::CreateInstance<T>());
			child->CreateWidget(nullptr, MyGUI::WidgetStyle::Overlapped, managed_to_utf8(_skin), _coord, _align, managed_to_utf8(_layer), managed_to_utf8(_name));
			return (T)child;
		}

	public:
		generic <typename T> where T : ref class
		T CreateWidget(System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer)
		{
			Widget^ child = (Widget^)(System::Activator::CreateInstance<T>());
			child->CreateWidget(nullptr, MyGUI::WidgetStyle::Overlapped, managed_to_utf8(_skin), _coord, _align, managed_to_utf8(_layer), "");
			return (T)child;
		}

	public:
		System::Collections::Generic::List<Widget^>^ LoadLayout(System::String^ _file, Widget^ _parent, System::String^ _prefix)
		{
			const std::string& file = managed_to_utf8(_file);
			const std::string& prefix = managed_to_utf8(_prefix);

			MyGUI::xml::Document doc;
			if ( ! doc.open(file, MyGUI::ResourceManager::getInstance().getResourceGroup()) )
			{
				MYGUI_EXCEPT("MMyGUI::Gui : '" << file << "', " << doc.getLastError());
			}

			MyGUI::xml::ElementPtr root = doc.getRoot();
			if ( (nullptr == root) || (root->getName() != "MyGUI") )
			{
				MYGUI_EXCEPT("MMyGUI::Gui : '" << file << "', tag 'MyGUI' not found");
			}

			if (root->findAttribute("type") != "Layout")
			{
				MYGUI_EXCEPT("MMyGUI::Gui : '" << file << "', attribute 'type' != 'Layout'");
			}

			MyGUI::Version version = MyGUI::Version::parse(root->findAttribute("version"));

			System::Collections::Generic::List<Widget^>^ widgets = gcnew System::Collections::Generic::List<Widget^>();

			MyGUI::xml::ElementEnumerator widgets_array = root->getElementEnumerator();
			while(widgets_array.next("Widget"))
			{
				ParseWidget(widgets_array.current(), version, widgets, _parent, prefix);
			}

			return widgets;
		}

	private:
		void ParseWidget(MyGUI::xml::ElementPtr _widget, MyGUI::Version _version, System::Collections::Generic::List<Widget^>^ _widgets, Widget^ _parent, const std::string& _prefix)
		{
			// парсим атрибуты виджета
			std::string type, skin, name, layer, tmp;
			MyGUI::IntCoord coord;
			MyGUI::Align align = MyGUI::Align::Default;

			_widget->findAttribute("type", type);
			_widget->findAttribute("skin", skin);
			_widget->findAttribute("name", name);
			_widget->findAttribute("layer", layer);
			if (_widget->findAttribute("align", tmp)) align = MyGUI::Align::parse(tmp);
			if (_widget->findAttribute("position", tmp)) coord = MyGUI::IntCoord::parse(tmp);
			if (_widget->findAttribute("position_real", tmp)) coord = MyGUI::WidgetManager::getInstance().convertRelativeToInt(MyGUI::FloatCoord::parse(tmp), _parent->GetNativePtr());

			if (!name.empty()) name = _prefix + name;

			Widget^ wid = nullptr;
			if (nullptr == _parent)
			{
				wid = CreateWidget(_parent, MyGUI::WidgetStyle::Overlapped, type, skin, coord, align, layer, name);
			}
			else
			{
				MyGUI::WidgetStyle style = MyGUI::WidgetStyle::Child;
				if (_widget->findAttribute("style", tmp)) style = MyGUI::WidgetStyle::parse(tmp);
				if (style != MyGUI::WidgetStyle::Popup && !layer.empty()) layer.clear();

				wid = CreateWidget(_parent, style, type, skin, coord, align, layer, name);
			}

			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator widget_element = _widget->getElementEnumerator();
			while (widget_element.next())
			{
				std::string key, value;

				if (widget_element->getName() == "Widget")
				{
					ParseWidget(widget_element.current(), _version, _widgets, wid, _prefix);
				}
				else if (widget_element->getName() == "Property")
				{
					// парсим атрибуты
					if (false == widget_element->findAttribute("key", key)) continue;
					if (false == widget_element->findAttribute("value", value)) continue;
					// и парсим свойство
					MyGUI::WidgetManager::getInstance().parse(wid->GetNativePtr(), key, value);
				}
				else if (widget_element->getName() == "UserString")
				{
					// парсим атрибуты
					if (false == widget_element->findAttribute("key", key)) continue;
					if (false == widget_element->findAttribute("value", value)) continue;
					wid->GetNativePtr()->setUserString(key, value);
				}

			}
		}

		Widget^ CreateWidget(Widget^ _parent, MyGUI::WidgetStyle _style, const std::string& _type, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
		{
			return mCreators[utf8_to_managed(_type)](_parent, _style, _skin, _coord, _align, _layer, _name);
		}

	private:
		static Gui^ m_instance = gcnew Gui();
		static MyGUI::Gui * m_native = nullptr;

		delegate Widget^ HandleCreator(Widget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name);
		static System::Collections::Generic::Dictionary<System::String^, HandleCreator^>^ mCreators = gcnew System::Collections::Generic::Dictionary<System::String^, HandleCreator^>();

		static Gui()
		{
			#define MMYGUI_DECLARE_CREATOR(type) mCreators->Add(gcnew System::String(#type), gcnew HandleCreator(type::WidgetCreator))

			MMYGUI_DECLARE_CREATOR(MenuItem);
			MMYGUI_DECLARE_CREATOR(TabItem);
			MMYGUI_DECLARE_CREATOR(Button);
			MMYGUI_DECLARE_CREATOR(Canvas);
			MMYGUI_DECLARE_CREATOR(ComboBox);
			MMYGUI_DECLARE_CREATOR(DDContainer);
			MMYGUI_DECLARE_CREATOR(Edit);
			MMYGUI_DECLARE_CREATOR(HScroll);
			MMYGUI_DECLARE_CREATOR(ItemBox);
			MMYGUI_DECLARE_CREATOR(List);
			MMYGUI_DECLARE_CREATOR(MenuBar);
			MMYGUI_DECLARE_CREATOR(MenuCtrl);
			MMYGUI_DECLARE_CREATOR(Message);
			MMYGUI_DECLARE_CREATOR(MultiList);
			MMYGUI_DECLARE_CREATOR(PopupMenu);
			MMYGUI_DECLARE_CREATOR(Progress);
			MMYGUI_DECLARE_CREATOR(RenderBox);
			MMYGUI_DECLARE_CREATOR(ScrollView);
			MMYGUI_DECLARE_CREATOR(StaticImage);
			MMYGUI_DECLARE_CREATOR(StaticText);
			MMYGUI_DECLARE_CREATOR(Tab);
			MMYGUI_DECLARE_CREATOR(VScroll);
			MMYGUI_DECLARE_CREATOR(Widget);
			MMYGUI_DECLARE_CREATOR(Window);

			#undef MMYGUI_DECLARE_CREATOR
		}

	};

} // namespace MMyGUI
