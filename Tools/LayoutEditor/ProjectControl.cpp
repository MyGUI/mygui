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
		mList(nullptr)
	{
		assignWidget(mList, "List");
		assignWidget(mProjectNameText, "ProjectName");

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->addFileMask("*.xml");
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &ProjectControl::notifyEndDialogOpenSaveFile);
		mOpenSaveFileDialog->setCurrentFolder(RecentFilesManager::getInstance().getRecentFolder());
		mOpenSaveFileDialog->setRecentFilders(RecentFilesManager::getInstance().getRecentFolders());

		CommandManager::getInstance().registerCommand("Command_ProjectLoad", MyGUI::newDelegate(this, &ProjectControl::command_ProjectLoad));
		CommandManager::getInstance().registerCommand("Command_ProjectClose", MyGUI::newDelegate(this, &ProjectControl::command_ProjectClose));
	}

	ProjectControl::~ProjectControl()
	{
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
		}

		mOpenSaveFileDialog->endModal();
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

} // namespace tools
