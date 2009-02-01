/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>

#include "MMyGUI_Utility.h"
#include "MMyGUI_SubWidgetText.h"

namespace MMyGUI
{

	class ObjectHolder
	{
	public:
		ObjectHolder() : object() { }
		ObjectHolder(System::Object ^ _obj) : object(_obj) { }
		~ObjectHolder() { }
		System::Object ^ toObject() { return object; }
	private:
		gcroot<System::Object^> object;
	};


	// базовые шаблоны для конвертации переменных и типов
	template <typename T> struct Convert
	{
		typedef T Type;
		static inline Type To(T _value) { return _value; }
		static inline T From(Type _value) { return _value; }
	};


	// перегрузка для виджетов
	ref class Widget;
	template <> struct Convert<MyGUI::Widget*>
	{
		typedef Widget^ Type;
		static Widget^ To(MyGUI::Widget* _value);
		static MyGUI::Widget* From(Widget^ _value);
	};
	ref class MenuItem;
	template <> struct Convert<MyGUI::MenuItem*>
	{
		typedef MenuItem^ Type;
		static MenuItem^ To(MyGUI::MenuItem* _value);
		static MyGUI::MenuItem* From(MenuItem^ _value);
	};
	ref class MenuCtrl;
	template <> struct Convert<MyGUI::MenuCtrl*>
	{
		typedef MenuCtrl^ Type;
		static MenuCtrl^ To(MyGUI::MenuCtrl* _value);
		static MyGUI::MenuCtrl* From(MenuCtrl^ _value);
	};
	ref class TabItem;
	template <> struct Convert<MyGUI::TabItem*>
	{
		typedef TabItem^ Type;
		static TabItem^ To(MyGUI::TabItem* _value);
		static MyGUI::TabItem* From(TabItem^ _value);
	};
	ref class StaticImage;
	template <> struct Convert<MyGUI::StaticImage*>
	{
		typedef StaticImage^ Type;
		static StaticImage^ To(MyGUI::StaticImage* _value);
		static MyGUI::StaticImage* From(StaticImage^ _value);
	};
	ref class Edit;
	template <> struct Convert<MyGUI::Edit*>
	{
		typedef Edit^ Type;
		static Edit^ To(MyGUI::Edit* _value);
		static MyGUI::Edit* From(Edit^ _value);
	};
	ref class ComboBox;
	template <> struct Convert<MyGUI::ComboBox*>
	{
		typedef ComboBox^ Type;
		static ComboBox^ To(MyGUI::ComboBox* _value);
		static MyGUI::ComboBox* From(ComboBox^ _value);
	};
	ref class DDContainer;
	template <> struct Convert<MyGUI::DDContainer*>
	{
		typedef DDContainer^ Type;
		static DDContainer^ To(MyGUI::DDContainer* _value);
		static MyGUI::DDContainer* From(DDContainer^ _value);
	};
	ref class VScroll;
	template <> struct Convert<MyGUI::VScroll*>
	{
		typedef VScroll^ Type;
		static VScroll^ To(MyGUI::VScroll* _value);
		static MyGUI::VScroll* From(VScroll^ _value);
	};
	ref class ItemBox;
	template <> struct Convert<MyGUI::ItemBox*>
	{
		typedef ItemBox^ Type;
		static ItemBox^ To(MyGUI::ItemBox* _value);
		static MyGUI::ItemBox* From(ItemBox^ _value);
	};
	ref class List;
	template <> struct Convert<MyGUI::List*>
	{
		typedef List^ Type;
		static List^ To(MyGUI::List* _value);
		static MyGUI::List* From(List^ _value);
	};
	ref class Message;
	template <> struct Convert<MyGUI::Message*>
	{
		typedef Message^ Type;
		static Message^ To(MyGUI::Message* _value);
		static MyGUI::Message* From(Message^ _value);
	};
	ref class MultiList;
	template <> struct Convert<MyGUI::MultiList*>
	{
		typedef MultiList^ Type;
		static MultiList^ To(MyGUI::MultiList* _value);
		static MyGUI::MultiList* From(MultiList^ _value);
	};
	ref class Tab;
	template <> struct Convert<MyGUI::Tab*>
	{
		typedef Tab^ Type;
		static Tab^ To(MyGUI::Tab* _value);
		static MyGUI::Tab* From(Tab^ _value);
	};
	ref class Window;
	template <> struct Convert<MyGUI::Window*>
	{
		typedef Window^ Type;
		static Window^ To(MyGUI::Window* _value);
		static MyGUI::Window* From(Window^ _value);
	};


	// перегрузка для базовых типов
	template <> struct Convert<size_t>
	{
		typedef System::UInt32 Type;
		inline static System::UInt32 To(size_t _value) { return System::UInt32(_value); }
		inline static size_t From(System::UInt32 _value) { return size_t(_value); }
	};

	// перегрузка для строк
	template <> struct Convert<const std::string&>
	{
		typedef System::String^ Type;
		inline static System::String^ To(const std::string& _value) { return utf8_to_managed(_value); }
		inline static std::string From(System::String^ _value) { return managed_to_utf8(_value); }
	};
	template <> struct Convert<const Ogre::UTFString&>
	{
		typedef System::String^ Type;
		inline static System::String^ To(const Ogre::UTFString& _value) { return utf16_to_managed(_value); }
		inline static Ogre::UTFString From(System::String^ _value) { return managed_to_utf16(_value); }
	};
	template <> struct Convert<Ogre::UTFString>
	{
		typedef System::String^ Type;
		inline static System::String^ To(const Ogre::UTFString& _value) { return utf16_to_managed(_value); }
		inline static Ogre::UTFString From(System::String^ _value) { return managed_to_utf16(_value); }
	};

	// прегрузка для Any
	template <> struct Convert<MyGUI::Any>
	{
		typedef System::Object^ Type;
		inline static System::Object^ To(MyGUI::Any _value)
		{
			ObjectHolder * obj = _value.castType< ObjectHolder >(false);
			return obj ? obj->toObject() : nullptr;
		}
		inline static MyGUI::Any From(System::Object^ _value)
		{
			ObjectHolder obj = _value;
			return obj;
		}
	};

	template <> struct Convert<const MyGUI::Guid&>
	{
		typedef System::Guid Type;
		inline static const System::Guid& To(const MyGUI::Guid& _value)
		{
			return reinterpret_cast<const System::Guid&>(_value);
		}
		inline static const MyGUI::Guid& From(System::Guid& _value)
		{
			return reinterpret_cast<const MyGUI::Guid&>(_value);
		}
	};

	template <> struct Convert<MyGUI::ISubWidgetText*>
	{
		typedef SubWidgetText Type;
		inline static SubWidgetText To(MyGUI::ISubWidgetText* _value)
		{
			return SubWidgetText(_value);
		}
	};

}
