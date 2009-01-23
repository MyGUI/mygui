/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"

namespace MMyGUI
{

	public value struct DropItemState
	{

		MMYGUI_MANAGED_NATIVE_CONVERSIONS_FOR_VALUE(DropItemState);
		MMYGUI_DECLARE_EQUALS(DropItemState);

		MMYGUI_DECLARE_ENUM(DropItemState, None);
		MMYGUI_DECLARE_ENUM(DropItemState, Start);
		MMYGUI_DECLARE_ENUM(DropItemState, End);
		MMYGUI_DECLARE_ENUM(DropItemState, Miss);
		MMYGUI_DECLARE_ENUM(DropItemState, Accept);
		MMYGUI_DECLARE_ENUM(DropItemState, Refuse);

		explicit DropItemState( int _value ) : value( _value) { }

		static bool operator == ( DropItemState lvalue, DropItemState rvalue ) { return ( lvalue.value == rvalue.value ); }

	private:
		int value;
	};

} // namespace MMyGUI
