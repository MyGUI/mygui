#include "Precompiled.h"
#include "EditorState.h"
#include "Common.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"
#include "GroupMessage.h"
#include "FileSystemInfo/FileSystemInfo.h"
#include "CommandManager.h"
#include "SettingsManager.h"
#include "WidgetSelectorManager.h"
#include "HotKeyManager.h"
#include "MessageBoxManager.h"
#include "DialogManager.h"
#include "StateManager.h"
#include "Localise.h"
#include "Application.h"
#include "RecentFilesManager.h"
#include "WidgetCreatorManager.h"

namespace tools
{
	EditorState::EditorState() :
		mSettingsWindow(nullptr),
		mCodeGenerator(nullptr),
		mOpenSaveFileDialog(nullptr),
		mMessageBoxFadeControl(nullptr),
		mBackgroundControl(nullptr),
		mMainPaneControl(nullptr),
		mFileName("unnamed.layout"),
		mDefaultFileName("unnamed.layout")
	{
		CommandManager::getInstance().registerCommand("Command_FileLoad", MyGUI::newDelegate(this, &EditorState::command_Load));
		CommandManager::getInstance().registerCommand("Command_FileSave", MyGUI::newDelegate(this, &EditorState::command_Save));
		CommandManager::getInstance().registerCommand("Command_FileSaveAs", MyGUI::newDelegate(this, &EditorState::command_SaveAs));
		CommandManager::getInstance().registerCommand("Command_ClearAll", MyGUI::newDelegate(this, &EditorState::command_Clear));
		CommandManager::getInstance().registerCommand("Command_Test", MyGUI::newDelegate(this, &EditorState::command_Test));
		CommandManager::getInstance().registerCommand("Command_Quit", MyGUI::newDelegate(this, &EditorState::command_Quit));
		CommandManager::getInstance().registerCommand("Command_Settings", MyGUI::newDelegate(this, &EditorState::command_Settings));
		CommandManager::getInstance().registerCommand("Command_CodeGenerator", MyGUI::newDelegate(this, &EditorState::command_CodeGenerator));
		CommandManager::getInstance().registerCommand("Command_OpenRecentFile", MyGUI::newDelegate(this, &EditorState::command_OpenRecentFile));
		CommandManager::getInstance().registerCommand("Command_FileDrop", MyGUI::newDelegate(this, &EditorState::command_FileDrop));
		CommandManager::getInstance().registerCommand("Command_SaveItemAs", MyGUI::newDelegate(this, &EditorState::command_SaveItemAs));
		CommandManager::getInstance().registerCommand("Command_UpdateItemName", MyGUI::newDelegate(this, &EditorState::command_UpdateItemName));
		CommandManager::getInstance().registerCommand("Command_UpdateResources", MyGUI::newDelegate(this, &EditorState::command_UpdateResources));
	}

	EditorState::~EditorState()
	{
	}

	void EditorState::initState()
	{
		addUserTag("\\n", "\n");
		setFileName(mFileName);

		mBackgroundControl = new BackgroundControl();
		mMainPaneControl = new MainPaneControl();

		mSettingsWindow = new SettingsWindow();
		mSettingsWindow->eventEndDialog = MyGUI::newDelegate(this, &EditorState::notifySettingsWindowEndDialog);

		mCodeGenerator = new CodeGenerator();
		mCodeGenerator->eventEndDialog = MyGUI::newDelegate(this, &EditorState::notifyEndDialogCodeGenerator);

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->setFileMask("*.layout");
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &EditorState::notifyEndDialogOpenSaveFile);
		mOpenSaveFileDialog->setCurrentFolder(RecentFilesManager::getInstance().getRecentFolder());
		mOpenSaveFileDialog->setRecentFilders(RecentFilesManager::getInstance().getRecentFolders());

		mMessageBoxFadeControl = new MessageBoxFadeControl();

		updateCaption();

		if (!Application::getInstance().getParams().empty())
		{
			setFileName(Application::getInstance().getParams().front());

			load();
			updateCaption();
		}

