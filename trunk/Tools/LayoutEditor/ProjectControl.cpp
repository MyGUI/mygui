/*!
	@file
	@author		Albert Semenov
	@date		10/2010
*/

#include "Precompiled.h"
#include "ProjectControl.h"
#include "RecentFilesManager.h"
#include "CommandManager.h"
#include "MessageBoxManager.h"
#include "DialogManager.h"
#include "FileSystemInfo/FileSystemInfo.h"
#include "Localise.h"

namespace tools
{
	const std::string LogSection = "LayoutEditor";

	ProjectControl::ProjectControl(MyGUI::Widget* _parent) :
		BaseLayout("ProjectControl.layout", _parent),
		mOpenSaveFileDialog(nullptr),
		mTextFieldControl(nullptr),
		mList(nullptr)
	{
		assignWidget(mList, "List");
		assignWidget(mProjectNameText, "ProjectName");

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->addFileMask("*.xml");
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &ProjectControl::notifyEndDialogOpenSaveFile);
		mOpenSaveFileDialog->setCurrentFolder(RecentFilesManager::getInstance().getRecentFolder());
		mOpenSaveFileDialog->setRecentFilders(RecentFilesManager::getInstance().getRecentFolders());

		mTextFieldControl = new TextFieldControl();
		mTextFieldControl->eventEndDialog = MyGUI::newDelegate(this, &ProjectControl::notifyTextFieldEndDialog);

