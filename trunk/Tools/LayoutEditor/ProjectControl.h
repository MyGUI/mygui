/*!
	@file
	@author		Albert Semenov
	@date		10/2010
*/
#ifndef __PEOJECT_CONTROL_H__
#define __PEOJECT_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
//#include "EditorToolTip.h"
#include "OpenSaveFileDialog.h"

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

		bool checkCommand();

		void command_ProjectLoad(const MyGUI::UString& _commandName, bool& _result);
		void command_ProjectClose(const MyGUI::UString& _commandName, bool& _result);

		void clear();
		bool load();

		void setFileName(const MyGUI::UString& _filePath, const MyGUI::UString& _fileName);
		void updateCaption();

	private:
		OpenSaveFileDialog* mOpenSaveFileDialog;
		MyGUI::UString mProjectName;
		MyGUI::UString mProjectPath;

		MyGUI::List* mList;
		MyGUI::StaticText* mProjectNameText;
	};

} // namespace tools

#endif // __PEOJECT_CONTROL_H__
