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

#include "Generate/MyGUI.Managed_GuiFactory.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class Gui : public GuiFactory
		{
		private:
			Gui() : GuiFactory() { }

		public:
			static property Gui^ Instance
			{
				Gui^ get( )
				{
					if (mGui == nullptr)
					{
						// на случай если создали до нас
						mGui = MyGUI::Gui::getInstancePtr();
						if (mGui == nullptr)
						{
							throw gcnew System::NullReferenceException();
						}
						else
						{
							mInputManager = MyGUI::InputManager::getInstancePtr();
							mLayerManager = MyGUI::LayerManager::getInstancePtr();

							MMYGUI_INITIALISE;
						}
					}
					return m_instance;
				}
			}

		public:
			static void Initialise(System::String^ _config, System::String^ _group, System::String^ _logname)
			{
				Initialise(Ogre::Root::getSingleton().getAutoCreatedWindow(),
					Convert<const std::string&>::From(_config),
					Convert<const std::string&>::From(_group),
					Convert<const std::string&>::From(_logname));
			}

			static void Initialise(System::String^ _config, System::String^ _group)
			{
				Initialise(Ogre::Root::getSingleton().getAutoCreatedWindow(),
					Convert<const std::string&>::From(_config),
					Convert<const std::string&>::From(_group),
					"MyGUI.log");
			}

			static void Initialise(System::String^ _config)
			{
				Initialise(Ogre::Root::getSingleton().getAutoCreatedWindow(),
					Convert<const std::string&>::From(_config),
					"General",
					"MyGUI.log");
			}

			static void Initialise()
			{
				Initialise(Ogre::Root::getSingleton().getAutoCreatedWindow(),
					"core.xml",
					"General",
					"MyGUI.log");
			}

			static void Shutdown()
			{
				if (mGui)
				{
					mGui->shutdown();
					delete mGui;
					mGui = nullptr;
				}
			}

		public:
			void DebugOut(System::String^ _line)
			{
				MYGUI_OUT( Convert<const std::string&>::From(_line) );
			}

			void InjectFrameEntered(float _time)
			{
				mGui->injectFrameEntered(_time);
			}

			bool InjectMouseMove(int _absx, int _absy, int _absz)
			{
				return mGui->injectMouseMove(_absx, _absy, _absz);
			}

			bool InjectMousePress(int _absx, int _absy, int _mouseid)
			{
				return mGui->injectMousePress(_absx, _absy, MyGUI::MouseButton::Enum(_mouseid));
			}

			bool InjectMouseRelease(int _absx, int _absy, int _mouseid)
			{
				return mGui->injectMouseRelease(_absx, _absy, MyGUI::MouseButton::Enum(_mouseid));
			}

			bool InjectKeyPress(int _keyid, unsigned int _text)
			{
				return mGui->injectKeyPress(MyGUI::KeyCode::Enum(_keyid), _text);
			}

			bool InjectKeyRelease(int _keyid)
			{
				return mGui->injectKeyRelease(MyGUI::KeyCode::Enum(_keyid));
			}

		private:
			static void Initialise(Ogre::RenderWindow* _window, const std::string& _core, const std::string& _group, const std::string& _logFileName)
			{
				mGui = new MyGUI::Gui();
				mGui->initialise(_window, _core, _group, _logFileName);

				mInputManager = MyGUI::InputManager::getInstancePtr();
				mLayerManager = MyGUI::LayerManager::getInstancePtr();
				mPointerManager = MyGUI::PointerManager::getInstancePtr();

				MMYGUI_INITIALISE;
			}

		public:
			generic <typename WidgetType> where WidgetType : ref class
			WidgetType CreateWidget(System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name)
			{
				BaseWidget^ child = (BaseWidget^)(System::Activator::CreateInstance<WidgetType>());
				child->CreateWidget(nullptr, MyGUI::WidgetStyle::Overlapped, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), Convert<const std::string&>::From(_layer), Convert<const std::string&>::From(_name));
				return (WidgetType)child;
			}

			generic <typename WidgetType> where WidgetType : ref class
			WidgetType CreateWidget(System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer)
			{
				BaseWidget^ child = (BaseWidget^)(System::Activator::CreateInstance<WidgetType>());
				child->CreateWidget(nullptr, MyGUI::WidgetStyle::Overlapped, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), Convert<const std::string&>::From(_layer), "");
				return (WidgetType)child;
			}

			BaseWidget^ CreateWidgetT(System::Type^ _type, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name)
			{
				System::Reflection::ConstructorInfo^ ci = _type->GetConstructor(gcnew cli::array<System::Type^>(0));
				BaseWidget^ child = (BaseWidget^)ci->Invoke(nullptr);
				child->CreateWidget(nullptr, MyGUI::WidgetStyle::Overlapped, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), Convert<const std::string&>::From(_layer), Convert<const std::string&>::From(_name));
				return child;
			}

			Widget^ CreateWidgetT(BaseWidget^ _parent, WidgetStyle _style, System::String^ _type, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name)
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

			void DestroyWidget(Widget^ _widget)
			{
				delete _widget;
				_widget = nullptr;
			}

		public:
			property bool KeyFocus
			{
				bool get() { return mInputManager->getKeyFocusWidget() != nullptr; }
			}

		public:
			property bool MouseFocus
			{
				bool get() { return mInputManager->getMouseFocusWidget() != nullptr; }
			}

		public:
			property Widget^ KeyFocusWidget
			{
				Widget^ get() { return Convert< MyGUI::Widget * >::To(mInputManager->getKeyFocusWidget()); }
				void set(Widget^ _widget) { mInputManager->setKeyFocusWidget( Convert< MyGUI::Widget * >::From(_widget) ); }
			}

		public:
			property Widget^ MouseFocusWidget
			{
				Widget^ get() { return Convert< MyGUI::Widget * >::To(mInputManager->getMouseFocusWidget()); }
			}

		public:
			void ResetKeyFocus()
			{
				mInputManager->setKeyFocusWidget(nullptr);
			}

		public:
			void LoadResource(System::String^ _source, System::String^ _group)
			{
				mGui->load( Convert< const std::string& >::From(_source), Convert< const std::string& >::From(_group) );
			}

		public:
			void AddWidgetModal(Widget^ _widget)
			{
				mInputManager->addWidgetModal( Convert< MyGUI::Widget * >::From(_widget) );
			}

		public:
			void RemoveWidgetModal(Widget^ _widget)
			{
				mInputManager->removeWidgetModal( Convert< MyGUI::Widget * >::From(_widget) );
			}

		public:
			void UpWidget(Widget^ _widget)
			{
				mLayerManager->upLayerItem( Convert< MyGUI::Widget * >::From(_widget) );
			}

		public:
			void SetProperty(Widget^ _widget, System::String^ _key, System::String^ _value)
			{
				MyGUI::WidgetManager::getInstance().parse(
					_widget->GetNativePtr(),
					Convert<const std::string&>::From(_key),
					Convert<const std::string&>::From(_value)
					);
			}

			void SetUserString(Widget^ _widget, System::String^ _key, System::String^ _value)
			{
				_widget->GetNativePtr()->setUserString
					(
					Convert<const std::string&>::From(_key),
					Convert<const std::string&>::From(_value)
					);

				if (mDelegateParserUserData != nullptr)
					mDelegateParserUserData(_widget, _key, _value);
			}

		public:
			System::Collections::Generic::List<Widget^>^ LoadLayout(System::String^ _file)
			{
				return LoadLayout(_file, nullptr, "");
			}

		public:
			System::Collections::Generic::List<Widget^>^ LoadLayout(System::String^ _file, Widget^ _parent)
			{
				return LoadLayout(_file, _parent, "");
			}

		public:
			System::Collections::Generic::List<Widget^>^ LoadLayout(System::String^ _file, System::String^ _prefix)
			{
				return LoadLayout(_file, nullptr, _prefix);
			}

		public:
			void AttachToLayer(System::String^  _layer, Widget^ _widget)
			{
				mLayerManager->attachToLayerKeeper( string_utility::managed_to_utf8(_layer) , Convert< MyGUI::Widget * >::From(_widget) );
			}

		public:
			System::Collections::Generic::List<Widget^>^ LoadLayout(System::String^ _file, Widget^ _parent, System::String^ _prefix)
			{
				const std::string& file = string_utility::managed_to_utf8(_file);
				const std::string& prefix = string_utility::managed_to_utf8(_prefix);

				MyGUI::xml::Document doc;
				if ( ! doc.open(file, MyGUI::ResourceManager::getInstance().getResourceGroup()) )
				{
					MYGUI_EXCEPT("MyGUI::Gui : '" << file << "', " << doc.getLastError());
				}

				MyGUI::xml::ElementPtr root = doc.getRoot();
				if ( (nullptr == root) || (root->getName() != "MyGUI") )
				{
					MYGUI_EXCEPT("MyGUI::Gui : '" << file << "', tag 'MyGUI' not found");
				}

				if (root->findAttribute("type") != "Layout")
				{
					MYGUI_EXCEPT("MyGUI::Gui : '" << file << "', attribute 'type' != 'Layout'");
				}

				MyGUI::Version version = MyGUI::Version::parse(root->findAttribute("version"));

				System::Collections::Generic::List<Widget^>^ widgets = gcnew System::Collections::Generic::List<Widget^>();

				MyGUI::xml::ElementEnumerator widgets_array = root->getElementEnumerator();
				while(widgets_array.next("Widget"))
				{
					ParseWidget(widgets_array.current(), version, widgets, _parent, prefix, true);
				}

				return widgets;
			}

			void UnloadLayout(System::Collections::Generic::List<Widget^>^ _list)
			{
				for (int index=0; index<_list->Count; ++index)
				{
					delete _list[index];
				}
				_list->Clear();
			}

		public:
			Widget^ FindWidgetT(const std::string& _name)
			{
				return FindWidgetT(_name, true);
			}

			Widget^ FindWidgetT(const std::string& _name, bool _throw)
			{
				return Convert< MyGUI::Widget * >::To( mGui->findWidgetT(_name, _throw) );
			}

			Widget^ FindWidgetT(const std::string& _name, const std::string& _prefix)
			{
				return FindWidgetT(_prefix + _name, true);
			}

			Widget^ FindWidgetT(const std::string& _name, const std::string& _prefix, bool _throw)
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
				Widget^ widget = FindWidgetT(_name, _throw);
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
			property bool PointerVisible
			{
				bool get() { return mPointerManager->isVisible(); }
				void set(bool _value) { mPointerManager->setVisible(_value); }
			}

		public:
			delegate void HandleParserUserData( Widget^ _widget, System::String^ _key, System::String^ _value );
			event HandleParserUserData^ EventParserUserData
			{
				void add(HandleParserUserData^ _value)
				{
					mDelegateParserUserData += _value;
				}
				void remove(HandleParserUserData^ _value)
				{
					mDelegateParserUserData -= _value;
				}
			}
		private:
			HandleParserUserData^ mDelegateParserUserData;

		private:
			void ParseWidget(
				MyGUI::xml::ElementPtr _widget,
				MyGUI::Version _version, System::Collections::Generic::List<Widget^>^ _widgets,
				Widget^ _parent,
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
				if (_widget->findAttribute("position", tmp)) coord = MyGUI::IntCoord::parse(tmp);
				//FIXME парент может быть и не кроппед
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
						MyGUI::WidgetManager::getInstance().parse(wid->GetNativePtr(), key, value);
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

		private:
			static Gui^ m_instance = gcnew Gui();
			static MyGUI::Gui * mGui = nullptr;
			static MyGUI::InputManager* mInputManager = nullptr;
			static MyGUI::LayerManager* mLayerManager = nullptr;
			static MyGUI::PointerManager* mPointerManager = nullptr;

		};

	} // namespace Managed
} // namespace MyGUI