		CommandManager::getInstance().registerCommand("Command_ProjectCreate", MyGUI::newDelegate(this, &ProjectControl::command_ProjectCreate));
		CommandManager::getInstance().registerCommand("Command_ProjectLoad", MyGUI::newDelegate(this, &ProjectControl::command_ProjectLoad));
		CommandManager::getInstance().registerCommand("Command_ProjectClose", MyGUI::newDelegate(this, &ProjectControl::command_ProjectClose));
		CommandManager::getInstance().registerCommand("Command_ProjectDeleteItem", MyGUI::newDelegate(this, &ProjectControl::command_ProjectDeleteItem));
		CommandManager::getInstance().registerCommand("Command_ProjectRenameItem", MyGUI::newDelegate(this, &ProjectControl::command_ProjectRenameItem));
	}

	ProjectControl::~ProjectControl()
	{
		delete mTextFieldControl;
		mTextFieldControl = nullptr;

		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;
	}

	void ProjectControl::notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result)
	{
		if (_result)
		{
			if (mOpenSaveFileDialog->getMode() == "Load")
			{
				clear();

				RecentFilesManager::getInstance().setRecentFolder(mOpenSaveFileDialog->getCurrentFolder());
				setFileName(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());

				if (!load())
				{
					/*MyGUI::Message* message = */MessageBoxManager::getInstance().create(
						replaceTags("Error"),
						replaceTags("MessageFailedLoadProject"),
						MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok
					);

					clear();
				}

				updateCaption();
			}
			else if (mOpenSaveFileDialog->getMode() == "Create")
			{
				if (isExistFile(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName()))
				{
					/*MyGUI::Message* message = */MessageBoxManager::getInstance().create(
						replaceTags("Error"),
						replaceTags("MessageFileExist"),
						MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok
					);
				}
				else
				{
					createProject(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				}
			}
		}

		mOpenSaveFileDialog->endModal();
	}

	void ProjectControl::command_ProjectCreate(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mOpenSaveFileDialog->setCurrentFolder(RecentFilesManager::getInstance().getRecentFolder());
		mOpenSaveFileDialog->setRecentFilders(RecentFilesManager::getInstance().getRecentFolders());
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionCreateFile"), replaceTags("ButtonCreateFile"));
		mOpenSaveFileDialog->setMode("Create");
		mOpenSaveFileDialog->doModal();

		_result = true;
	}

	void ProjectControl::command_ProjectLoad(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mOpenSaveFileDialog->setCurrentFolder(RecentFilesManager::getInstance().getRecentFolder());
		mOpenSaveFileDialog->setRecentFilders(RecentFilesManager::getInstance().getRecentFolders());
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionOpenFile"), replaceTags("ButtonOpenFile"));
		mOpenSaveFileDialog->setMode("Load");
		mOpenSaveFileDialog->doModal();

		_result = true;
	}

	void ProjectControl::command_ProjectClose(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		clear();
		updateCaption();

		_result = true;
	}

	void ProjectControl::command_ProjectDeleteItem(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		size_t index = mList->getIndexSelected();
		if (index == MyGUI::ITEM_NONE)
			return;

		MyGUI::Message* message = MessageBoxManager::getInstance().create(
			replaceTags("Warning"),
			replaceTags("MessageDeleteLayout"),
			MyGUI::MessageBoxStyle::IconQuest
				| MyGUI::MessageBoxStyle::Yes
				| MyGUI::MessageBoxStyle::No);
		message->eventMessageBoxResult += MyGUI::newDelegate(this, &ProjectControl::notifyMessageBoxResultDelete);

		_result = true;
	}

	void ProjectControl::command_ProjectRenameItem(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		size_t index = mList->getIndexSelected();
		if (index == MyGUI::ITEM_NONE)
			return;

		mTextFieldControl->setCaption(replaceTags("CaptionRenameLayout"));
		mTextFieldControl->setTextField(mList->getItemNameAt(index));
		mTextFieldControl->doModal();

		_result = true;
	}

	bool ProjectControl::checkCommand()
	{
		if (DialogManager::getInstance().getAnyDialog())
			return false;

		if (MessageBoxManager::getInstance().hasAny())
			return false;

		return true;
	}

	void ProjectControl::clear()
	{
		mProjectName.clear();
		mList->removeAllItems();
	}

	bool ProjectControl::load()
	{
		mList->removeAllItems();

		MyGUI::UString fileName = common::concatenatePath(mProjectPath, mProjectName);

		MyGUI::xml::Document doc;
		if (!doc.open(fileName))
		{
			MYGUI_LOGGING(LogSection, Error, doc.getLastError());
			return false;
		}

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if ((nullptr == root) || (root->getName() != "MyGUI"))
		{
			MYGUI_LOGGING(LogSection, Error, "'" << fileName << "', tag 'MyGUI' not found");
			return false;
		}

		if (root->findAttribute("type") == "Resource")
		{
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator element = root->getElementEnumerator();
			while (element.next("Resource"))
			{
				if (element->findAttribute("type") == "ResourceLayout")
					mList->addItem(element->findAttribute("name"));
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	void ProjectControl::setFileName(const MyGUI::UString& _filePath, const MyGUI::UString& _fileName)
	{
		mProjectName = _fileName;
		mProjectPath = _filePath;

		addUserTag("CurrentProjectName", mProjectName);
	}

	void ProjectControl::updateCaption()
	{
		mProjectNameText->setCaption(mProjectName);
	}

	bool ProjectControl::deleteItemFromProject(size_t _index)
	{
		bool deleted = false;

		MyGUI::UString fileName = common::concatenatePath(mProjectPath, mProjectName);

		MyGUI::xml::Document doc;
		if (!doc.open(fileName))
		{
			MYGUI_LOGGING(LogSection, Error, doc.getLastError());
			return false;
		}

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if ((nullptr == root) || (root->getName() != "MyGUI"))
		{
			MYGUI_LOGGING(LogSection, Error, "'" << fileName << "', tag 'MyGUI' not found");
			return false;
		}

		if (root->findAttribute("type") == "Resource")
		{
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator element = root->getElementEnumerator();
			while (element.next("Resource"))
			{
				if (element->findAttribute("type") == "ResourceLayout")
				{
					if (_index == 0)
					{
						root->removeChild(element.current());
						deleted = true;
						break;
					}
					else
					{
						_index --;
					}
				}
			}
		}

		doc.save(fileName);

		return deleted;
	}

	bool ProjectControl::renameItemInProject(size_t _index, const MyGUI::UString& _name)
	{
		bool renamed = false;

		MyGUI::UString fileName = common::concatenatePath(mProjectPath, mProjectName);

		MyGUI::xml::Document doc;
		if (!doc.open(fileName))
		{
			MYGUI_LOGGING(LogSection, Error, doc.getLastError());
			return false;
		}

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if ((nullptr == root) || (root->getName() != "MyGUI"))
		{
			MYGUI_LOGGING(LogSection, Error, "'" << fileName << "', tag 'MyGUI' not found");
			return false;
		}

		if (root->findAttribute("type") == "Resource")
		{
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator element = root->getElementEnumerator();
			while (element.next("Resource"))
			{
				if (element->findAttribute("type") == "ResourceLayout")
				{
					if (_index == 0)
					{
						element->setAttribute("name", _name);
						renamed = true;
						break;
					}
					else
					{
						_index --;
					}
				}
			}
		}

		doc.save(fileName);

		return renamed;
	}

	void ProjectControl::notifyMessageBoxResultDelete(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			size_t index = mList->getIndexSelected();
			if (index == MyGUI::ITEM_NONE)
				return;

			deleteItemFromProject(index);
			load();

			if (index < mList->getItemCount())
				mList->setIndexSelected(index);
			else if (mList->getItemCount() != 0)
				mList->setIndexSelected(mList->getItemCount() - 1);
		}
	}

	void ProjectControl::notifyTextFieldEndDialog(Dialog* _sender, bool _result)
	{
		mTextFieldControl->endModal();

		if (_result)
		{
			size_t index = mList->getIndexSelected();
			if (index == MyGUI::ITEM_NONE)
				return;

			if (mTextFieldControl->getTextField() == "")
				return;

			renameItemInProject(index, mTextFieldControl->getTextField());
			load();

			if (index < mList->getItemCount())
				mList->setIndexSelected(index);
			else if (mList->getItemCount() != 0)
				mList->setIndexSelected(mList->getItemCount() - 1);
		}
	}

	bool ProjectControl::isExistFile(const MyGUI::UString& _filePath, const MyGUI::UString& _fileName)
	{
		common::VectorFileInfo fileInfo;
		common::getSystemFileList(fileInfo, _filePath, _fileName);

		return fileInfo.size() != 0;
	}

	void ProjectControl::createProject(const MyGUI::UString& _filePath, const MyGUI::UString& _fileName)
	{
		clear();

		RecentFilesManager::getInstance().setRecentFolder(_filePath);
		setFileName(_filePath, _fileName);

		MyGUI::xml::Document doc;
		doc.createDeclaration();
		MyGUI::xml::Element* root = doc.createRoot("MyGUI");
		root->addAttribute("type", "Resource");

		doc.save(common::concatenatePath(mProjectPath, mProjectName));

		updateCaption();
	}

} // namespace tools
