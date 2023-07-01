#ifndef _a192a99d_7dc5_414c_9b85_5345449cb7f6_
#define _a192a99d_7dc5_414c_9b85_5345449cb7f6_

#include "EditorWidgets.h"
#include "CyclicBuffer.h"
#include "sigslot.h"

namespace tools
{

	enum
	{
		PR_DEFAULT,
		PR_POSITION,
		PR_PROPERTIES,
		PR_KEY_POSITION
	};

	using Event_Changes = MyGUI::delegates::MultiDelegate<bool>;

	class UndoManager : public sigslot::has_slots<>
	{
		MYGUI_SINGLETON_DECLARATION(UndoManager);

	public:
		UndoManager();

	public:
		void initialise(EditorWidgets* _ew);
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
		void setUnsaved(bool _value);

		Event_Changes eventChanges;

	private:
		void commandUndo(const MyGUI::UString& _commandName, bool& _result);
		void commandRedo(const MyGUI::UString& _commandName, bool& _result);

	private:
		// position in the bufer (0 - newest element)
		size_t mPosition{0};

		CyclicBuffer<MyGUI::xml::Document*> mOperations;
		int mLastProperty{0};

		EditorWidgets* mEditorWidgets{nullptr};

		bool mUnsaved{false};
	};

}

#endif
