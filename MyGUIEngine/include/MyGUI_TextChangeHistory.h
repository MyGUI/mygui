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

	// инфо об одной операции
	struct TextCommandInfo
	{
		// типы операций
		enum CommandType
		{
			COMMAND_POSITION,
			COMMAND_INSERT,
			COMMAND_ERASE
		};

		// для удаления и вставки текста
		TextCommandInfo(const UString& _text, size_t _start, CommandType _type) :
			text(_text),
			type(_type),
			start(_start),
			undo(ITEM_NONE),
			redo(ITEM_NONE),
			length(ITEM_NONE)
		{
		}

		// для указания позиции
		TextCommandInfo(size_t _undo, size_t _redo, size_t _length) :
			type(COMMAND_POSITION),
			start(ITEM_NONE),
			undo(_undo),
			redo(_redo),
			length(_length)
		{
		}

		// строка харрактиризуещая изменения
		UString text;
		// тип операции
		CommandType type;
		// инфа о начале позиции
		size_t start;
		// инфа о псевдо позиции
		size_t undo, redo, length;
	};

	typedef std::vector<TextCommandInfo> VectorChangeInfo;
	typedef std::deque<VectorChangeInfo> DequeUndoRedoInfo;

} // namespace MyGUI

#endif // MYGUI_TEXT_CHANGE_HISTORY_H_
