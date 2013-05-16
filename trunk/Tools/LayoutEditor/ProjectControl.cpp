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
#include "EditorWidgets.h"
#include "WidgetSelectorManager.h"
#include "SettingsManager.h"

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

		mList->setNeedToolTip(true);
		mList->eventListSelectAccept += MyGUI::newDelegate(this, &ProjectControl::notifyListSelectAccept);
		mList->eventToolTip += MyGUI::newDelegate(this, &ProjectControl::notifyToolTip);

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->Initialise(SettingsManager::getInstance().getValue("EditorState/OpenSaveFileDialogLayout"));
		mOpenSaveFileDialog->setFileMask("*.xml");
		mOpenSaveFileDialog->eventEndDialog.connect(this, &ProjectControl::notifyEndDialogOpenSaveFile);
		mOpenSaveFileDialog->setCurrentFolder(RecentFilesManager::getInstance().getRecentFolder());
		mOpenSaveFileDialog->setRecentFolders(RecentFilesManager::getInstance().getRecentFolders());

		mTextFieldControl = new TextFieldControl();
		mTextFieldControl->Initialise();
		mTextFieldControl->eventEndDialog.connect(this, &ProjectControl::notifyTextFieldEndDialog);

		CommandManager::getInstance().getEvent("Command_ProjectCreate")->connect(this, &ProjectControl::command_ProjectCreate);
		CommandManager::getInstance().getEvent("Command_ProjectLoad")->connect(this, &ProjectControl::command_ProjectLoad);
		CommandManager::getInstance().getEvent("Command_ProjectClose")->connect(this, &ProjectControl::command_ProjectClose);
		CommandManager::getInstance().getEvent("Command_ProjectDeleteItem")->connect(this, &ProjectControl::command_ProjectDeleteItem);
		CommandManager::getInstance().getEvent("Command_ProjectRenameItem")->connect(this, &ProjectControl::command_ProjectRenameItem);
		CommandManager::getInstance().getEvent("Command_ProjectAddItem")->connect(this, &ProjectControl::command_ProjectAddItem);
		CommandManager::getInstance().getEvent("Command_UpdateResources")->connect(this, &ProjectControl::command_UpdateResources);
		CommandManager::getInstance().getEvent("Command_OpenRecentProject")->connect(this, &ProjectControl::command_OpenRecentProject);

		if (SettingsManager::getInstance().getValue<bool>("Settings/LoadLastProject"))
			loadLastProject();
	}

	ProjectControl::~ProjectControl()
	{
		mList->eventListSelectAccept -= MyGUI::newDelegate(this, &ProjectControl::notifyListSelectAccept);
		mList->eventToolTip -= MyGUI::newDelegate(this, &ProjectControl::notifyToolTip);

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

				updateProjectSkins();
				if (!load())
				{
					/*Message* message = */MessageBoxManager::getInstance().create(
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
					/*Message* message = */MessageBoxManager::getInstance().create(
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
		mOpenSaveFileDialog->setRecentFolders(RecentFilesManager::getInstance().getRecentFolders());
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
		mOpenSaveFileDialog->setRecentFolders(RecentFilesManager::getInstance().getRecentFolders());
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

		if (isProjectItemOpen())
		{
			/*MyGUI::Message* message = */MessageBoxManager::getInstance().create(
				replaceTags("Error"),
				replaceTags("MessageProjectItemOpen"),
				MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok
			);
			return;
		}

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
		mTextFieldControl->setTextField(MyGUI::TextIterator::getOnlyText(mList->getItemNameAt(index)));
		mTextFieldControl->doModal();

		_result = true;
	}

	void ProjectControl::command_ProjectAddItem(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (mProjectName.empty())
		{
			/*MyGUI::Message* message = */MessageBoxManager::getInstance().create(
				replaceTags("Error"),
				replaceTags("MessageProjectNotOpen"),
				MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok
			);
			return;
		}

		saveItemToProject();
		updateProjectSkins();
		load();

		if (mList->getItemCount() != 0)
			mList->setIndexSelected(mList->getItemCount() - 1);

		_result = true;
	}

	void ProjectControl::command_UpdateResources(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (mProjectName.empty())
			return;

		updateProjectSkins();

		MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
		EditorWidgets::getInstance().clear();
		EditorWidgets::getInstance().loadxmlDocument(savedDoc);
		delete savedDoc;
		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

		// перегружаем список
		load();

		_result = true;
	}

	void ProjectControl::updateProjectSkins()
	{
		MyGUI::ResourceManager::getInstance().load(mProjectName);
	}

	void ProjectControl::command_OpenRecentProject(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		MyGUI::UString data = CommandManager::getInstance().getCommandData();
		if (data.empty())
			return;

		clear();

		MyGUI::UString filePath = "";
		MyGUI::UString fileName = data;

		size_t index = data.find_last_of("\\/");
		if (index != MyGUI::UString::npos)
		{
			filePath = data.substr(0, index);
			fileName = data.substr(index + 1);
		}

		RecentFilesManager::getInstance().setRecentFolder(filePath);
		setFileName(filePath, fileName);

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
		MyGUI::VectorString items;

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
					items.push_back(element->findAttribute("name"));
			}
		}
		else
		{
			return false;
		}

		const std::string& colour_error = MyGUI::LanguageManager::getInstance().getTag("ColourError");

		for (MyGUI::VectorString::const_iterator item = items.begin(); item != items.end(); ++item)
		{
			bool successItem = checkItem(*item, items);
			mList->addItem(successItem ? (*item) : (colour_error + (*item)));
		}

		RecentFilesManager::getInstance().addRecentProject(fileName);

		return true;
	}

	bool ProjectControl::checkItem(const std::string& _name, const MyGUI::VectorString& _items)
	{
		size_t count = 0;
		for (MyGUI::VectorString::const_iterator item = _items.begin(); item != _items.end(); ++item)
		{
			if ((*item) == _name)
				count++;
		}

		if (count > 1)
			return false;

		return checkTemplate(_name);
	}

	bool ProjectControl::checkTemplate(const std::string& _skinName)
	{
		MyGUI::ResourceLayout* templateInfo = MyGUI::LayoutManager::getInstance().getByName(_skinName, false);
		if (templateInfo != nullptr)
		{
			const MyGUI::VectorWidgetInfo& data = templateInfo->getLayoutData();
			for (MyGUI::VectorWidgetInfo::const_iterator container = data.begin(); container != data.end(); ++container)
			{
				if (container->name == "Root")
					return true;
			}
		}

		return false;
	}

	void ProjectControl::setFileName(const MyGUI::UString& _filePath, const MyGUI::UString& _fileName)
	{
		mProjectName = _fileName;
		mProjectPath = _filePath;

		SettingsManager::getInstance().setValue("Files/LastProjectName", mProjectName);
		SettingsManager::getInstance().setValue("Files/LastProjectPath", mProjectPath);

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
			updateProjectSkins();
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
			updateProjectSkins();
			load();

			if (index < mList->getItemCount())
				mList->setIndexSelected(index);
			else if (mList->getItemCount() != 0)
				mList->setIndexSelected(mList->getItemCount() - 1);

			CommandManager::getInstance().executeCommand("Command_UpdateItemName");
		}
	}

	bool ProjectControl::isExistFile(const MyGUI::UString& _filePath, const MyGUI::UString& _fileName)
	{
		common::VectorFileInfo fileInfo;
		common::getSystemFileList(fileInfo, _filePath, _fileName);

		return !fileInfo.empty();
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

		MyGUI::UString fileName = common::concatenatePath(mProjectPath, mProjectName);
		doc.save(fileName);

		RecentFilesManager::getInstance().addRecentProject(fileName);

		updateCaption();
	}

	void ProjectControl::notifyListSelectAccept(MyGUI::ListBox* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE)
			return;

		MyGUI::UString data = MyGUI::utility::toString(MyGUI::UString(common::concatenatePath(mProjectPath, mProjectName)), "|", _index);
		CommandManager::getInstance().setCommandData(data);
		CommandManager::getInstance().executeCommand("Command_FileDrop");
	}

	bool ProjectControl::isProjectItemOpen()
	{
		return EditorWidgets::getInstance().getCurrentFileName() == common::concatenatePath(mProjectPath, mProjectName);
	}

	void ProjectControl::saveItemToProject()
	{
		MyGUI::UString name = EditorWidgets::getInstance().getCurrentFileName();

		size_t index = name.find_last_of("\\/");
		name = (index == MyGUI::UString::npos) ? name : name.substr(index + 1);

		MyGUI::UString endName = ".layout";
		index = name.find(endName);
		if (index != MyGUI::UString::npos && (index + endName.size()) == name.size())
		{
			name = name.substr(0, index);
		}
		else
		{
			name = EditorWidgets::getInstance().getCurrentItemName();
			if (name.empty())
				name = "unnamed";
		}

		size_t indexItem = MyGUI::ITEM_NONE;
		addItemToProject(name, indexItem);

		if (indexItem != MyGUI::ITEM_NONE)
		{
			MyGUI::UString fileName = MyGUI::utility::toString(MyGUI::UString(common::concatenatePath(mProjectPath, mProjectName)), "|", indexItem);
			CommandManager::getInstance().setCommandData(fileName);
			CommandManager::getInstance().executeCommand("Command_SaveItemAs");
		}
	}

	bool ProjectControl::addItemToProject(const MyGUI::UString& _name, size_t& _index)
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
			MyGUI::xml::ElementPtr node = root->createChild("Resource");
			node->addAttribute("type", "ResourceLayout");
			node->addAttribute("name", _name);

			_index = 0;

			MyGUI::xml::ElementEnumerator element = root->getElementEnumerator();
			while (element.next("Resource"))
			{
				if (element->findAttribute("type") == "ResourceLayout")
				{
					_index++;
				}
			}

			if (_index == 0)
				_index = MyGUI::ITEM_NONE;
			else
				_index --;
		}

		return doc.save(fileName);
	}

	void ProjectControl::loadLastProject()
	{
		MyGUI::UString projectName = SettingsManager::getInstance().getValue("Files/LastProjectName");
		MyGUI::UString projectPath = SettingsManager::getInstance().getValue("Files/LastProjectPath");

		if (projectName.empty())
			return;

		setFileName(projectPath, projectName);

		updateProjectSkins();
		if (!load())
		{
			/*Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Error"),
				replaceTags("MessageFailedLoadProject"),
				MessageBoxStyle::IconError | MessageBoxStyle::Ok
			);*/

			clear();
		}

		updateCaption();
	}

	void ProjectControl::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
	{
		if (_info.type == MyGUI::ToolTipInfo::Show)
		{
			if (_info.index != MyGUI::ITEM_NONE)
			{
				SkinInfo data = getCellData(_sender, _info.index);
				EditorToolTip::getInstancePtr()->show(data);
				EditorToolTip::getInstancePtr()->move(_info.point);
			}
		}
		else if (_info.type == MyGUI::ToolTipInfo::Hide)
		{
			EditorToolTip::getInstancePtr()->hide();
		}
		else if (_info.type == MyGUI::ToolTipInfo::Move)
		{
			EditorToolTip::getInstancePtr()->move(_info.point);
		}
	}

	SkinInfo ProjectControl::getCellData(MyGUI::Widget* _sender, size_t _index)
	{
		MyGUI::ListBox* box = _sender->castType<MyGUI::ListBox>();
		MyGUI::UString name = box->getItemNameAt(_index);
		return SkinInfo(MyGUI::TextIterator::getOnlyText(name), "", "");
	}

}
