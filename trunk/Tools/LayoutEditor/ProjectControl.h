/*!
	@file
	@author		Albert Semenov
	@date		10/2010
*/

#ifndef _6b0d9374_eb9a_4e43_abf5_7987b55a6c4a_
#define _6b0d9374_eb9a_4e43_abf5_7987b55a6c4a_

#include "BaseLayout/BaseLayout.h"
#include "OpenSaveFileDialog.h"
#include "TextFieldControl.h"
#include "EditorToolTip.h"
#include "MessageBox/MessageBox.h"
#include "sigslot.h"

namespace tools
{

	class ProjectControl :
		public wraps::BaseLayout,
		public sigslot::has_slots<>
	{
	public:
		ProjectControl(MyGUI::Widget* _parent = nullptr);
		virtual ~ProjectControl();

	private:
		void notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result);
		void notifyMessageBoxResultDelete(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyTextFieldEndDialog(Dialog* _sender, bool _result);
		void notifyListSelectAccept(MyGUI::ListBox* _sender, size_t _index);
		void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);

		bool checkCommand();

		void command_ProjectCreate(const MyGUI::UString& _commandName, bool& _result);
		void command_ProjectLoad(const MyGUI::UString& _commandName, bool& _result);
		void command_ProjectClose(const MyGUI::UString& _commandName, bool& _result);
		void command_ProjectDeleteItem(const MyGUI::UString& _commandName, bool& _result);
		void command_ProjectRenameItem(const MyGUI::UString& _commandName, bool& _result);
		void command_ProjectAddItem(const MyGUI::UString& _commandName, bool& _result);
		void command_UpdateResources(const MyGUI::UString& _commandName, bool& _result);
		void command_OpenRecentProject(const MyGUI::UString& _commandName, bool& _result);

		void clear();
		bool load();

		void setFileName(const MyGUI::UString& _filePath, const MyGUI::UString& _fileName);
		void updateCaption();

		bool deleteItemFromProject(size_t _index);
		bool renameItemInProject(size_t _index, const MyGUI::UString& _name);
		bool addItemToProject(const MyGUI::UString& _name, size_t& _index);
		void saveItemToProject();

		bool isExistFile(const MyGUI::UString& _filePath, const MyGUI::UString& _fileName);
		void createProject(const MyGUI::UString& _filePath, const MyGUI::UString& _fileName);

		bool isProjectItemOpen();
		void loadLastProject();

		SkinInfo getCellData(MyGUI::Widget* _sender, size_t _index);
		bool checkItem(const std::string& _name, const MyGUI::VectorString& _items);
		bool checkTemplate(const std::string& _skinName);

		void updateProjectSkins();

	private:
		OpenSaveFileDialog* mOpenSaveFileDialog;
		TextFieldControl* mTextFieldControl;

		MyGUI::UString mProjectName;
		MyGUI::UString mProjectPath;

		MyGUI::ListBox* mList;
		MyGUI::TextBox* mProjectNameText;
	};

}

#endif
