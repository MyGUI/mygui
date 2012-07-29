#ifndef _a192a99d_7dc5_414c_9b85_5345449cb7f6_
#define _a192a99d_7dc5_414c_9b85_5345449cb7f6_

#include "EditorWidgets.h"
#include "CyclicBuffer.h"
#include "sigslot.h"

namespace tools
{

	enum { PR_DEFAULT, PR_POSITION, PR_PROPERTIES, PR_KEY_POSITION };

	typedef MyGUI::delegates::CMultiDelegate1<bool> Event_Changes;

	class UndoManager :
		public MyGUI::Singleton<UndoManager>,
		public sigslot::has_slots<>
	{
	public:
		UndoManager();

	public:
		void initialise(EditorWidgets* mEditorWidgets);
		void shutdown();

		void undo();
		void redo();
		void addValue(int _property = PR_DEFAULT);

		void dropLastProperty()
		{
			mLastProperty = PR_DEFAULT;
		}

		bool isUnsaved() const
		{
			return mUnsaved;
		}
		void setUnsaved(bool _unsaved);

		Event_Changes eventChanges;

	private:
		void commandUndo(const MyGUI::UString& _commandName, bool& _result);
		void commandRedo(const MyGUI::UString& _commandName, bool& _result);

	private:
		// position in the bufer (0 - newest element)
		size_t mPosition;

		CyclicBuffer<MyGUI::xml::Document*> mOperations;
		int mLastProperty;

		EditorWidgets* mEditorWidgets;

		bool mUnsaved;
	};

}

#endif
