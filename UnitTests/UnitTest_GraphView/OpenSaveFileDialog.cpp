/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "OpenSaveFileDialog.h"

#include "FileSystemInfo/FileSystemInfo.h"

namespace tools
{
	OpenSaveFileDialog::OpenSaveFileDialog() :
		Dialog("OpenSaveFileDialog.layout"),
		mFileMask("*.*")
	{
		assignWidget(mListFiles, "ListFiles");
		assignWidget(mEditFileName, "EditFileName");
		assignWidget(mButtonUp, "ButtonUp");
		assignWidget(mCurrentFolderField, "CurrentFolder");
		assignWidget(mButtonOpenSave, "ButtonOpenSave");

		mListFiles->eventListChangePosition += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyListChangePosition);
		mListFiles->eventListSelectAccept += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyListSelectAccept);
		mEditFileName->eventEditSelectAccept += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyEditSelectAccept);
		mButtonUp->eventMouseButtonClick += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyUpButtonClick);
		mCurrentFolderField->eventComboAccept +=
			MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyDirectoryComboAccept);
		mCurrentFolderField->eventComboChangePosition +=
			MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyDirectoryComboChangePosition);
		mButtonOpenSave->eventMouseButtonClick += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyMouseButtonClick);

		mWindow = mMainWidget->castType<MyGUI::Window>();
		mWindow->eventWindowButtonPressed += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyWindowButtonPressed);

		mCurrentFolder = common::getSystemCurrentFolder();

		mMainWidget->setVisible(false);

		update();
	}

	void OpenSaveFileDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, std::string_view _name)
	{
		if (_name == "close")
			eventEndDialog(this, false);
	}

	void OpenSaveFileDialog::notifyEditSelectAccept(MyGUI::EditBox* _sender)
	{
		accept();
	}

	void OpenSaveFileDialog::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		accept();
	}

	void OpenSaveFileDialog::notifyUpButtonClick(MyGUI::Widget* _sender)
	{
		upFolder();
	}

	void OpenSaveFileDialog::setDialogInfo(
		const MyGUI::UString& _caption,
		const MyGUI::UString& _button,
		bool _folderMode)
	{
		mFolderMode = _folderMode;
		mWindow->setCaption(_caption);
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
				mEditFileName->setCaption(info.name);
		}
	}

	void OpenSaveFileDialog::notifyListSelectAccept(MyGUI::ListBox* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE)
			return;

		common::FileInfo info = *_sender->getItemDataAt<common::FileInfo>(_index);
		if (info.folder)
		{
			if (info.name == L"..")
			{
				upFolder();
			}
			else
			{
				mCurrentFolder = common::concatenatePath(mCurrentFolder.asWStr(), info.name);
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
					mCurrentFolder = common::concatenatePath(mCurrentFolder.asWStr(), info.name);
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

		for (auto& info : infos)
		{
			if (info.folder)
				mListFiles->addItem(L"[" + info.name + L"]", info);
		}

		if (!mFolderMode)
		{
			// add files by given mask
			infos.clear();
			getSystemFileList(infos, mCurrentFolder, mFileMask);

			for (auto& info : infos)
			{
				if (!info.folder)
					mListFiles->addItem(info.name, info);
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

		MyGUI::IntSize windowSize = mMainWidget->getSize();
		MyGUI::IntSize parentSize = mMainWidget->getParentSize();

		mMainWidget->setPosition(
			(parentSize.width - windowSize.width) / 2,
			(parentSize.height - windowSize.height) / 2);
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

		for (const auto& _listFolder : _listFolders)
			mCurrentFolderField->addItem(_listFolder);
	}

	void OpenSaveFileDialog::notifyDirectoryComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		if (_index != MyGUI::ITEM_NONE)
			setCurrentFolder(_sender->getItemNameAt(_index));
	}

} // namespace tools
