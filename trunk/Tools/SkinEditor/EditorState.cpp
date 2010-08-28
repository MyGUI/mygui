/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "EditorState.h"
#include "Application.h"
#include "FileSystemInfo/FileSystemInfo.h"
#include "Localise.h"
#include "CommandManager.h"
#include "ActionManager.h"
#include "MessageBoxManager.h"
#include "DialogManager.h"
#include "StateManager.h"

namespace tools
{

	EditorState::EditorState() :
		mMainPane(nullptr),
		mFileName("unnamed.xml"),
		mDefaultFileName("unnamed.xml"),
		mOpenSaveFileDialog(nullptr),
		mTestWindow(nullptr),
		mMessageBoxFadeControl(nullptr)
	{
	}

	EditorState::~EditorState()
	{
	}

	void EditorState::initState()
	{
		addUserTag("\\n", "\n");
		addUserTag("SE_CurrentFileName", mFileName);

		mMainPane = new MainPane();
		mMessageBoxFadeControl = new MessageBoxFadeControl();

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &EditorState::notifyEndDialog);
		mOpenSaveFileDialog->setFileMask("*.xml");

		mTestWindow = new TestWindow();
		mTestWindow->eventEndDialog = MyGUI::newDelegate(this, &EditorState::notifyEndDialogTest);

		CommandManager::getInstance().registerCommand("Command_FileLoad", MyGUI::newDelegate(this, &EditorState::commandLoad));
		CommandManager::getInstance().registerCommand("Command_FileSave", MyGUI::newDelegate(this, &EditorState::commandSave));
		CommandManager::getInstance().registerCommand("Command_FileSaveAs", MyGUI::newDelegate(this, &EditorState::commandSaveAs));
		CommandManager::getInstance().registerCommand("Command_ClearAll", MyGUI::newDelegate(this, &EditorState::commandClear));
		CommandManager::getInstance().registerCommand("Command_Test", MyGUI::newDelegate(this, &EditorState::commandTest));
		CommandManager::getInstance().registerCommand("Command_QuitApp", MyGUI::newDelegate(this, &EditorState::commandQuit));
		CommandManager::getInstance().registerCommand("Command_FileDrop", MyGUI::newDelegate(this, &EditorState::commandFileDrop));

		ActionManager::getInstance().eventChanges += MyGUI::newDelegate(this, &EditorState::notifyChanges);

		updateCaption();

