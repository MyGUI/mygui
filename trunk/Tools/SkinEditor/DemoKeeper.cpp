/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "SkinManager.h"
#include "ActionManager.h"
#include "CommandManager.h"
#include "ExportManager.h"
#include "MyGUI_FilterNoneSkin.h"
#include "FileSystemInfo/FileSystemInfo.h"

template <> tools::DemoKeeper* MyGUI::Singleton<tools::DemoKeeper>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::DemoKeeper>::mClassTypeName("DemoKeeper");

namespace tools
{

	DemoKeeper::DemoKeeper() :
		mMainPane(nullptr),
		mChanges(false),
		mFileName("unnamed.xml"),
		mDefaultFileName("unnamed.xml"),
		mOpenSaveFileDialog(nullptr),
		mTestWindow(nullptr),
		mMessageBox(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Tools/SkinEditor");
		setResourceFilename("editor.xml");
	}

	void DemoKeeper::createScene()
	{
		if (!mLocale.empty())
			MyGUI::LanguageManager::getInstance().setCurrentLanguage(mLocale);

		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::FilterNone>("BasisSkin");

		CommandManager* commandManager = new CommandManager();
		commandManager->initialise();

		SkinManager* skinManager = new SkinManager();
		skinManager->initialise();

		ActionManager* actionManager = new ActionManager();
		actionManager->initialise();

		ExportManager* exportManager = new ExportManager();
		exportManager->initialise();

		mMainPane = new MainPane();

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDialog);
		mOpenSaveFileDialog->setFileMask("*.xml");

		mTestWindow = new TestWindow();
		mTestWindow->eventEndDialog = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDialogTest);

		CommandManager::getInstance().registerCommand("Command_FileLoad", MyGUI::newDelegate(this, &DemoKeeper::commandLoad));
		CommandManager::getInstance().registerCommand("Command_FileSave", MyGUI::newDelegate(this, &DemoKeeper::commandSave));
		CommandManager::getInstance().registerCommand("Command_FileSaveAs", MyGUI::newDelegate(this, &DemoKeeper::commandSaveAs));
		CommandManager::getInstance().registerCommand("Command_ClearAll", MyGUI::newDelegate(this, &DemoKeeper::commandClear));
		CommandManager::getInstance().registerCommand("Command_Test", MyGUI::newDelegate(this, &DemoKeeper::commandTest));
		CommandManager::getInstance().registerCommand("Command_QuitApp", MyGUI::newDelegate(this, &DemoKeeper::commandQuit));

		updateCaption();
	}

	void DemoKeeper::destroyScene()
	{
		delete mTestWindow;
		mTestWindow = nullptr;

		mOpenSaveFileDialog->eventEndDialog = nullptr;
		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;

		delete mMainPane;
		mMainPane = nullptr;

		ExportManager* exportManager = ExportManager::getInstancePtr();
		exportManager->shutdown();
		delete exportManager;

		ActionManager* actionManager = ActionManager::getInstancePtr();
		actionManager->shutdown();
		delete actionManager;

		SkinManager* skinManager = SkinManager::getInstancePtr();
		skinManager->shutdown();
		delete skinManager;

		CommandManager* commandManager = CommandManager::getInstancePtr();
		commandManager->shutdown();
		delete commandManager;

		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>("BasisSkin");
	}

	void DemoKeeper::prepare()
	{
		// устанавливаем локаль из переменной окружения
		// без этого не будут открываться наши файлы
		mLocale = ::setlocale( LC_ALL, "" );
		// erase everything after '_' to get language name
		mLocale.erase(std::find(mLocale.begin(), mLocale.end(), '_'), mLocale.end());
		if (mLocale == "ru")
			mLocale = "Russian";
		else if (mLocale == "en")
			mLocale = "English";
	}

	void DemoKeeper::onFileDrop(const std::wstring& _filename)
	{
	}

	bool DemoKeeper::onWinodwClose(size_t _handle)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	if (::IsIconic((HWND)_handle))
		ShowWindow((HWND)_handle, SW_SHOWNORMAL);
