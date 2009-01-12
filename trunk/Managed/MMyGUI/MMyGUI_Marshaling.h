/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_MARSHALING_H__
#define __MMYGUI_MARSHALING_H__

#include <MyGUI.h>

#include "MMyGUI_Utility.h"
#include "MMyGUI_MouseButton.h"
#include "MMyGUI_KeyCode.h"

namespace MMyGUI
{

	ref class Widget;

	class WidgetHolder
	{
	public:
		WidgetHolder() : object() { }
		WidgetHolder(Widget ^ _obj) : object(_obj) { }
		~WidgetHolder() { }
		Widget ^ toObject() { return object; }
	private:
		gcroot<Widget^> object;
	};

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
	template <typename T> struct ConvertToType
	{
		typedef T Type;
		static inline Type ConvertToValue(T _value) { return _value; }
		static inline T ConvertFromValue(Type _value) { return _value; }
	};

	// перегрузка для виджетов
	template <> struct ConvertToType<MyGUI::Widget*>
	{
		typedef Widget^ Type;
		inline static Widget^ ConvertToValue(MyGUI::Widget* _value)
		{
			if (_value == 0) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			return obj ? obj->toObject() : nullptr;
		}
	};

	// перегрузка для базовых типов
	template <> struct ConvertToType<size_t>
	{
		typedef System::UInt32 Type;
		inline static System::UInt32 ConvertToValue(size_t _value) { return System::UInt32(_value); }
		inline static size_t ConvertFromValue(System::UInt32 _value) { return size_t(_value); }
	};

	// перегрузка для строк
	template <> struct ConvertToType<const std::string&>
	{
		typedef System::String^ Type;
		inline static System::String^ ConvertToValue(const std::string& _value) { return utf8_to_managed(_value); }
		inline static std::string ConvertFromValue(System::String^ _value) { return managed_to_utf8(_value); }
	};
	template <> struct ConvertToType<const Ogre::UTFString&>
	{
		typedef System::String^ Type;
		inline static System::String^ ConvertToValue(const Ogre::UTFString& _value) { return utf16_to_managed(_value); }
		inline static Ogre::UTFString ConvertFromValue(System::String^ _value) { return managed_to_utf16(_value); }
	};

	// перегрузка для ввода
	template <> struct ConvertToType<MyGUI::MouseButton>
	{
		typedef MouseButton Type;
		inline static MouseButton ConvertToValue(MyGUI::MouseButton _value) { return (MouseButton)_value.toValue(); }
	};
	template <> struct ConvertToType<MyGUI::KeyCode>
	{
		typedef KeyCode Type;
		inline static KeyCode ConvertToValue(MyGUI::KeyCode _value) { return (KeyCode)_value.toValue(); }
	};

	// прегрузка для Any
	template <> struct ConvertToType<MyGUI::Any>
	{
		typedef System::Object^ Type;
		inline static System::Object^ ConvertToValue(MyGUI::Any _value)
		{
			ObjectHolder * obj = _value.castType< ObjectHolder >(false);
			return obj ? obj->toObject() : nullptr;
		}
		inline static MyGUI::Any ConvertFromValue(System::Object^ _value)
		{
			ObjectHolder obj = _value;
			return obj;
		}
	};






#define MMYGUI_MANAGED_NATIVE_CONVERSIONS_FOR_VALUE(T) \
	static operator MyGUI::T& (T& obj) { return reinterpret_cast<MyGUI::T&>(obj); } \
	static operator const T& ( const MyGUI::T& obj) { return reinterpret_cast<const T&>(obj); } \
	static operator const T& ( const MyGUI::T* pobj) { return reinterpret_cast<const T&>(*pobj); }

#define MMYGUI_DECLARE_EQUALS(T) \
    static bool operator != ( T lvalue, T rvalue ) { return !(lvalue == rvalue); } \
	virtual bool Equals(T other) { return *this == other; }

}

#endif // __MMYGUI_MARSHALING_H__
