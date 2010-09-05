/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __OPEN_SAVE_FILE_DIALOG_H__
#define __OPEN_SAVE_FILE_DIALOG_H__

#include <MyGUI.h>
#include "Dialog.h"

namespace tools
{
	class OpenSaveFileDialog :
		public Dialog
	{
	public:
		OpenSaveFileDialog();

		void setDialogInfo(const MyGUI::UString& _caption, const MyGUI::UString& _button, bool _folderMode = false);

		void setCurrentFolder(const MyGUI::UString& _value = "");
		const MyGUI::UString& getCurrentFolder()
		{
			return mCurrentFolder;
		}

		void setFileMask(const MyGUI::UString& _value);
		const MyGUI::UString& getFileMask() const
		{
			return mFileMask;
		}

		void setFileName(const MyGUI::UString& _value);
		const MyGUI::UString& getFileName() const
		{
			return mFileName;
		}

		const MyGUI::UString& getMode()
		{
			return mMode;
		}
		void setMode(const MyGUI::UString& _value)
		{
			mMode = _value;
		}

	protected:
		virtual void onDoModal();
		virtual void onEndModal();

	private:
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);
		void notifyDirectoryAccept(MyGUI::Edit* _sender);
		void notifyListChangePosition(MyGUI::List* _sender, size_t _index);
		void notifyListSelectAccept(MyGUI::List* _sender, size_t _index);
		void notifyEditSelectAccept(MyGUI::Edit* _sender);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

		void update();
		void accept();

	private:
		MyGUI::Window* mWindow;
		MyGUI::List* mListFiles;
		MyGUI::Edit* mEditFileName;
		MyGUI::Edit* mEditCurrentFolder;
		MyGUI::Button* mButtonOpenSave;

		MyGUI::UString mCurrentFolder;
		MyGUI::UString mFileName;
		MyGUI::UString mFileMask;

		MyGUI::UString mMode;
		bool mFolderMode;
	};

} // namespace tools

#endif // __OPEN_SAVE_FILE_DIALOG_H__
