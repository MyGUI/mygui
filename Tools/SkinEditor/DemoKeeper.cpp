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
#include "MyGUI_FilterNoneSkin.h"
#include "FileSystemInfo/FileSystemInfo.h"

template <> demo::DemoKeeper* MyGUI::Singleton<demo::DemoKeeper>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<demo::DemoKeeper>::mClassTypeName("DemoKeeper");

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mMainPane(nullptr),
		mChanges(false),
		mOpenSaveFileDialog(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Tools/SkinEditor");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::FilterNone>("BasisSkin");

		MyGUI::ResourceManager::getInstance().load("SE_skins.xml");

		tools::CommandManager* commandManager = new tools::CommandManager();
		commandManager->initialise();

		tools::SkinManager* skinManager = new tools::SkinManager();
		skinManager->initialise();

		tools::ActionManager* actionManager = new tools::ActionManager();
		actionManager->initialise();

		mMainPane = new tools::MainPane();

		mOpenSaveFileDialog = new common::OpenSaveFileDialog();
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDialog);
		mOpenSaveFileDialog->setFileMask("*.xml");

		tools::CommandManager::getInstance().registerCommand("Command_FileLoad", MyGUI::newDelegate(this, &DemoKeeper::commandLoad));
		tools::CommandManager::getInstance().registerCommand("Command_FileSave", MyGUI::newDelegate(this, &DemoKeeper::commandSave));
		tools::CommandManager::getInstance().registerCommand("Command_FileSaveAs", MyGUI::newDelegate(this, &DemoKeeper::commandSaveAs));
		tools::CommandManager::getInstance().registerCommand("Command_ClearAll", MyGUI::newDelegate(this, &DemoKeeper::commandClear));
		tools::CommandManager::getInstance().registerCommand("Command_QuitApp", MyGUI::newDelegate(this, &DemoKeeper::commandQuit));

		updateCaption();
	}

	void DemoKeeper::destroyScene()
	{
		mOpenSaveFileDialog->eventEndDialog = nullptr;
		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;

		delete mMainPane;
		mMainPane = nullptr;

		tools::ActionManager* actionManager = tools::ActionManager::getInstancePtr();
		actionManager->shutdown();
		delete actionManager;

		tools::SkinManager* skinManager = tools::SkinManager::getInstancePtr();
		skinManager->shutdown();
		delete skinManager;

		tools::CommandManager* commandManager = tools::CommandManager::getInstancePtr();
		commandManager->shutdown();
		delete commandManager;

		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>("BasisSkin");
	}

	void DemoKeeper::onFileDrop(const std::wstring& _filename)
	{
	}

	bool DemoKeeper::onWinodwClose(size_t _handle)
	{
		return true;
	}

	void DemoKeeper::setChanges(bool _value)
	{
		mChanges = _value;
		updateCaption();
	}

	void DemoKeeper::updateCaption()
	{
		std::wstring result = L"Skin editor - '";
		result += mFileName.empty() ? L"unnamed" : mFileName;
		result += L"' ";
		result += mChanges ? L"*" : L"";

		setWindowCaption(result);
	}

	void DemoKeeper::commandLoad(const MyGUI::UString & _commandName)
	{
		if (tools::ActionManager::getInstance().getChanges())
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"Внимание",
				L"Сохранить изменения?",
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &DemoKeeper::notifyMessageBoxResultLoad);
		}
		else
		{
			showLoadWindow();
		}
	}

	void DemoKeeper::commandSave(const MyGUI::UString & _commandName)
	{
		if (tools::ActionManager::getInstance().getChanges())
		{
			if (mFileName.empty())
				showSaveAsWindow();
			else
				save();
		}
	}

	void DemoKeeper::commandSaveAs(const MyGUI::UString & _commandName)
	{
		showSaveAsWindow();
	}

	void DemoKeeper::commandClear(const MyGUI::UString & _commandName)
	{
		if (tools::ActionManager::getInstance().getChanges())
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"Внимание",
				L"Сохранить изменения?",
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &DemoKeeper::notifyMessageBoxResultClear);
		}
		else
		{
			clear();
		}
	}

	void DemoKeeper::commandQuit(const MyGUI::UString & _commandName)
	{
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
		mOpenSaveFileDialog->setModeSave(false);
		mOpenSaveFileDialog->setVisible(true);
	}

	void DemoKeeper::save()
	{
		if (mFileName != "")
		{
			MyGUI::xml::Document doc;
			doc.createDeclaration();
			MyGUI::xml::Element* root = doc.createRoot("Root");
			MyGUI::xml::Element* skins = root->createChild("SkinManager");

			tools::SkinManager::getInstance().serialization(skins, MyGUI::Version());

			doc.save(mFileName);

			tools::ActionManager::getInstance().setChanges(false);
		}
		else
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"Ошибка",
				L"Не указано имя файла",
				MyGUI::MessageBoxStyle::IconError
					| MyGUI::MessageBoxStyle::Yes);
		}
	}

	void DemoKeeper::clear()
	{
		tools::SkinManager::getInstance().clear();
		tools::ActionManager::getInstance().setChanges(false);

		mFileName = "";
		updateCaption();
	}

	void DemoKeeper::notifyEndDialog(bool _result)
	{
		if (_result)
		{
			mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());

			if (mOpenSaveFileDialog->getModeSave())
				save();
			else
				load();

			updateCaption();
		}

		mOpenSaveFileDialog->setVisible(false);
	}

	void DemoKeeper::load()
	{
		tools::SkinManager::getInstance().clear();

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
					tools::SkinManager::getInstance().deserialization(nodes.current(), MyGUI::Version());
					result = true;
					break;
				}
			}

			if (!result)
			{
				MyGUI::UString text = L"Файл '" + mFileName + L"' не соответсвует формату.";
				MyGUI::Message* message = MyGUI::Message::createMessageBox(
					"Message",
					L"Ошибка",
					text,
					MyGUI::MessageBoxStyle::IconError
						| MyGUI::MessageBoxStyle::Yes);

				mFileName = "";
				updateCaption();
			}
		}
		else
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"Ошибка",
				doc.getLastError(),
				MyGUI::MessageBoxStyle::IconError
					| MyGUI::MessageBoxStyle::Yes);
		}

		tools::ActionManager::getInstance().setChanges(false);
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
		mOpenSaveFileDialog->setModeSave(true);
		mOpenSaveFileDialog->setVisible(true);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
