/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "Precompiled.h"
#include "OpenSaveFileDialog.h"

#include "FileSystemInfo/FileSystemInfo.h"

namespace common
{

	OpenSaveFileDialog::OpenSaveFileDialog() : BaseLayout("OpenSaveFileDialog.layout")
	{
		assignWidget(mListFiles, "ListFiles");
		assignWidget(mEditFileName, "EditFileName");
		assignWidget(mEditCurrentFolder, "EditCurrentFolder");
		assignWidget(mButtonOpenSave, "ButtonOpenSave");

		mListFiles->eventListChangePosition += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyListChangePosition);
		mListFiles->eventListSelectAccept += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyListSelectAccept);
		mEditFileName->eventEditSelectAccept += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyEditSelectAccept);
		mButtonOpenSave->eventMouseButtonClick += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyMouseButtonClick);

		mMainWidget->castType<MyGUI::Window>()->eventWindowButtonPressed += MyGUI::newDelegate(this, &OpenSaveFileDialog::notifyWindowButtonPressed);

		mModalMode = false;

		mFileMask = L"*.*";
		mCurrentFolder = getSystemCurrentFolder();

		update();
	}

	void OpenSaveFileDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
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

	void OpenSaveFileDialog::setDialogInfo(const MyGUI::UString& _caption, const MyGUI::UString& _button)
	{
		mMainWidget->castType<MyGUI::Window>()->setCaption(_caption);
		mButtonOpenSave->setCaption(_button);
	}

	void OpenSaveFileDialog::notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE)
		{
			mEditFileName->setCaption("");
		}
		else
		{
			FileInfo info = *_sender->getItemDataAt<FileInfo>(_index);
			if (!info.folder)
				mEditFileName->setCaption(info.name);
		}
	}

	void OpenSaveFileDialog::notifyListSelectAccept(MyGUI::ListBox* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE) return;

		FileInfo info = *_sender->getItemDataAt<FileInfo>(_index);
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
				mCurrentFolder = concatenatePath (mCurrentFolder.asWStr(), info.name);
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
		mFileName = mEditFileName->getCaption();
		if (!mFileName.empty())
			eventEndDialog(this, true);
	}

	void OpenSaveFileDialog::setCurrentFolder(const MyGUI::UString& _folder)
	{
		mCurrentFolder = _folder.empty() ? MyGUI::UString(getSystemCurrentFolder()) : _folder;

		update();
	}

	void OpenSaveFileDialog::update()
	{
		mEditCurrentFolder->setCaption(mCurrentFolder);

		mListFiles->removeAllItems();

		// add all folders first
		VectorFileInfo infos;
		getSystemFileList(infos, mCurrentFolder, L"*.*");

		for (VectorFileInfo::iterator item = infos.begin(); item != infos.end(); ++item)
		{
			if ((*item).folder)
				mListFiles->addItem(L"[" + (*item).name + L"]", *item);
		}

		// add files by given mask
		infos.clear();
		getSystemFileList(infos, mCurrentFolder, mFileMask);

		for (VectorFileInfo::iterator item = infos.begin(); item != infos.end(); ++item)
		{
			if (!(*item).folder)
				mListFiles->addItem((*item).name, *item);
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

	void OpenSaveFileDialog::setModalMode(bool _value)
	{
		mModalMode = _value;
		if (mModalMode && isVisible())
		{
			// update modal
			setVisible(true);
		}
	}

	bool OpenSaveFileDialog::getModalMode()
	{
		return mModalMode;
	}

	void OpenSaveFileDialog::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);

		if (_value)
		{
			if (mModalMode)
			{
				MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
				MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
			}
		}
		else
		{
			MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
		}
	}

} // namespace common
