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
#include "MMyGUI_IntCoord.h"
#include "MMyGUI_IntPoint.h"
#include "MMyGUI_IntSize.h"
#include "MMyGUI_Colour.h"
#include "MMyGUI_WidgetStyle.h"
#include "MMyGUI_Align.h"

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



	// ������� ������� ��� ����������� ���������� � �����
	template <typename T> struct ConvertToType
	{
		typedef T Type;
		static inline Type ConvertToValue(T _value) { return _value; }
		static inline T ConvertFromValue(Type _value) { return _value; }
	};

	// ���������� ��� ��������
	template <> struct ConvertToType<MyGUI::Widget*>
	{
		typedef Widget^ Type;
		inline static Widget^ ConvertToValue(MyGUI::Widget* _value)
		{
			if (_value == 0) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			return obj ? obj->toObject() : nullptr;
		}
		static inline MyGUI::Widget* ConvertFromValue(Widget^ _value) { return nullptr; }
	};

	// ���������� ��� ������� �����
	template <> struct ConvertToType<size_t>
	{
		typedef System::UInt32 Type;
		inline static System::UInt32 ConvertToValue(size_t _value) { return System::UInt32(_value); }
		inline static size_t ConvertFromValue(System::UInt32 _value) { return size_t(_value); }
	};

	// ���������� ��� �����
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

	// ���������� ��� �����
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

	// ��������� ��� Any
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

	// ���������� �����
	template <> struct ConvertToType<const MyGUI::IntCoord&>
	{
		typedef IntCoord Type;
		inline static Type ConvertToValue(const MyGUI::IntCoord& _value) { return _value; }
		inline static MyGUI::IntCoord ConvertFromValue(Type _value) { return _value; }
	};
	template <> struct ConvertToType<const MyGUI::IntPoint&>
	{
		typedef IntPoint Type;
		inline static Type ConvertToValue(const MyGUI::IntPoint& _value) { return _value; }
		inline static MyGUI::IntPoint ConvertFromValue(Type _value) { return _value; }
	};
	template <> struct ConvertToType<const MyGUI::IntSize&>
	{
		typedef IntSize Type;
		inline static Type ConvertToValue(const MyGUI::IntSize& _value) { return _value; }
		inline static MyGUI::IntSize ConvertFromValue(Type _value) { return _value; }
	};
	template <> struct ConvertToType<const MyGUI::Colour&>
	{
		typedef Colour Type;
		inline static Type ConvertToValue(const MyGUI::Colour& _value) { return _value; }
		inline static MyGUI::Colour ConvertFromValue(Type _value) { return _value; }
	};
	template <> struct ConvertToType<MyGUI::Align>
	{
		typedef Align Type;
		inline static Type ConvertToValue(MyGUI::Align _value) { return _value; }
		inline static MyGUI::Align ConvertFromValue(Type _value) { return _value; }
	};
	template <> struct ConvertToType<MyGUI::WidgetStyle>
	{
		typedef WidgetStyle Type;
		inline static Type ConvertToValue(MyGUI::WidgetStyle _value) { return _value; }
		inline static MyGUI::WidgetStyle ConvertFromValue(Type _value) { return _value; }
	};

}

#endif // __MMYGUI_MARSHALING_H__
