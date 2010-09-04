#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "EditorWidgets.h"
#include "CyclicBuffer.h"

namespace tools
{
	enum { PR_DEFAULT, PR_POSITION, PR_PROPERTIES, PR_KEY_POSITION };

	typedef MyGUI::delegates::CMultiDelegate1<bool> Event_Changes;

	class UndoManager :
		public MyGUI::Singleton<UndoManager>
	{
	public:
		UndoManager();

	public:
		void initialise(EditorWidgets * mEditorWidgets);
		void shutdown();

		void undo();
		void redo();
		void addValue(int _property = PR_DEFAULT);

		void dropLastProperty() { mLastProperty = PR_DEFAULT; }

		bool isUnsaved() const { return mUnsaved; }
		void setUnsaved(bool _unsaved);

		Event_Changes eventChanges;

	private:
		void commandUndo(const MyGUI::UString& _commandName);
		void commandRedo(const MyGUI::UString& _commandName);

	private:
		// position in the bufer (0 - newest element)
		size_t mPosition;

		CyclicBuffer<MyGUI::xml::Document*> mOperations;
		int mLastProperty;

		EditorWidgets* mEditorWidgets;

		bool mUnsaved;
	};

} // namespace tools

#endif // __UNDO_MANAGER_H__
