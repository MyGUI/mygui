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

		void setCurrentFolder(const MyGUI::UString& _value);
		const MyGUI::UString& getCurrentFolder();

		void setFileName(const MyGUI::UString& _value);
		const MyGUI::UString& getFileName() const;

		const MyGUI::UString& getMode();
		void setMode(const MyGUI::UString& _value);

		typedef std::vector<MyGUI::UString> VectorUString;
		void setRecentFilders(const VectorUString& _listFolders);

		void addFileMask(const MyGUI::UString& _value);
		void clearFileMasks();

	protected:
		virtual void onDoModal();
		virtual void onEndModal();

	private:
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);
		void notifyDirectoryComboAccept(MyGUI::ComboBox* _sender, size_t _index);
		void notifyDirectoryComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
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
		MyGUI::ComboBox* mCurrentFolderField;
		MyGUI::Button* mButtonOpenSave;

		MyGUI::UString mCurrentFolder;
		MyGUI::UString mFileName;
		VectorUString mFileMasks;

		MyGUI::UString mMode;
		bool mFolderMode;
	};

} // namespace tools

#endif // __OPEN_SAVE_FILE_DIALOG_H__