		for (Application::VectorWString::const_iterator file = Application::getInstance().getParams().begin(); file != Application::getInstance().getParams().end(); ++file)
		{
			mFileName = *file;
			addUserTag("SE_CurrentFileName", mFileName);

			load();
			updateCaption();
			break;
		}
	}

	void EditorState::cleanupState()
	{
		ActionManager::getInstance().eventChanges -= MyGUI::newDelegate(this, &EditorState::notifyChanges);

		delete mTestWindow;
		mTestWindow = nullptr;

		mOpenSaveFileDialog->eventEndDialog = nullptr;
		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;

		delete mMessageBoxFadeControl;
		mMessageBoxFadeControl = nullptr;

		delete mMainPane;
		mMainPane = nullptr;
	}

	void EditorState::pauseState()
	{
		mMainPane->setVisible(false);
	}

	void EditorState::resumeState()
	{
		mMainPane->setVisible(true);
	}

	void EditorState::updateCaption()
	{
		addUserTag("SE_HasChanged", ActionManager::getInstance().getChanges() ? "*" : "");
		Application::getInstance().setCaption(replaceTags("CaptionMainWindow"));
	}

	void EditorState::commandLoad(const MyGUI::UString& _commandName)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		if (!StateManager::getInstance().getStateActivate(this))
			return;

		if (ActionManager::getInstance().getChanges())
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
		if (!StateManager::getInstance().getStateActivate(this))
			return;

		if (ActionManager::getInstance().getChanges())
		{
			save();
		}
	}

	void EditorState::commandSaveAs(const MyGUI::UString& _commandName)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		if (!StateManager::getInstance().getStateActivate(this))
			return;

		showSaveAsWindow();
	}

	void EditorState::commandClear(const MyGUI::UString& _commandName)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		if (!StateManager::getInstance().getStateActivate(this))
			return;

		if (ActionManager::getInstance().getChanges())
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
		if (DialogManager::getInstance().getAnyDialog())
		{
			DialogManager::getInstance().endTopDialog();
		}
		else
		{
			if (MessageBoxManager::getInstance().hasAny())
			{
				MessageBoxManager::getInstance().endTop(MyGUI::MessageBoxStyle::Cancel);
			}
			else
			{
				if (StateManager::getInstance().getStateActivate(this))
				{
					if (ActionManager::getInstance().getChanges())
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
						Application::getInstance().quit();
					}
				}
				else
				{
					StateManager::getInstance().popState();
				}
			}
		}
	}

	void EditorState::commandFileDrop(const MyGUI::UString& _commandName)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		if (!StateManager::getInstance().getStateActivate(this))
			return;

		if (MessageBoxManager::getInstance().hasAny())
			return;

		mDropFileName = CommandManager::getInstance().getCommandData();
		if (mDropFileName.empty())
			return;

		if (ActionManager::getInstance().getChanges())
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
			loadDropFile();
		}
	}

	void EditorState::notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			save();
			clear();

			showLoadWindow();
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
			save();
			clear();

			loadDropFile();
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			clear();

			loadDropFile();
		}
	}

	void EditorState::loadDropFile()
	{
		mFileName = mDropFileName;
		addUserTag("SE_CurrentFileName", mFileName);

		load();
		updateCaption();
	}

	void EditorState::showLoadWindow()
	{
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionOpenFile"), replaceTags("ButtonOpenFile"));
		mOpenSaveFileDialog->setMode("Load");
		mOpenSaveFileDialog->doModal();
	}

	void EditorState::save()
	{
		MyGUI::xml::Document doc;
		doc.createDeclaration();
		MyGUI::xml::Element* root = doc.createRoot("Root");
		MyGUI::xml::Element* skins = root->createChild("SkinManager");

		SkinManager::getInstance().serialization(skins, MyGUI::Version());

		doc.save(mFileName);

		ActionManager::getInstance().setChanges(false);
	}

	void EditorState::clear()
	{
		SkinManager::getInstance().clear();
		ActionManager::getInstance().setChanges(false);

		mFileName = mDefaultFileName;
		addUserTag("SE_CurrentFileName", mFileName);

		updateCaption();
	}

	void EditorState::notifyEndDialog(Dialog* _sender, bool _result)
	{
		if (_result)
		{
			if (mOpenSaveFileDialog->getMode() == "SaveAs")
			{
				mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				addUserTag("SE_CurrentFileName", mFileName);

				save();
				updateCaption();
			}
			else if (mOpenSaveFileDialog->getMode() == "Load")
			{
				mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				addUserTag("SE_CurrentFileName", mFileName);

				load();
				updateCaption();
			}
		}

		mOpenSaveFileDialog->endModal();
	}

	void EditorState::load()
	{
		SkinManager::getInstance().clear();

		MyGUI::xml::Document doc;
		if (doc.open(mFileName))
		{
			bool result = false;
			MyGUI::xml::Element* root = doc.getRoot();
			if (root->getName() == "Root")
			{
				MyGUI::xml::ElementEnumerator nodes = root->getElementEnumerator();
				while (nodes.next("SkinManager"))
				{
					SkinManager::getInstance().deserialization(nodes.current(), MyGUI::Version());
					result = true;
					break;
				}
			}

			if (!result)
			{
				MyGUI::Message* message = MessageBoxManager::getInstance().create(
					replaceTags("Error"),
					replaceTags("MessageIncorrectFileFormat"),
					MyGUI::MessageBoxStyle::IconError
						| MyGUI::MessageBoxStyle::Yes);

				mFileName = mDefaultFileName;
				addUserTag("SE_CurrentFileName", mFileName);

				updateCaption();
			}
		}
		else
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Error"),
				doc.getLastError(),
				MyGUI::MessageBoxStyle::IconError
					| MyGUI::MessageBoxStyle::Yes);
		}

		ActionManager::getInstance().setChanges(false);
	}

	void EditorState::notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			save();
			clear();
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			clear();
		}
	}

	void EditorState::showSaveAsWindow()
	{
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionSaveFile"), replaceTags("ButtonSaveFile"));
		mOpenSaveFileDialog->setMode("SaveAs");
		mOpenSaveFileDialog->doModal();
	}

	void EditorState::notifyMessageBoxResultQuit(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			save();
			Application::getInstance().quit();
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			Application::getInstance().quit();
		}
	}

	void EditorState::commandTest(const MyGUI::UString & _commandName)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		if (!StateManager::getInstance().getStateActivate(this))
			return;

		SkinItem* item = SkinManager::getInstance().getItemSelected();
		if (item != nullptr)
		{
			mTestWindow->setSkinItem(item);
			mTestWindow->doModal();
		}
	}

	void EditorState::notifyEndDialogTest(Dialog* _sender, bool _result)
	{
		_sender->endModal();
	}

	void EditorState::notifyChanges(bool _changes)
	{
		updateCaption();
	}

} // namespace tools
