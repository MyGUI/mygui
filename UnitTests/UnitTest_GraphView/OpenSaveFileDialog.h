/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef OPEN_SAVE_FILE_DIALOG_H_
#define OPEN_SAVE_FILE_DIALOG_H_

#include <MyGUI.h>
#include "Dialog.h"

namespace tools
{
	class OpenSaveFileDialog : public Dialog
	{
	public:
		OpenSaveFileDialog();

		void setDialogInfo(const MyGUI::UString& _caption, const MyGUI::UString& _button, bool _folderMode = false);

		void setCurrentFolder(const MyGUI::UString& _value);
		const MyGUI::UString& getCurrentFolder() const;

		void setFileName(const MyGUI::UString& _value);
		const MyGUI::UString& getFileName() const;

		const MyGUI::UString& getMode() const;
		void setMode(const MyGUI::UString& _value);

		using VectorUString = std::vector<MyGUI::UString>;
		void setRecentFolders(const VectorUString& _listFolders);

		void setFileMask(const MyGUI::UString& _value);
		const MyGUI::UString& getFileMask() const;

	protected:
		void onDoModal() override;
		void onEndModal() override;

	private:
		void notifyWindowButtonPressed(MyGUI::Window* _sender, std::string_view _name);
		void notifyDirectoryComboAccept(MyGUI::ComboBox* _sender, size_t _index);
		void notifyDirectoryComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index);
		void notifyListSelectAccept(MyGUI::ListBox* _sender, size_t _index);
		void notifyEditSelectAccept(MyGUI::EditBox* _sender);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyUpButtonClick(MyGUI::Widget* _sender);

		void update();
		void accept();

		void upFolder();

	private:
		MyGUI::Window* mWindow{nullptr};
		MyGUI::ListBox* mListFiles{nullptr};
		MyGUI::EditBox* mEditFileName{nullptr};
		MyGUI::Button* mButtonUp{nullptr};
		MyGUI::ComboBox* mCurrentFolderField{nullptr};
		MyGUI::Button* mButtonOpenSave{nullptr};

		MyGUI::UString mCurrentFolder;
		MyGUI::UString mFileName;
		MyGUI::UString mFileMask;

		MyGUI::UString mMode;
		bool mFolderMode{false};
	};

} // namespace tools

#endif // OPEN_SAVE_FILE_DIALOG_H_
