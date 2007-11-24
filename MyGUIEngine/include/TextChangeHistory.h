#ifndef __TEXT_CHANGE_HISTORY_H__
#define __TEXT_CHANGE_HISTORY_H__

#include "Prerequest.h"

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
			: text(_text), start(_start), type(_type), undo(SIZE_MAX), redo(SIZE_MAX), lenght(SIZE_MAX) {}
		// для указания позиции
		tagChangeInfo(size_t _undo, size_t _redo, size_t _lenght)
			: start(SIZE_MAX), type(COMMAND_POSITION), undo(_undo), redo(_redo), lenght(_lenght) {}

		// строка харрактиризуещая изменения
		Ogre::DisplayString text;
		// тип операции
		CommandType type;
		// инфа о начале позиции
		size_t start;
		// инфа о псевдо позиции
		size_t undo, redo, lenght;
	};

	typedef std::vector<tagChangeInfo> VectorChangeInfo;
	typedef std::deque<VectorChangeInfo> DequeUndoRedoInfo;

} // namespace MyGUI

#endif // __TEXT_CHANGE_HISTORY_H__