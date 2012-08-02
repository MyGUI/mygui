/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#include "Precompiled.h"
#include "OpenSaveFileDialog.h"
#include "FileSystemInfo/FileSystemInfo.h"
#include "CommandManager.h"

namespace tools
{

	OpenSaveFileDialog::OpenSaveFileDialog() :
		mListFiles(nullptr),
		mEditFileName(nullptr),
		mCurrentFolderField(nullptr),
		mButtonOpenSave(nullptr),
		mFileMask("*.*"),
		mFolderMode(false)
	{
	}

	OpenSaveFileDialog::~OpenSaveFileDialog()
	{
	}

	void OpenSaveFileDialog::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		setDialogRoot(mMainWidget);

		assignWidget(mListFiles, "ListFiles");
		assignWidget(mEditFileName, "EditFileName");
		assignWidget(mCurrentFolderField, "CurrentFolder");
		assignWidget(mButtonOpenSave, "ButtonOpenSave");

		mListFiles->eventListChangePosition += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyListChangePosition);
		mListFiles->eventListSelectAccept += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyListSelectAccept);
		mCurrentFolderField->eventComboAccept += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyDirectoryComboAccept);
		mCurrentFolderField->eventComboChangePosition += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyDirectoryComboChangePosition);

		mCurrentFolder = common::getSystemCurrentFolder();

		CommandManager::getInstance().getEvent("Command_OpenSaveAccept")->connect(this, &OpenSaveFileDialog::commandOpenSaveAccept);
		CommandManager::getInstance().getEvent("Command_OpenSaveCancel")->connect(this, &OpenSaveFileDialog::commandOpenSaveCancel);
		CommandManager::getInstance().getEvent("Command_OpenSaveFolderUp")->connect(this, &OpenSaveFileDialog::commandOpenSaveFolderUp);

		mMainWidget->setVisible(false);

		update();
	}

	void OpenSaveFileDialog::setDialogInfo(const MyGUI::UString& _caption, const MyGUI::UString& _button, bool _folderMode)
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
			mEditFileName->setCaption("");
		}
		else
		{
			common::FileInfo info = *_sender->getItemDataAt<common::FileInfo>(_index);
			if (!info.folder)
				mEditFileName->setCaption(info.name);
		}
	}

	void OpenSaveFileDialog::notifyListSelectAccept(MyGUI::ListBox* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE) return;

		common::FileInfo info = *_sender->getItemDataAt<common::FileInfo>(_index);
		if (info.folder)
		{
			if (info.name == L"..")
			{
				upFolder();
			}
			else
			{
				mCurrentFolder = common::concatenatePath (mCurrentFolder.asWStr(), info.name);
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
				if (!common::isParentDir(info.name.c_str()))
					mCurrentFolder = common::concatenatePath (mCurrentFolder.asWStr(), info.name);
			}
			eventEndDialog(this, true);
		}
	}

	void OpenSaveFileDialog::upFolder()
	{
		size_t index = mCurrentFolder.find_last_of(L"\\/");
		if (index != std::string::npos)
		{
			mCurrentFolder = mCurrentFolder.substr(0, index);
		}
		update();
	}

	void OpenSaveFileDialog::setCurrentFolder(const MyGUI::UString& _folder)
	{
		mCurrentFolder = _folder.empty() ? MyGUI::UString(common::getSystemCurrentFolder()) : _folder;

		update();
	}

	void OpenSaveFileDialog::update()
	{
		if (mCurrentFolder.empty())
			mCurrentFolder = "/";
		mCurrentFolderField->setCaption(mCurrentFolder);

		mListFiles->removeAllItems();

		// add all folders first
		common::VectorFileInfo infos;
		getSystemFileList(infos, mCurrentFolder, L"*.*");

		for (common::VectorFileInfo::iterator item = infos.begin(); item != infos.end(); ++item)
		{
			if ((*item).folder)
				mListFiles->addItem(L"[" + (*item).name + L"]", *item);
		}

		if (!mFolderMode)
		{
			// add files by given mask
			infos.clear();
			getSystemFileList(infos, mCurrentFolder, mFileMask);

			for (common::VectorFileInfo::iterator item = infos.begin(); item != infos.end(); ++item)
			{
				if (!(*item).folder)
					mListFiles->addItem((*item).name, *item);
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
		setCurrentFolder(_sender->getOnlyText());
	}

	const MyGUI::UString& OpenSaveFileDialog::getCurrentFolder() const
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

		for (VectorUString::const_iterator item = _listFolders.begin(); item != _listFolders.end(); ++ item)
			mCurrentFolderField->addItem((*item));
	}

	void OpenSaveFileDialog::notifyDirectoryComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		if (_index != MyGUI::ITEM_NONE)
			setCurrentFolder(_sender->getItemNameAt(_index));
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
