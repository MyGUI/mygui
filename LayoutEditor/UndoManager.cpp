#include "UndoManager.h"

const int UNDO_COUNT = 64;

UndoManager::UndoManager(EditorWidgets * _ew):
	pos(0),
	operations(UNDO_COUNT),
	last_property(PR_DEFAULT),
	ew(_ew)
{
}

UndoManager::~UndoManager()
{
	for (size_t i=0; i<operations.GetSize(); i++){ delete operations[i];}
}

void UndoManager::undo()
{
	if (pos == operations.GetSize() - 1) return;
	pos++;
	ew->clear();
	ew->loadxmlDocument(operations[pos]);
}

void UndoManager::redo()
{
	if (pos == 0) return;
	pos--;
	ew->clear();
	ew->loadxmlDocument(operations[pos]);
}

void UndoManager::addValue(int _property)
{
	if ((_property != PR_DEFAULT) && (_property == last_property))
	{
		delete operations.Front();
		operations.PopFirst();
		operations.Push( ew->savexmlDocument() );
		return;
	}

	last_property = _property;

	if ( pos != 0 )
	{
		last_property = PR_DEFAULT;
		while (pos)
		{
			delete operations.Front();
			operations.PopFirst();
			pos--;
		}
	}

	if ( operations.IsFull() ) delete operations.Back();
	operations.Push( ew->savexmlDocument() );
	pos = 0;
}
