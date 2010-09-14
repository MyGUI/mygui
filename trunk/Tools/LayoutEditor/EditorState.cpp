#include "Precompiled.h"
#include "Common.h"
#include "EditorState.h"
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
		mSelectionAreaControl(nullptr),
		mFileName("unnamed.xml"),
		mDefaultFileName("unnamed.xml")
	{
		CommandManager::getInstance().registerCommand("Command_FileLoad", MyGUI::newDelegate(this, &EditorState::commandLoad));
		CommandManager::getInstance().registerCommand("Command_FileSave", MyGUI::newDelegate(this, &EditorState::commandSave));
		CommandManager::getInstance().registerCommand("Command_FileSaveAs", MyGUI::newDelegate(this, &EditorState::commandSaveAs));
		CommandManager::getInstance().registerCommand("Command_ClearAll", MyGUI::newDelegate(this, &EditorState::commandClear));
		CommandManager::getInstance().registerCommand("Command_Test", MyGUI::newDelegate(this, &EditorState::commandTest));
		CommandManager::getInstance().registerCommand("Command_Quit", MyGUI::newDelegate(this, &EditorState::commandQuit));
		CommandManager::getInstance().registerCommand("Command_Settings", MyGUI::newDelegate(this, &EditorState::commandSettings));
		CommandManager::getInstance().registerCommand("Command_CodeGenerator", MyGUI::newDelegate(this, &EditorState::commandCodeGenerator));
		CommandManager::getInstance().registerCommand("Command_RecentFiles", MyGUI::newDelegate(this, &EditorState::commandRecentFiles));
		CommandManager::getInstance().registerCommand("Command_FileDrop", MyGUI::newDelegate(this, &EditorState::commandFileDrop));
	}

	EditorState::~EditorState()
	{
	}

	void EditorState::initState()
	{
		addUserTag("\\n", "\n");
		addUserTag("CurrentFileName", mFileName);

		mBackgroundControl = new BackgroundControl();
		mMainPaneControl = new MainPaneControl();

		// settings window
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

		mSelectionAreaControl = new SelectionAreaControl();

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

		delete mSelectionAreaControl;
		mSelectionAreaControl = nullptr;

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

	void EditorState::commandTest(const MyGUI::UString& _commandName)
	{
		StateManager::getInstance().stateEvent(this, "Test");
	}

	void EditorState::commandSettings(const MyGUI::UString& _commandName)
	{
		mSettingsWindow->doModal();
	}

	void EditorState::commandCodeGenerator(const MyGUI::UString& _commandName)
	{
		mCodeGenerator->loadTemplate();
		mCodeGenerator->doModal();
	}

	void EditorState::commandRecentFiles(const MyGUI::UString& _commandName)
	{
		commandFileDrop(_commandName);
	}

	void EditorState::commandLoad(const MyGUI::UString& _commandName)
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
	}

	void EditorState::commandSave(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		if (UndoManager::getInstance().isUnsaved())
		{
			save();
		}
	}

	void EditorState::commandSaveAs(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		showSaveAsWindow();
	}

	void EditorState::commandClear(const MyGUI::UString& _commandName)
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
	}

	void EditorState::commandQuit(const MyGUI::UString& _commandName)
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
	}

	void EditorState::commandFileDrop(const MyGUI::UString& _commandName)
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
			if (mFileName != mDefaultFileName)
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
			if (mFileName != mDefaultFileName)
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
		Application::getInstance().setCaption(replaceTags("CaptionMainWindow"));
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
		addUserTag("CurrentFileName", mFileName);
		size_t pos = mFileName.find_last_of("\\/");
		MyGUI::UString shortName = pos == MyGUI::UString::npos ? mFileName : mFileName.substr(mFileName.find_last_of("\\/") + 1);
		addUserTag("CurrentFileName_Short", shortName);
	}

} // namespace tools
