#include "Precompiled.h"
#include "UndoManager.h"
#include "CommandManager.h"
#include "WidgetSelectorManager.h"

template <> tools::UndoManager* MyGUI::Singleton<tools::UndoManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::UndoManager>::mClassTypeName = "UndoManager";

namespace tools
{

	const int UNDO_COUNT = 64;

	UndoManager::UndoManager() :
		mPosition(0),
		mOperations(UNDO_COUNT),
		mLastProperty(0),
		mEditorWidgets(nullptr),
		mUnsaved(false)
	{
		CommandManager::getInstance().getEvent("Command_Undo")->connect(this, &UndoManager::commandUndo);
		CommandManager::getInstance().getEvent("Command_Redo")->connect(this, &UndoManager::commandRedo);
	}

	void UndoManager::initialise(EditorWidgets* _ew)
	{
		mPosition = 0;
		mLastProperty = PR_DEFAULT;
		mEditorWidgets = _ew;
		UndoManager::getInstance().addValue();
		setUnsaved(false);
	}

	void UndoManager::shutdown()
	{
		for (size_t i = 0; i < mOperations.GetSize(); i++)
		{
			delete mOperations[i];
		}
		mOperations.Clear();
	}

	void UndoManager::undo()
	{
		if (mPosition == mOperations.GetSize() - 1) return;

		setUnsaved(true);

		mPosition++;
		mEditorWidgets->clear();
		mEditorWidgets->loadxmlDocument(mOperations[mPosition]);
	}

	void UndoManager::redo()
	{
		if (mPosition == 0) return;

		setUnsaved(true);

		mPosition--;
		mEditorWidgets->clear();
		mEditorWidgets->loadxmlDocument(mOperations[mPosition]);
	}

	void UndoManager::addValue(int _property)
	{
		setUnsaved(true);

		if ((_property != PR_DEFAULT) && (_property == mLastProperty))
		{
			delete mOperations.Front();
			mOperations.PopFirst();
			mOperations.Push( mEditorWidgets->savexmlDocument() );
			return;
		}

		mLastProperty = _property;

		if ( mPosition != 0 )
		{
			mLastProperty = PR_DEFAULT;
			while (mPosition)
			{
				delete mOperations.Front();
				mOperations.PopFirst();
				mPosition--;
			}
		}

		if ( mOperations.IsFull() ) delete mOperations.Back();
		mOperations.Push( mEditorWidgets->savexmlDocument() );
		mPosition = 0;
	}

	void UndoManager::commandUndo(const MyGUI::UString& _commandName, bool& _result)
	{
		undo();
		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

		_result = true;
	}

	void UndoManager::commandRedo(const MyGUI::UString& _commandName, bool& _result)
	{
		redo();
		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

		_result = true;
	}

	void UndoManager::setUnsaved(bool _value)
	{
		if (mUnsaved != _value)
		{
			mUnsaved = _value;
			eventChanges(mUnsaved);
		}
	}

}
