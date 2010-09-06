/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __OPEN_SAVE_FILE_DIALOG_H__
#define __OPEN_SAVE_FILE_DIALOG_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace common
{

	class OpenSaveFileDialog : public wraps::BaseLayout
	{
	public:
		typedef MyGUI::delegates::CDelegate2<OpenSaveFileDialog*, bool> EventHandle_Result;

	public:
		OpenSaveFileDialog();

		void setDialogInfo(const MyGUI::UString& _caption, const MyGUI::UString& _button);

		void setCurrentFolder(const MyGUI::UString& _value = "");
		const MyGUI::UString& getCurrentFolder()
		{
			return mCurrentFolder;
		}

		void setFileMask(const MyGUI::UString& _value);
		const MyGUI::UString& getFileMask()
		{
			return mFileMask;
		}

		void setFileName(const MyGUI::UString& _value);
		const MyGUI::UString& getFileName()
		{
			return mFileName;
		}

		void setVisible(bool _value);
		bool isVisible()
		{
			return mMainWidget->getVisible();
		}

		void setModalMode(bool _value);
		bool getModalMode();

		EventHandle_Result eventEndDialog;

	private:
		void notifyListChangePosition(MyGUI::ListPtr _sender, size_t _index);
		void notifyListSelectAccept(MyGUI::ListPtr _sender, size_t _index);
		void notifyEditSelectAccept(MyGUI::EditPtr _sender);
		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);
		void notifyWindowButtonPressed(MyGUI::WindowPtr _sender, const std::string& _name);

		void update();
		void accept();

	private:
		MyGUI::ListPtr mListFiles;
		MyGUI::EditPtr mEditFileName;
		MyGUI::EditPtr mEditCurrentFolder;
		MyGUI::ButtonPtr mButtonOpenSave;

		MyGUI::UString mCurrentFolder;
		MyGUI::UString mFileName;
		MyGUI::UString mFileMask;

		bool mModalMode;
	};

} // namespace common

#endif // __OPEN_SAVE_FILE_DIALOG_H__