		UndoManager::getInstance().eventChanges += MyGUI::newDelegate(this, &EditorState::notifyChanges);
	}

	void EditorState::cleanupState()
	{
		UndoManager::getInstance().eventChanges -= MyGUI::newDelegate(this, &EditorState::notifyChanges);

		delete mMessageBoxFadeControl;
		mMessageBoxFadeControl = nullptr;

		delete mSettingsWindow;
		mSettingsWindow = nullptr;

		delete mCodeGenerator;
		mCodeGenerator = nullptr;

		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;

		delete mBackgroundControl;
		mBackgroundControl = nullptr;

		delete mMainPaneControl;
		mMainPaneControl = nullptr;
	}

	void EditorState::notifySettingsWindowEndDialog(Dialog* _dialog, bool _result)
	{
		MYGUI_ASSERT(mSettingsWindow == _dialog, "mSettingsWindow == _sender");

		if (_result)
		{
			mSettingsWindow->saveSettings();
		}

		mSettingsWindow->endModal();
	}

	void EditorState::command_Test(const MyGUI::UString& _commandName, bool& _result)
	{
		StateManager::getInstance().stateEvent(this, "Test");

		_result = true;
	}

	void EditorState::command_Settings(const MyGUI::UString& _commandName, bool& _result)
	{
		mSettingsWindow->doModal();

		_result = true;
	}

	void EditorState::command_CodeGenerator(const MyGUI::UString& _commandName, bool& _result)
	{
		mCodeGenerator->loadTemplate();
		mCodeGenerator->doModal();

		_result = true;
	}

	void EditorState::command_OpenRecentFile(const MyGUI::UString& _commandName, bool& _result)
	{
		command_FileDrop(_commandName, _result);
	}

	void EditorState::command_Load(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (UndoManager::getInstance().isUnsaved())
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageUnsavedData"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &EditorState::notifyMessageBoxResultLoad);
		}
		else
		{
			showLoadWindow();
		}

		_result = true;
	}

	void EditorState::command_Save(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (UndoManager::getInstance().isUnsaved())
		{
			save();
		}

		_result = true;
	}

	void EditorState::command_SaveAs(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		showSaveAsWindow();

		_result = true;
	}

	void EditorState::command_Clear(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (UndoManager::getInstance().isUnsaved())
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageUnsavedData"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &EditorState::notifyMessageBoxResultClear);
		}
		else
		{
			clear();
		}

		_result = true;
	}

	void EditorState::command_Quit(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (UndoManager::getInstance().isUnsaved())
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageUnsavedData"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &EditorState::notifyMessageBoxResultQuit);
		}
		else
		{
			StateManager::getInstance().stateEvent(this, "Exit");
		}

		_result = true;
	}

	void EditorState::command_FileDrop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mDropFileName = CommandManager::getInstance().getCommandData();
		if (mDropFileName.empty())
			return;

		if (UndoManager::getInstance().isUnsaved())
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageUnsavedData"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &EditorState::notifyMessageBoxResultLoadDropFile);
		}
		else
		{
			clear();

			loadDropFile();
		}

		_result = true;
	}

	void EditorState::command_SaveItemAs(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		setFileName(CommandManager::getInstance().getCommandData());

		save();
		updateCaption();

		_result = true;
	}

	void EditorState::command_UpdateItemName(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		setFileName(mFileName);
		updateCaption();

		_result = true;
	}

	void EditorState::clear()
	{
		WidgetCreatorManager::getInstance().resetCreatorInfo();
		EditorWidgets::getInstance().clear();

		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

		UndoManager::getInstance().shutdown();
		UndoManager::getInstance().initialise(EditorWidgets::getInstancePtr());

		setFileName(mDefaultFileName);

		updateCaption();
	}

	void EditorState::load()
	{
		if (EditorWidgets::getInstance().load(mFileName))
		{
			if (mFileName != mDefaultFileName && !isProjectFile(mFileName))
				RecentFilesManager::getInstance().addRecentFile(mFileName);

			UndoManager::getInstance().addValue();
			UndoManager::getInstance().setUnsaved(false);
		}
		else
		{
			/*MyGUI::Message* message = */MessageBoxManager::getInstance().create(
				replaceTags("Error"),
				replaceTags("MessageFailedLoadFile"),
				MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok
			);

			setFileName(mDefaultFileName);

			updateCaption();
		}
	}

	bool EditorState::save()
	{
		if (EditorWidgets::getInstance().save(mFileName))
		{
			if (mFileName != mDefaultFileName && !isProjectFile(mFileName))
				RecentFilesManager::getInstance().addRecentFile(mFileName);

			UndoManager::getInstance().addValue();
			UndoManager::getInstance().setUnsaved(false);
			return true;
		}
		else
		{
			/*MyGUI::Message* message = */MessageBoxManager::getInstance().create(
				replaceTags("Error"),
				replaceTags("MessageFailedSaveFile"),
				MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok
			);
		}
		return false;
	}

	void EditorState::updateCaption()
	{
		addUserTag("HasChanged", UndoManager::getInstance().isUnsaved() ? "*" : "");

		CommandManager::getInstance().executeCommand("Command_UpdateAppCaption");
	}

	void EditorState::notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			if (save())
			{
				clear();

				showLoadWindow();
			}
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			clear();

			showLoadWindow();
		}
	}

	void EditorState::notifyMessageBoxResultLoadDropFile(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			if (save())
			{
				clear();

				loadDropFile();
			}
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			clear();

			loadDropFile();
		}
	}

	void EditorState::loadDropFile()
	{
		setFileName(mDropFileName);

		load();
		updateCaption();
	}

	void EditorState::showLoadWindow()
	{
		mOpenSaveFileDialog->setCurrentFolder(RecentFilesManager::getInstance().getRecentFolder());
		mOpenSaveFileDialog->setRecentFilders(RecentFilesManager::getInstance().getRecentFolders());
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionOpenFile"), replaceTags("ButtonOpenFile"));
		mOpenSaveFileDialog->setMode("Load");
		mOpenSaveFileDialog->doModal();
	}

	void EditorState::notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result)
	{
		if (_result)
		{
			if (mOpenSaveFileDialog->getMode() == "SaveAs")
			{
				RecentFilesManager::getInstance().setRecentFolder(mOpenSaveFileDialog->getCurrentFolder());
				setFileName(common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName()));

				save();
				updateCaption();
			}
			else if (mOpenSaveFileDialog->getMode() == "Load")
			{
				clear();

				RecentFilesManager::getInstance().setRecentFolder(mOpenSaveFileDialog->getCurrentFolder());
				setFileName(common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName()));

				load();
				updateCaption();
			}
		}

		mOpenSaveFileDialog->endModal();
	}

	void EditorState::notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			if (save())
			{
				clear();
			}
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			clear();
		}
	}

	void EditorState::showSaveAsWindow()
	{
		mOpenSaveFileDialog->setCurrentFolder(RecentFilesManager::getInstance().getRecentFolder());
		mOpenSaveFileDialog->setRecentFilders(RecentFilesManager::getInstance().getRecentFolders());
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionSaveFile"), replaceTags("ButtonSaveFile"));
		mOpenSaveFileDialog->setMode("SaveAs");
		mOpenSaveFileDialog->doModal();
	}

	void EditorState::notifyMessageBoxResultQuit(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			if (save())
			{
				StateManager::getInstance().stateEvent(this, "Exit");
			}
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			StateManager::getInstance().stateEvent(this, "Exit");
		}
	}

	bool EditorState::checkCommand()
	{
		if (DialogManager::getInstance().getAnyDialog())
			return false;

		if (MessageBoxManager::getInstance().hasAny())
			return false;

		if (!StateManager::getInstance().getStateActivate(this))
			return false;

		return true;
	}

	void EditorState::notifyChanges(bool _changes)
	{
		updateCaption();
	}

	void EditorState::notifyEndDialogCodeGenerator(Dialog* _dialog, bool _result)
	{
		mCodeGenerator->endModal();
		if (_result)
			mCodeGenerator->saveTemplate();
	}

	void EditorState::pauseState()
	{
		mMainPaneControl->setVisible(false);
	}

	void EditorState::resumeState()
	{
		mMainPaneControl->setVisible(true);
	}

	void EditorState::setFileName(const MyGUI::UString& _fileName)
	{
		mFileName = _fileName;
		addUserTag("CurrentFileName", convertProjectName(mFileName));

		size_t pos = mFileName.find_last_of("\\/");
		MyGUI::UString shortName = pos == MyGUI::UString::npos ? mFileName : mFileName.substr(pos + 1);
		addUserTag("CurrentFileName_Short", shortName);
	}

	MyGUI::UString EditorState::convertProjectName(const MyGUI::UString& _fileName)
	{
		size_t index = _fileName.find("|");
		if (index == MyGUI::UString::npos)
			return _fileName;

		MyGUI::UString fileName = _fileName.substr(0, index);
		MyGUI::UString itemIndexName = _fileName.substr(index + 1);
		size_t itemIndex = MyGUI::utility::parseValue<size_t>(itemIndexName);

		MyGUI::xml::Document doc;
		if (!doc.open(fileName))
			return _fileName;

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if ((nullptr == root) || (root->getName() != "MyGUI"))
			return _fileName;

		if (root->findAttribute("type") == "Resource")
		{
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator element = root->getElementEnumerator();
			while (element.next("Resource"))
			{
				if (element->findAttribute("type") == "ResourceLayout")
				{
					if (itemIndex == 0)
					{
						// поменять на теги
						return MyGUI::utility::toString(fileName, " [", element->findAttribute("name"), "]");
					}
					else
					{
						itemIndex --;
					}
				}
			}
		}

		return _fileName;
	}

	bool EditorState::isProjectFile(const MyGUI::UString& _fileName)
	{
		size_t index = _fileName.find("|");
		return (index != MyGUI::UString::npos);
	}

	void EditorState::command_UpdateResources(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		typedef std::vector<MyGUI::UString> VectorUString;
		VectorUString resources = SettingsManager::getInstance().getSector("Settings")->getPropertyValueList("UpdateResources");
		if (resources.empty())
			return;

		for (VectorUString::iterator resource = resources.begin(); resource != resources.end(); ++resource)
			MyGUI::ResourceManager::getInstance().load(*resource);

		MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
		EditorWidgets::getInstance().clear();
		EditorWidgets::getInstance().loadxmlDocument(savedDoc);
		delete savedDoc;
		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

		_result = true;
	}

} // namespace tools
