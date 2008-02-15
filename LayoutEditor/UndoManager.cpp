#include "UndoManager.h"
#include "WidgetContainer.h"

const int UNDO_COUNT = 64;

UndoManager::UndoManager(EditorWidgets * _ew):
	pos(0),
	operations(UNDO_COUNT),
	last_property(PR_DEFAULT),
	ew(_ew)
{

}

void UndoManager::addValue( const UndoOperation & _undoOp, int _property)
{
	UndoOperation top = operations.Front();

	if ((_property != PR_DEFAULT) && (_property == last_property))
	{
		if ((1 == _undoOp.containers.size()) && (1 == top.containers.size()))
		{
			// "merge" (just delete last one)
			operations.Pop();
			operations.Push(_undoOp);
			return;
		}
	}

	last_property = _property;
	size_t sz = operations.GetSize( );

	if ( pos < sz - 1 )
	{
		last_property = PR_DEFAULT;
		while ( pos != ( sz - 1 ) )
		{
			UndoOperation pop = operations.Front( );
			if (pop.operation = OP_DELETE)
				for (std::vector<WidgetContainer*>::iterator iter = pop.containers.begin(); iter != pop.containers.end(); ++iter) ;//delete *iter;
			operations.Pop();
			sz = operations.GetSize( );
		}
	}

	operations.Push( _undoOp );
	sz = operations.GetSize( );
	pos = sz - 1;
}