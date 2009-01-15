/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Types.h"
#include "MMyGUI_Marshaling.h"

namespace MMyGUI
{

	// перегрузка для ввода
	template <> struct Convert<MyGUI::MouseButton>
	{
		typedef MouseButton Type;
		inline static MouseButton To(MyGUI::MouseButton _value) { return (MouseButton)_value.toValue(); }
	};
	template <> struct Convert<MyGUI::KeyCode>
	{
		typedef KeyCode Type;
		inline static KeyCode To(MyGUI::KeyCode _value) { return (KeyCode)_value.toValue(); }
	};

	// перегрузка типов
	template <> struct Convert<const MyGUI::IntCoord&>
	{
		typedef IntCoord Type;
		inline static Type To(const MyGUI::IntCoord& _value) { return _value; }
		inline static MyGUI::IntCoord From(Type _value) { return _value; }
	};
	template <> struct Convert<const MyGUI::IntPoint&>
	{
		typedef IntPoint Type;
		inline static Type To(const MyGUI::IntPoint& _value) { return _value; }
		inline static MyGUI::IntPoint From(Type _value) { return _value; }
	};
	template <> struct Convert<const MyGUI::IntSize&>
	{
		typedef IntSize Type;
		inline static Type To(const MyGUI::IntSize& _value) { return _value; }
		inline static MyGUI::IntSize From(Type _value) { return _value; }
	};
	template <> struct Convert<const MyGUI::Colour&>
	{
		typedef Colour Type;
		inline static Type To(const MyGUI::Colour& _value) { return _value; }
		inline static MyGUI::Colour From(Type _value) { return _value; }
	};
	template <> struct Convert<MyGUI::Align>
	{
		typedef Align Type;
		inline static Type To(MyGUI::Align _value) { return _value; }
		inline static MyGUI::Align From(Type _value) { return _value; }
	};
	template <> struct Convert<MyGUI::WidgetStyle>
	{
		typedef WidgetStyle Type;
		inline static Type To(MyGUI::WidgetStyle _value) { return _value; }
		inline static MyGUI::WidgetStyle From(Type _value) { return _value; }
	};
	template <> struct Convert<MyGUI::MenuItemType>
	{
		typedef MenuItemType Type;
		inline static Type To(MyGUI::MenuItemType _value) { return _value; }
		inline static MyGUI::MenuItemType From(Type _value) { return _value; }
	};
}