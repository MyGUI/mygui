/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/

#ifndef _75bf3bc8_5f9f_4d93_ad22_949d0cd2f50b_
#define _75bf3bc8_5f9f_4d93_ad22_949d0cd2f50b_

#include <MyGUI.h>
#include "Dialog.h"
#include "Control.h"

namespace tools
{

	class MYGUI_EXPORT_DLL OpenSaveFileDialog :
		public Dialog,
		public Control,
		public sigslot::has_slots<>
	{
	public:
		OpenSaveFileDialog();
		virtual ~OpenSaveFileDialog();

		void setDialogInfo(const MyGUI::UString& _caption, const MyGUI::UString& _button, bool _folderMode = false);

		void setCurrentFolder(const MyGUI::UString& _value);
		const MyGUI::UString& getCurrentFolder() const;

		void setFileName(const MyGUI::UString& _value);
		const MyGUI::UString& getFileName() const;

		const MyGUI::UString& getMode() const;
		void setMode(const MyGUI::UString& _value);

		typedef std::vector<MyGUI::UString> VectorUString;
		void setRecentFolders(const VectorUString& _listFolders);

		void setFileMask(const MyGUI::UString& _value);
		const MyGUI::UString& getFileMask() const;

	protected:
		virtual void onDoModal();
		virtual void onEndModal();

		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		bool checkCommand();
		void commandOpenSaveAccept(const MyGUI::UString& _commandName, bool& _result);
		void commandOpenSaveCancel(const MyGUI::UString& _commandName, bool& _result);
		void commandOpenSaveFolderUp(const MyGUI::UString& _commandName, bool& _result);

		void notifyDirectoryComboAccept(MyGUI::ComboBox* _sender, size_t _index);
		void notifyDirectoryComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index);
		void notifyListSelectAccept(MyGUI::ListBox* _sender, size_t _index);

		void update();
		void accept();

		void upFolder();

	private:
		MyGUI::ListBox* mListFiles;
		MyGUI::EditBox* mEditFileName;
		MyGUI::ComboBox* mCurrentFolderField;
		MyGUI::Button* mButtonOpenSave;

		MyGUI::UString mCurrentFolder;
		MyGUI::UString mFileName;
		MyGUI::UString mFileMask;

		MyGUI::UString mMode;
		bool mFolderMode;
	};

}

#endif
