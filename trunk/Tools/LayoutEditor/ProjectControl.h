/*!
	@file
	@author		Albert Semenov
	@date		10/2010
*/
#ifndef __PEOJECT_CONTROL_H__
#define __PEOJECT_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "OpenSaveFileDialog.h"
#include "TextFieldControl.h"

namespace tools
{
	class ProjectControl :
		public wraps::BaseLayout
	{
	public:
		ProjectControl(MyGUI::Widget* _parent = nullptr);
		virtual ~ProjectControl();

	private:
		void notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result);
		void notifyMessageBoxResultDelete(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyTextFieldEndDialog(Dialog* _sender, bool _result);
		void notifyListSelectAccept(MyGUI::List* _sender, size_t _index);

		bool checkCommand();

		void command_ProjectCreate(const MyGUI::UString& _commandName, bool& _result);
		void command_ProjectLoad(const MyGUI::UString& _commandName, bool& _result);
		void command_ProjectClose(const MyGUI::UString& _commandName, bool& _result);
		void command_ProjectDeleteItem(const MyGUI::UString& _commandName, bool& _result);
		void command_ProjectRenameItem(const MyGUI::UString& _commandName, bool& _result);
		void command_ProjectAddItem(const MyGUI::UString& _commandName, bool& _result);
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

	private:
		OpenSaveFileDialog* mOpenSaveFileDialog;
		TextFieldControl* mTextFieldControl;

		MyGUI::UString mProjectName;
		MyGUI::UString mProjectPath;

		MyGUI::List* mList;
		MyGUI::StaticText* mProjectNameText;
	};

} // namespace tools

#endif // __PEOJECT_CONTROL_H__