#endif

		CommandManager::getInstance().executeCommand("Command_QuitApp");
		return false;
	}

	void DemoKeeper::setChanges(bool _value)
	{
		mChanges = _value;
		updateCaption();
	}

	void DemoKeeper::updateCaption()
	{
		std::wstring result = L"Skin editor - '";
		result += mFileName;
		result += L"' ";
		result += mChanges ? L"*" : L"";

		setWindowCaption(result);
	}

	void DemoKeeper::commandLoad(const MyGUI::UString & _commandName)
	{
		if (ActionManager::getInstance().getChanges())
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				MyGUI::LanguageManager::getInstance().replaceTags("#{Warning}"),
				MyGUI::LanguageManager::getInstance().replaceTags("#{WarningUnsavedData}"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &DemoKeeper::notifyMessageBoxResultLoad);
			registerMessageBox(message);
		}
		else
		{
			showLoadWindow();
		}
	}

	void DemoKeeper::commandSave(const MyGUI::UString & _commandName)
	{
		if (ActionManager::getInstance().getChanges())
		{
			save();
		}
	}

	void DemoKeeper::commandSaveAs(const MyGUI::UString & _commandName)
	{
		showSaveAsWindow();
	}

	void DemoKeeper::commandClear(const MyGUI::UString & _commandName)
	{
		if (ActionManager::getInstance().getChanges())
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				MyGUI::LanguageManager::getInstance().replaceTags("#{Warning}"),
				MyGUI::LanguageManager::getInstance().replaceTags("#{WarningUnsavedData}"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &DemoKeeper::notifyMessageBoxResultClear);
			registerMessageBox(message);
		}
		else
		{
			clear();
		}
	}

	void DemoKeeper::commandQuit(const MyGUI::UString & _commandName)
	{
		if (Dialog::getAnyDialog())
		{
			Dialog::endTopDialog();
		}
		else
		{
			if (mMessageBox != nullptr)
			{
				mMessageBox->endMessage(MyGUI::MessageBoxStyle::Cancel);
			}
			else
			{
				if (mChanges)
				{
					MyGUI::Message* message = MyGUI::Message::createMessageBox(
						"Message",
						MyGUI::LanguageManager::getInstance().replaceTags("#{Warning}"),
						MyGUI::LanguageManager::getInstance().replaceTags("#{WarningUnsavedData}"),
						MyGUI::MessageBoxStyle::IconQuest
							| MyGUI::MessageBoxStyle::Yes
							| MyGUI::MessageBoxStyle::No
							| MyGUI::MessageBoxStyle::Cancel);
					message->eventMessageBoxResult += MyGUI::newDelegate(this, &DemoKeeper::notifyMessageBoxResultQuit);
					registerMessageBox(message);
				}
				else
				{
					quit();
				}
			}
		}
	}

	void DemoKeeper::notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Cancel)
		{
		}
		else if (_result == MyGUI::MessageBoxStyle::Yes)
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

	void DemoKeeper::showLoadWindow()
	{
		mOpenSaveFileDialog->setDialogInfo("Load", "Load");
		mOpenSaveFileDialog->setMode("Load");
		mOpenSaveFileDialog->setVisible(true);
	}

	void DemoKeeper::save()
	{
		MyGUI::xml::Document doc;
		doc.createDeclaration();
		MyGUI::xml::Element* root = doc.createRoot("Root");
		MyGUI::xml::Element* skins = root->createChild("SkinManager");

		SkinManager::getInstance().serialization(skins, MyGUI::Version());

		doc.save(mFileName);

		ActionManager::getInstance().setChanges(false);
	}

	void DemoKeeper::clear()
	{
		SkinManager::getInstance().clear();
		ActionManager::getInstance().setChanges(false);

		mFileName = mDefaultFileName;
		updateCaption();
	}

	void DemoKeeper::notifyEndDialog(Dialog* _sender, bool _result)
	{
		if (_result)
		{
			if (mOpenSaveFileDialog->getMode() == "SaveAs")
			{
				mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				save();
				updateCaption();
			}
			else if (mOpenSaveFileDialog->getMode() == "Load")
			{
				mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				load();
				updateCaption();
			}
		}

		mOpenSaveFileDialog->setVisible(false);
	}

	void DemoKeeper::load()
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
				MyGUI::UString text = L"Файл '" + mFileName + L"' не соответсвует формату.";
				MyGUI::Message* message = MyGUI::Message::createMessageBox(
					"Message",
					MyGUI::LanguageManager::getInstance().replaceTags("#{Error}"),
					text,
					MyGUI::MessageBoxStyle::IconError
						| MyGUI::MessageBoxStyle::Yes);
				registerMessageBox(message);

				mFileName = mDefaultFileName;
				updateCaption();
			}
		}
		else
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				MyGUI::LanguageManager::getInstance().replaceTags("#{Error}"),
				doc.getLastError(),
				MyGUI::MessageBoxStyle::IconError
					| MyGUI::MessageBoxStyle::Yes);
			registerMessageBox(message);
		}

		ActionManager::getInstance().setChanges(false);
	}

	void DemoKeeper::notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Cancel)
		{
		}
		else if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			save();
			clear();
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			clear();
		}
	}

	void DemoKeeper::showSaveAsWindow()
	{
		mOpenSaveFileDialog->setDialogInfo("SaveAs", "Save");
		mOpenSaveFileDialog->setMode("SaveAs");
		mOpenSaveFileDialog->setVisible(true);
	}

	void DemoKeeper::notifyMessageBoxResultQuit(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Cancel)
		{
		}
		else if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			save();
			quit();
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			quit();
		}
	}

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (MyGUI::Gui::getInstancePtr() == nullptr)
			return;

		if (_key == MyGUI::KeyCode::Escape)
		{
			CommandManager::getInstance().executeCommand("Command_QuitApp");
			return;
		}

		if (MyGUI::InputManager::getInstance().isControlPressed())
		{
			if (_key == MyGUI::KeyCode::O || _key == MyGUI::KeyCode::L)
				CommandManager::getInstance().executeCommand("Command_FileLoad");
			else if (_key == MyGUI::KeyCode::S)
				CommandManager::getInstance().executeCommand("Command_FileSave");

			return;
		}

		MyGUI::InputManager::getInstance().injectKeyPress(_key, _text);
	}

	void DemoKeeper::registerMessageBox(MyGUI::Message* _message)
	{
		MYGUI_ASSERT(mMessageBox == nullptr, "Message box already registred");
		mMessageBox = _message;
		mMessageBox->eventMessageBoxResult += MyGUI::newDelegate(this, &DemoKeeper::notifMessageBoxResultRegister);
	}

	void DemoKeeper::notifMessageBoxResultRegister(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		mMessageBox = nullptr;
	}

	void DemoKeeper::commandTest(const MyGUI::UString & _commandName)
	{
		SkinItem* item = SkinManager::getInstance().getItemSelected();
		if (item != nullptr)
		{
			mTestWindow->setSkinItem(item);
			mTestWindow->setVisible(true);
		}
	}

	void DemoKeeper::notifyEndDialogTest(Dialog* _sender, bool _result)
	{
		_sender->setVisible(false);
	}

} // namespace tools

MYGUI_APP(tools::DemoKeeper)
