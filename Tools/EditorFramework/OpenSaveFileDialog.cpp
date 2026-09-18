/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#include "Precompiled.h"
#include "OpenSaveFileDialog.h"
#include "FileSystemInfo.h"
#include "CommandManager.h"
#include "MyGUI_FileSystemUtility.h"

namespace tools
{

	void OpenSaveFileDialog::OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		setDialogRoot(mMainWidget);

		assignWidget(mListFiles, "ListFiles");
		assignWidget(mEditFileName, "EditFileName");
		assignWidget(mCurrentFolderField, "CurrentFolder");
		assignWidget(mButtonOpenSave, "ButtonOpenSave");

		mListFiles->eventListChangePosition += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyListChangePosition);
		mListFiles->eventListSelectAccept += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyListSelectAccept);
		mCurrentFolderField->eventComboAccept +=
			MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyDirectoryComboAccept);
		mCurrentFolderField->eventComboChangePosition +=
			MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyDirectoryComboChangePosition);

		mCurrentFolder = std::filesystem::current_path();

		CommandManager::getInstance()
			.getEvent("Command_OpenSaveAccept")
			->connect(this, &OpenSaveFileDialog::commandOpenSaveAccept);
		CommandManager::getInstance()
			.getEvent("Command_OpenSaveCancel")
			->connect(this, &OpenSaveFileDialog::commandOpenSaveCancel);
		CommandManager::getInstance()
			.getEvent("Command_OpenSaveFolderUp")
			->connect(this, &OpenSaveFileDialog::commandOpenSaveFolderUp);

		mMainWidget->setVisible(false);

		update();
	}

	void OpenSaveFileDialog::setDialogInfo(
		const MyGUI::UString& _caption,
		const MyGUI::UString& _button,
		bool _folderMode)
	{
		mFolderMode = _folderMode;

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->setCaption(_caption);

		mButtonOpenSave->setCaption(_button);
		mEditFileName->setVisible(!_folderMode);
	}

	void OpenSaveFileDialog::notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE)
		{
			mEditFileName->setCaption(MyGUI::UString());
		}
		else
		{
			common::FileInfo info = *_sender->getItemDataAt<common::FileInfo>(_index);
			if (!info.folder)
				mEditFileName->setCaption(MyGUI::utility::pathToUTF8(info.name));
		}
	}

	void OpenSaveFileDialog::notifyListSelectAccept(MyGUI::ListBox* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE)
			return;

		common::FileInfo info = *_sender->getItemDataAt<common::FileInfo>(_index);
		if (info.folder)
		{
			if (info.name == "..")
			{
				upFolder();
			}
			else
			{
				mCurrentFolder /= info.name;
				update();
			}
		}
		else
		{
			accept();
		}
	}

	void OpenSaveFileDialog::accept()
	{
		if (!mFolderMode)
		{
			mFileName = mEditFileName->getOnlyText();
			if (!mFileName.empty())
				eventEndDialog(this, true);
		}
		else
		{
			if (mListFiles->getIndexSelected() != MyGUI::ITEM_NONE)
			{
				common::FileInfo info = *mListFiles->getItemDataAt<common::FileInfo>(mListFiles->getIndexSelected());
				if (info.name != "..")
					mCurrentFolder /= info.name;
			}
			eventEndDialog(this, true);
		}
	}

	void OpenSaveFileDialog::upFolder()
	{
		mCurrentFolder = mCurrentFolder.parent_path();
		update();
	}

	void OpenSaveFileDialog::setCurrentFolder(const std::filesystem::path& _folder)
	{
		mCurrentFolder = _folder.empty() ? std::filesystem::current_path() : _folder;

		update();
	}

	void OpenSaveFileDialog::update()
	{
		if (mCurrentFolder.empty())
			mCurrentFolder = "/";
		mCurrentFolderField->setCaption(MyGUI::utility::pathToUTF8(mCurrentFolder));

		mListFiles->removeAllItems();

		// add all folders first
		common::VectorFileInfo infos = common::getSystemFileList(mCurrentFolder, "*");

		for (const auto& info : infos)
		{
			if (info.folder)
				mListFiles->addItem("[" + MyGUI::utility::pathToUTF8(info.name) + "]", info);
		}

		if (!mFolderMode)
		{
			// add files by given mask
			infos = common::getSystemFileList(mCurrentFolder, MyGUI::utility::pathFromUTF8(mFileMask.asUTF8()));

			for (const auto& info : infos)
			{
				if (!info.folder)
					mListFiles->addItem(MyGUI::utility::pathToUTF8(info.name), info);
			}
		}
	}

	void OpenSaveFileDialog::setFileName(const MyGUI::UString& _value)
	{
		mFileName = _value;
		mEditFileName->setCaption(_value);
	}

	void OpenSaveFileDialog::setFileMask(const MyGUI::UString& _value)
	{
		mFileMask = _value;
		update();
	}

	const MyGUI::UString& OpenSaveFileDialog::getFileMask() const
	{
		return mFileMask;
	}

	void OpenSaveFileDialog::onDoModal()
	{
		update();
	}

	void OpenSaveFileDialog::onEndModal()
	{
	}

	void OpenSaveFileDialog::notifyDirectoryComboAccept(MyGUI::ComboBox* _sender, size_t _index)
	{
		setCurrentFolder(MyGUI::utility::pathFromUTF8(_sender->getOnlyText().asUTF8()));
	}

	const std::filesystem::path& OpenSaveFileDialog::getCurrentFolder() const
	{
		return mCurrentFolder;
	}

	const MyGUI::UString& OpenSaveFileDialog::getFileName() const
	{
		return mFileName;
	}

	const MyGUI::UString& OpenSaveFileDialog::getMode() const
	{
		return mMode;
	}

	void OpenSaveFileDialog::setMode(const MyGUI::UString& _value)
	{
		mMode = _value;
	}

	void OpenSaveFileDialog::setRecentFolders(const VectorUString& _listFolders)
	{
		mCurrentFolderField->removeAllItems();

		for (const auto& folder : _listFolders)
			mCurrentFolderField->addItem(folder);
	}

	void OpenSaveFileDialog::notifyDirectoryComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		if (_index != MyGUI::ITEM_NONE)
			setCurrentFolder(MyGUI::utility::pathFromUTF8(_sender->getItemNameAt(_index).asUTF8()));
	}

	bool OpenSaveFileDialog::checkCommand()
	{
		return isDialogModal();
	}

	void OpenSaveFileDialog::commandOpenSaveAccept(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		accept();

		_result = true;
	}

	void OpenSaveFileDialog::commandOpenSaveCancel(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		eventEndDialog(this, false);

		_result = true;
	}

	void OpenSaveFileDialog::commandOpenSaveFolderUp(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		upFolder();

		_result = true;
	}

}
