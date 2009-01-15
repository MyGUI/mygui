/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>

#include "MMyGUI_Utility.h"

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

}
