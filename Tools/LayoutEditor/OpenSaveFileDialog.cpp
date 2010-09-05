/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "precompiled.h"
#include "OpenSaveFileDialog.h"

#include "FileSystemInfo/FileSystemInfo.h"

namespace tools
{
	OpenSaveFileDialog::OpenSaveFileDialog() :
		Dialog("OpenSaveFileDialog.layout"),
		mWindow(nullptr),
		mListFiles(nullptr),
		mEditFileName(nullptr),
		mEditCurrentFolder(nullptr),
		mButtonOpenSave(nullptr),
		mFolderMode(false)
	{
		assignWidget(mListFiles, "ListFiles");
		assignWidget(mEditFileName, "EditFileName");
		assignWidget(mEditCurrentFolder, "EditCurrentFolder");
		assignWidget(mButtonOpenSave, "ButtonOpenSave");

		mListFiles->eventListChangePosition += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyListChangePosition);
		mListFiles->eventListSelectAccept += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyListSelectAccept);
		mEditFileName->eventEditSelectAccept += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyEditSelectAccept);
		mEditCurrentFolder->eventEditSelectAccept += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyDirectoryAccept);
		mButtonOpenSave->eventMouseButtonClick += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyMouseButtonClick);

		mWindow = mMainWidget->castType<MyGUI::Window>();
		mWindow->eventWindowButtonPressed += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyWindowButtonPressed);

		mFileMask = L"*.*";
		mCurrentFolder = common::getSystemCurrentFolder();

		mMainWidget->setVisible(false);

		update();
	}

	void OpenSaveFileDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		if (_name == "close")
			eventEndDialog(this, false);
	}

	void OpenSaveFileDialog::notifyDirectoryAccept(MyGUI::Edit* _sender)
	{
		setCurrentFolder(_sender->getOnlyText());
	}

	void OpenSaveFileDialog::notifyEditSelectAccept(MyGUI::Edit* _sender)
	{
		accept();
	}

	void OpenSaveFileDialog::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		accept();
	}

	void OpenSaveFileDialog::setDialogInfo(const MyGUI::UString& _caption, const MyGUI::UString& _button, bool _folderMode)
	{
		mFolderMode = _folderMode;
		mWindow->setCaption(_caption);
		mButtonOpenSave->setCaption(_button);
		mEditFileName->setVisible(!_folderMode);
	}

	void OpenSaveFileDialog::notifyListChangePosition(MyGUI::List* _sender, size_t _index)
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

	void OpenSaveFileDialog::notifyListSelectAccept(MyGUI::List* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE) return;

		common::FileInfo info = *_sender->getItemDataAt<common::FileInfo>(_index);
		if (info.folder)
		{
			if (info.name == L"..")
			{
				size_t index = mCurrentFolder.find_last_of(L"\\/");
				if (index != std::string::npos)
				{
					mCurrentFolder = mCurrentFolder.substr(0, index);
				}
			}
			else
			{
				mCurrentFolder = common::concatenatePath (mCurrentFolder.asWStr(), info.name);
			}

			update();
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
			eventEndDialog(this, true);
		}
	}

	void OpenSaveFileDialog::setCurrentFolder(const MyGUI::UString& _folder)
	{
		mCurrentFolder = _folder.empty() ? MyGUI::UString(common::getSystemCurrentFolder()) : _folder;

		update();
	}

	void OpenSaveFileDialog::update()
	{
		mEditCurrentFolder->setCaption(mCurrentFolder);

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

	void OpenSaveFileDialog::onDoModal()
	{
		update();

		MyGUI::IntSize windowSize = mMainWidget->getSize();
		MyGUI::IntSize parentSize = mMainWidget->getParentSize();

		mMainWidget->setPosition((parentSize.width - windowSize.width) / 2, (parentSize.height - windowSize.height) / 2);
	}

	void OpenSaveFileDialog::onEndModal()
	{
	}

} // namespace tools
