#include "precompiled.h"
#include "UndoManager.h"

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#	ifdef MYGUI_CHECK_MEMORY_LEAKS
#		define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#		define new DEBUG_NEW
#	endif
#endif


const int UNDO_COUNT = 64;

const std::string INSTANCE_TYPE_NAME("UndoManager");
UndoManager* UndoManager::msInstance = 0;
UndoManager* UndoManager::getInstancePtr()
{
	return msInstance;
}
UndoManager& UndoManager::getInstance()
{
	MYGUI_ASSERT(0 != msInstance, "instance " << INSTANCE_TYPE_NAME << " was not created");
	return (*msInstance);
}
UndoManager::UndoManager() :
	mIsInitialise(false),
	pos(0),
	operations(UNDO_COUNT),
	last_property(0),
	ew(nullptr),
	mUnsaved(false)
{
	MYGUI_ASSERT(0 == msInstance, "instance " << INSTANCE_TYPE_NAME << " is exsist");
	msInstance=this;
}
UndoManager::~UndoManager() { msInstance=0; }

void UndoManager::initialise(EditorWidgets * _ew)
{
	pos = 0;
	last_property = PR_DEFAULT;
	ew = _ew;
	mUnsaved = false;
}

void UndoManager::shutdown()
{
	for (size_t i=0; i<operations.GetSize(); i++)
	{
		delete operations[i];
	}
	operations.Clear();
}

void UndoManager::undo()
{
	mUnsaved = true;

	if (pos == operations.GetSize() - 1) return;
	pos++;
	ew->clear();
	ew->loadxmlDocument(operations[pos]);
}

void UndoManager::redo()
{
	mUnsaved = true;

	if (pos == 0) return;
	pos--;
	ew->clear();
	ew->loadxmlDocument(operations[pos]);
}

void UndoManager::addValue(int _property)
{
	mUnsaved = true;

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
