/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TEXT_CHANGE_HISTORY_H_
#define MYGUI_TEXT_CHANGE_HISTORY_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_UString.h"
#include <deque>

namespace MyGUI
{

	// information about single text change operation
	struct TextCommandInfo
	{
		enum CommandType
		{
			COMMAND_POSITION,
			COMMAND_INSERT,
			COMMAND_ERASE
		};

		// for COMMAND_INSERT and COMMAND_ERASE
		TextCommandInfo(const UString::utf32string& _text, size_t _start, CommandType _type) :
			text(_text),
			type(_type),
			start(_start),
			undo(ITEM_NONE),
			redo(ITEM_NONE),
			length(ITEM_NONE)
		{
		}

		// for COMMAND_POSITION
		TextCommandInfo(size_t _undo, size_t _redo, size_t _length) :
			type(COMMAND_POSITION),
			start(ITEM_NONE),
			undo(_undo),
			redo(_redo),
			length(_length)
		{
		}

		// inserted/erased string
		UString::utf32string text;
		CommandType type;
		// text start position
		size_t start;
		// pseudo position
		size_t undo, redo, length;
	};

	using VectorChangeInfo = std::vector<TextCommandInfo>;
	using DequeUndoRedoInfo = std::deque<VectorChangeInfo>;

} // namespace MyGUI

#endif // MYGUI_TEXT_CHANGE_HISTORY_H_
