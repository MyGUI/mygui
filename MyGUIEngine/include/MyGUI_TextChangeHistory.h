/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TEXT_CHANGE_HISTORY_H__
#define __MYGUI_TEXT_CHANGE_HISTORY_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	// типы операций
	enum CommandType
	{
		COMMAND_POSITION,
		COMMAND_INSERT,
		COMMAND_ERASE
	};

	// инфо об одной операции
	struct tagChangeInfo
	{
		// для удаления и вставки текста
		tagChangeInfo(const Ogre::DisplayString & _text, size_t _start, CommandType _type)
			: text(_text), start(_start), type(_type), undo(ITEM_NONE), redo(ITEM_NONE), length(ITEM_NONE) {}
		// для указания позиции
		tagChangeInfo(size_t _undo, size_t _redo, size_t _length)
			: start(ITEM_NONE), type(COMMAND_POSITION), undo(_undo), redo(_redo), length(_length) {}

		// строка харрактиризуещая изменения
		Ogre::DisplayString text;
		// тип операции
		CommandType type;
		// инфа о начале позиции
		size_t start;
		// инфа о псевдо позиции
		size_t undo, redo, length;
	};

	typedef std::vector<tagChangeInfo> VectorChangeInfo;
	typedef std::deque<VectorChangeInfo> DequeUndoRedoInfo;

} // namespace MyGUI

#endif // __MYGUI_TEXT_CHANGE_HISTORY_H__