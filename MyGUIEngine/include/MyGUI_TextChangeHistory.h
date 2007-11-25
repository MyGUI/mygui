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
		tagChangeInfo(const Ogre::DisplayString & _text, size_t _start, CommandType _type)
			: text(_text), start(_start), type(_type), undo(SIZE_MAX), redo(SIZE_MAX), lenght(SIZE_MAX) {}
		// ��� �������� �������
		tagChangeInfo(size_t _undo, size_t _redo, size_t _lenght)
			: start(SIZE_MAX), type(COMMAND_POSITION), undo(_undo), redo(_redo), lenght(_lenght) {}

		// ������ ���������������� ���������
		Ogre::DisplayString text;
		// ��� ��������
		CommandType type;
		// ���� � ������ �������
		size_t start;
		// ���� � ������ �������
		size_t undo, redo, lenght;
	};

	typedef std::vector<tagChangeInfo> VectorChangeInfo;
	typedef std::deque<VectorChangeInfo> DequeUndoRedoInfo;

} // namespace MyGUI

#endif // __MYGUI_TEXT_CHANGE_HISTORY_H__