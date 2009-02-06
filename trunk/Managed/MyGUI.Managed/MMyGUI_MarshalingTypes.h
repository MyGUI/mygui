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
	template <> struct Convert<MyGUI::IntCoord>
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
	template <> struct Convert<MyGUI::MessageStyle>
	{
		typedef MessageStyle Type;
		inline static Type To(MyGUI::MessageStyle _value) { return _value; }
		inline static MyGUI::MessageStyle From(Type _value) { return _value; }
	};
	template <> struct Convert<MyGUI::DropItemState>
	{
		typedef DropItemState Type;
		inline static Type To(MyGUI::DropItemState _value) { return _value; }
		inline static MyGUI::DropItemState From(Type _value) { return _value; }
	};

	template <> struct Convert<const MyGUI::DropWidgetState&>
	{
		typedef DropWidgetState Type;
		inline static const DropWidgetState& To(const MyGUI::DropWidgetState& _value)
		{
			return reinterpret_cast<const DropWidgetState&>(_value);
		}
	};

	template <> struct Convert<const MyGUI::ToolTipInfo&>
	{
		typedef ToolTipInfo Type;
		inline static const ToolTipInfo& To(const MyGUI::ToolTipInfo& _value)
		{
			return reinterpret_cast<const ToolTipInfo&>(_value);
		}
	};

	template <> struct Convert<const MyGUI::ItemDropInfo&>
	{
		typedef ItemDropInfo Type;
		inline static ItemDropInfo To(const MyGUI::ItemDropInfo& _value)
		{
			ItemDropInfo info;
			info.sender_index = _value.sender_index;
			info.sender = Convert<MyGUI::Widget*>::To(_value.sender);
			info.reseiver_index = _value.reseiver_index;
			info.reseiver = Convert<MyGUI::Widget*>::To(_value.reseiver);
			return info;
		}
	};

	template <> struct Convert<bool&>
	{
		typedef bool% Type;
		inline static bool% To(bool& _value)
		{
			return reinterpret_cast<bool&>(_value);
		}
	};

	template <> struct Convert<MyGUI::IntCoord&>
	{
		typedef IntCoord%Type;
		inline static IntCoord% To(MyGUI::IntCoord& _value)
		{
			return reinterpret_cast<IntCoord&>(_value);
		}
	};

	template <> struct Convert<const MyGUI::NotifyItemData&>
	{
		typedef NotifyItemData Type;
		inline static NotifyItemData To(const MyGUI::NotifyItemData& _value)
		{
			NotifyItemData info;
			info.index = _value.index;
			info.notify = (NotifyItemData::NotifyItem)_value.notify;
			info.x = _value.x;
			info.y = _value.y;
			info.id = (MouseButton)_value.id.toValue();
			info.code = (KeyCode)_value.code.toValue();
			info.key = _value.key;
			return info;
		}
	};

	template <> struct Convert<const MyGUI::ItemInfo&>
	{
		typedef ItemInfo Type;
		inline static const ItemInfo& To(const MyGUI::ItemInfo& _value)
		{
			return reinterpret_cast<const ItemInfo&>(_value);
		}
	};

}