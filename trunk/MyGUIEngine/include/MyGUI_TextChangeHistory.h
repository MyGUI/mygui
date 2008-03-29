/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TEXT_CHANGE_HISTORY_H__
#define __MYGUI_TEXT_CHANGE_HISTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include <Ogre.h>

namespace MyGUI
{

	// ���� ��������
	enum CommandType
	{
		COMMAND_POSITION,
		COMMAND_INSERT,
		COMMAND_ERASE
	};

	// ���� �� ����� ��������
	struct tagChangeInfo
	{
		// ��� �������� � ������� ������
		tagChangeInfo(const Ogre::UTFString & _text, size_t _start, CommandType _type)
			: text(_text), type(_type), start(_start), undo(ITEM_NONE), redo(ITEM_NONE), length(ITEM_NONE) {}
		// ��� �������� �������
		tagChangeInfo(size_t _undo, size_t _redo, size_t _length)
			: type(COMMAND_POSITION), start(ITEM_NONE), undo(_undo), redo(_redo), length(_length) {}

		// ������ ���������������� ���������
		Ogre::UTFString text;
		// ��� ��������
		CommandType type;
		// ���� � ������ �������
		size_t start;
		// ���� � ������ �������
		size_t undo, redo, length;
	};

	typedef std::vector<tagChangeInfo> VectorChangeInfo;
	typedef std::deque<VectorChangeInfo> DequeUndoRedoInfo;

} // namespace MyGUI

#endif // __MYGUI_TEXT_CHANGE_HISTORY_H__
