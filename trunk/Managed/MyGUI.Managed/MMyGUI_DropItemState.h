/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"
#include "MMyGUI_Marshaling.h"

MMYGUI_BEGIN_NAMESPACE

//#ifndef MMYGUI_USING_EXTERNAL_TYPE

public enum struct DropItemState
{
	None = MyGUI::DropItemState::None,
	Start = MyGUI::DropItemState::Start,
	End = MyGUI::DropItemState::End,
	Miss = MyGUI::DropItemState::Miss,
	Accept = MyGUI::DropItemState::Accept,
	Refuse = MyGUI::DropItemState::Refuse
};

//#endif // MMYGUI_USING_EXTERNAL_TYPE

template <> struct Convert<MyGUI::DropItemState>
{
	typedef DropItemState Type;
	inline static const DropItemState& To(const MyGUI::DropItemState& _value) { return reinterpret_cast<const DropItemState&>(_value); }
	inline static MyGUI::DropItemState& From(DropItemState& _value) { return reinterpret_cast<MyGUI::DropItemState&>(_value); }
};

MMYGUI_END_NAMESPACE
