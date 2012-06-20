/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>

#include "Config.h"
#include "Utility.h"

#include "Generate/MyGUI_Managed_GuiFactory.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class Gui :
			public GuiFactory
		{
		private:
			Gui() :
			   GuiFactory()
			{
			}

		public:
			static property Gui ^ Instance
			{
				Gui ^ get( )
				{
					if (mGui == nullptr)
					{
						mGui = MyGUI::Gui::getInstancePtr();
						if (mGui == nullptr)
						{
							throw gcnew System::NullReferenceException();
						}
						else
						{
							MMYGUI_INITIALISE;
						}
					}
					return m_instance;
				}
			}

		public:
			generic <typename WidgetType> where WidgetType : ref class
			WidgetType CreateWidget(System::String ^ _skin, IntCoord _coord, Align _align, System::String ^ _layer, System::String ^ _name)
			{
				BaseWidget ^ child = (BaseWidget ^ )(System::Activator::CreateInstance<WidgetType>());
				child->CreateWidget(nullptr, MyGUI::WidgetStyle::Overlapped, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), Convert<const std::string&>::From(_layer), Convert<const std::string&>::From(_name));
				return (WidgetType)child;
			}

			generic <typename WidgetType> where WidgetType : ref class
			WidgetType CreateWidget(System::String ^ _skin, IntCoord _coord, Align _align, System::String ^ _layer)
			{
				BaseWidget ^ child = (BaseWidget ^ )(System::Activator::CreateInstance<WidgetType>());
				child->CreateWidget(nullptr, MyGUI::WidgetStyle::Overlapped, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), Convert<const std::string&>::From(_layer), "");
				return (WidgetType)child;
			}

			BaseWidget ^ CreateWidgetT(System::Type ^ _type, System::String ^ _skin, IntCoord _coord, Align _align, System::String ^ _layer, System::String ^ _name)
			{
				System::Reflection::ConstructorInfo ^ ci = _type->GetConstructor(gcnew cli::array < System::Type ^ > (0));
				BaseWidget ^ child = (BaseWidget ^ )ci->Invoke(nullptr);
				child->CreateWidget(nullptr, MyGUI::WidgetStyle::Overlapped, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), Convert<const std::string&>::From(_layer), Convert<const std::string&>::From(_name));
				return child;
			}

			Widget ^ CreateWidgetT(BaseWidget ^ _parent, WidgetStyle _style, System::String ^ _type, System::String ^ _skin, IntCoord _coord, Align _align, System::String ^ _layer, System::String ^ _name)
			{
				return CreateWidget(
					_parent,
					Convert<MyGUI::WidgetStyle>::From(_style),
					Convert<const std::string&>::From(_type),
					Convert<const std::string&>::From(_skin),
					Convert<const MyGUI::IntCoord&>::From(_coord),
					Convert<MyGUI::Align>::From(_align),
					Convert<const std::string&>::From(_layer),
					Convert<const std::string&>::From(_name)
				);
			}

			void DestroyWidget(Widget ^ _widget)
			{
				delete _widget;
				_widget = nullptr;
			}

		public:
			void LoadResource(System::String ^ _source)
			{
				MyGUI::ResourceManager::getInstance().load( Convert< const std::string& >::From(_source) );
			}

		public:
			void SetProperty(Widget ^ _widget, System::String ^ _key, System::String ^ _value)
			{
				_widget->GetNativePtr()->setProperty
				(
					Convert<const std::string&>::From(_key),
					Convert<const std::string&>::From(_value)
				);
			}

		public:
			System::Collections::Generic::List < Widget ^ > ^ LoadLayout(System::String ^ _file)
			{
				return LoadLayout(_file, nullptr, "");
			}

		public:
			System::Collections::Generic::List < Widget ^ > ^ LoadLayout(System::String ^ _file, Widget ^ _parent)
			{
				return LoadLayout(_file, _parent, "");
			}

		public:
			System::Collections::Generic::List < Widget ^ > ^ LoadLayout(System::String ^ _file, System::String ^ _prefix)
			{
				return LoadLayout(_file, nullptr, _prefix);
			}

		public:
			System::Collections::Generic::List < Widget ^ > ^ LoadLayout(System::String ^ _file, Widget ^ _parent, System::String ^ _prefix)
			{
				System::Collections::Generic::List < Widget ^ > ^ widgets = gcnew System::Collections::Generic::List < Widget ^ > ();

				const std::string& file = string_utility::managed_to_utf8(_file);
				const std::string& prefix = string_utility::managed_to_utf8(_prefix);

				MyGUI::xml::Document doc;
				if ( ! doc.open(MyGUI::DataManager::getInstance().getDataPath(file)) )
				{
					MYGUI_LOG(Warning, "MyGUI::Gui : '" << file << "', " << doc.getLastError());
					return widgets;
				}

				MyGUI::xml::ElementPtr root = doc.getRoot();
				if ( (nullptr == root) || (root->getName() != "MyGUI") )
				{
					MYGUI_LOG(Error, "MyGUI::Gui : '" << file << "', tag 'MyGUI' not found");
					return widgets;
				}

				if (root->findAttribute("type") != "Layout")
				{
					MYGUI_LOG(Error, "MyGUI::Gui : '" << file << "', attribute 'type' != 'Layout'");
					return widgets;
				}

				MyGUI::Version version = MyGUI::Version::parse(root->findAttribute("version"));

				MyGUI::xml::ElementEnumerator widgets_array = root->getElementEnumerator();
				while (widgets_array.next("Widget"))
				{
					ParseWidget(widgets_array.current(), version, widgets, _parent, prefix, true);
				}

				return widgets;
			}

			void UnloadLayout(System::Collections::Generic::List < Widget ^ > ^ _list)
			{
				for (int index = 0; index < _list->Count; ++index)
				{
					delete _list[index];
				}
				_list->Clear();
			}

		public:
			Widget ^ FindWidgetT(const std::string& _name)
			{
				return FindWidgetT(_name, true);
			}

			Widget ^ FindWidgetT(const std::string& _name, bool _throw)
			{
				return Convert< MyGUI::Widget* >::To( mGui->findWidgetT(_name, _throw) );
			}

			Widget ^ FindWidgetT(const std::string& _name, const std::string& _prefix)
			{
				return FindWidgetT(_prefix + _name, true);
			}

			Widget ^ FindWidgetT(const std::string& _name, const std::string& _prefix, bool _throw)
			{
				return FindWidgetT(_prefix + _name, _throw);
			}

			generic <typename WidgetType> where WidgetType : ref class
			WidgetType FindWidgetT(const std::string& _name)
			{
				return FindWidgetT<WidgetType>(_name, true);
			}

			generic <typename WidgetType> where WidgetType : ref class
			WidgetType FindWidgetT(const std::string& _name, bool _throw)
			{
				Widget ^ widget = FindWidgetT(_name, _throw);
				if (widget == nullptr) return (WidgetType)widget;
				if (_throw && (nullptr == cli::safe_cast<WidgetType>(widget))) throw gcnew System::Exception("error cast widget type");
				return (WidgetType)widget;
			}

			generic <typename WidgetType> where WidgetType : ref class
			WidgetType FindWidgetT(const std::string& _name, const std::string& _prefix)
			{
				return FindWidgetT<WidgetType>(_prefix + _name, true);
			}

			generic <typename WidgetType> where WidgetType : ref class
			WidgetType FindWidgetT(const std::string& _name, const std::string& _prefix, bool _throw)
			{
				return FindWidgetT<WidgetType>(_prefix + _name, _throw);
			}

		public:
			delegate void HandleParserUserData( Widget ^ _widget, System::String ^ _key, System::String ^ _value );
			event HandleParserUserData ^ EventParserUserData
			{
				void add(HandleParserUserData ^ _value)
				{
					mDelegateParserUserData += _value;
				}
				void remove(HandleParserUserData ^ _value)
				{
					mDelegateParserUserData -= _value;
				}
			}
		private:
			HandleParserUserData ^ mDelegateParserUserData;

		private:
			void ParseWidget(
				MyGUI::xml::ElementPtr _widget,
				MyGUI::Version _version, System::Collections::Generic::List < Widget ^ > ^ _widgets,
				Widget ^ _parent,
				const std::string& _prefix, bool _root)
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

				MyGUI::WidgetStyle style = MyGUI::WidgetStyle::Child;
				if (_widget->findAttribute("style", tmp)) style = MyGUI::WidgetStyle::parse(tmp);
				if (_parent != nullptr && style != MyGUI::WidgetStyle::Popup) layer.clear();

				if (_widget->findAttribute("position", tmp)) coord = MyGUI::IntCoord::parse(tmp);
				else if (_widget->findAttribute("position_real", tmp))
				{
					if (_parent == nullptr || style == MyGUI::WidgetStyle::Popup)
						coord = CoordConverter::convertFromRelative(MyGUI::FloatCoord::parse(tmp), MyGUI::RenderManager::getInstance().getViewSize());
					else
						coord = CoordConverter::convertFromRelative(MyGUI::FloatCoord::parse(tmp), _parent->GetNativePtr()->getSize());
				}

				if (!name.empty()) name = _prefix + name;

				Widget ^ wid = nullptr;
				if (nullptr == _parent)
					wid = CreateWidget(_parent, MyGUI::WidgetStyle::Overlapped, type, skin, coord, align, layer, name);
				else
					wid = CreateWidget(_parent, style, type, skin, coord, align, layer, name);

				// составляем список рутов
				if (_root) _widgets->Add(wid);

				// берем детей и крутимся
				MyGUI::xml::ElementEnumerator widget_element = _widget->getElementEnumerator();
				while (widget_element.next())
				{
					std::string key, value;

					if (widget_element->getName() == "Widget")
					{
						ParseWidget(widget_element.current(), _version, _widgets, wid, _prefix, false);
					}
					else if (widget_element->getName() == "Property")
					{
						// парсим атрибуты
						if (false == widget_element->findAttribute("key", key)) continue;
						if (false == widget_element->findAttribute("value", value)) continue;
						// и парсим свойство
						wid->GetNativePtr()->setProperty(key, value);
					}
					else if (widget_element->getName() == "UserString")
					{
						// парсим атрибуты
						if (false == widget_element->findAttribute("key", key)) continue;
						if (false == widget_element->findAttribute("value", value)) continue;
						wid->GetNativePtr()->setUserString(key, value);

						if (mDelegateParserUserData != nullptr) mDelegateParserUserData(wid, string_utility::utf8_to_managed(key), string_utility::utf8_to_managed(value));
					}

				}
			}

		public:
			void Log(
				Convert<const std::string &>::Type _section,
				Convert<MyGUI::LogLevel>::Type _level,
				Convert<const std::string &>::Type _message)
			{
				MyGUI::LogManager::getInstance().log(
					Convert<const std::string&>::From(_section),
					Convert<MyGUI::LogLevel>::From(_level),
					Convert<const std::string&>::From(_message),
					__FILE__,
					__LINE__
				);
			}

		private:
			static Gui ^ m_instance = gcnew Gui();
			static MyGUI::Gui* mGui = nullptr;
		};

	} // namespace Managed
} // namespace MyGUI
