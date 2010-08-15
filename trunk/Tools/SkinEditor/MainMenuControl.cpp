/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "MainMenuControl.h"
#include "ActionManager.h"
#include "SkinManager.h"
#include "MainWindowManager.h"

namespace tools
{

	MainMenuControl::MainMenuControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("MainMenuControl.layout", _parent),
		mMainMenu(nullptr),
		mOpenSaveFileDialog(nullptr)
	{
		assignWidget(mMainMenu, "MainMenu");

		mMainMenu->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenuControl::notifyMenuCtrlAccept);

		mOpenSaveFileDialog = new common::OpenSaveFileDialog();
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &MainMenuControl::notifyEndDialog);
		mOpenSaveFileDialog->setFileMask("*.xml");
	}

	MainMenuControl::~MainMenuControl()
	{
		mOpenSaveFileDialog->eventEndDialog = nullptr;
		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;

		mMainMenu->eventMenuCtrlAccept -= MyGUI::newDelegate(this, &MainMenuControl::notifyMenuCtrlAccept);
	}

	void MainMenuControl::notifyMenuCtrlAccept(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item)
	{
		const std::string& command = _item->getItemId();
		if (command == "File/Load")
			commandLoad();
		else if (command == "File/Save")
			commandSave();
		else if (command == "File/SaveAs")
			commandSaveAs();
		else if (command == "File/Clear")
			commandClear();
		else if (command == "File/Quit")
			commandQuit();
	}

	void MainMenuControl::commandLoad()
	{
		if (ActionManager::getInstance().getChanges())
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"Внимание",
				L"Сохранить изменения?",
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &MainMenuControl::notifyMessageBoxResultLoad);
		}
		else
		{
			showLoadWindow();
		}
	}

	void MainMenuControl::commandSave()
	{
		if (ActionManager::getInstance().getChanges())
		{
			save();
		}
	}

	void MainMenuControl::commandSaveAs()
	{
		showSaveAsWindow();
	}

	void MainMenuControl::commandClear()
	{
		if (ActionManager::getInstance().getChanges())
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"Внимание",
				L"Сохранить изменения?",
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &MainMenuControl::notifyMessageBoxResultClear);
		}
		else
		{
			clear();
		}
	}

	void MainMenuControl::commandQuit()
	{
	}

	void MainMenuControl::notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
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

	void MainMenuControl::showLoadWindow()
	{
		mOpenSaveFileDialog->setDialogInfo("Load", "Load");
		mOpenSaveFileDialog->setModeSave(false);
		mOpenSaveFileDialog->setVisible(true);
	}

	void MainMenuControl::save()
	{
		if (mFileName != "")
		{
			MyGUI::xml::Document doc;
			doc.createDeclaration();
			MyGUI::xml::Element* root = doc.createRoot("Root");
			MyGUI::xml::Element* skins = root->createChild("SkinManager");

			SkinManager::getInstance().serialization(skins, MyGUI::Version());

			doc.save(mFileName);

			ActionManager::getInstance().setChanges(false);
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

	void MainMenuControl::clear()
	{
		SkinManager::getInstance().clear();
		ActionManager::getInstance().setChanges(false);

		mFileName = "";
		updateWidgetCaption();
	}

	void MainMenuControl::notifyEndDialog(bool _result)
	{
		if (_result)
		{
			if (mOpenSaveFileDialog->getModeSave())
			{
				mFileName = mOpenSaveFileDialog->getFileName();
				save();
			}
			else
			{
				mFileName = mOpenSaveFileDialog->getFileName();
				load();
			}

			updateWidgetCaption();
		}

		mOpenSaveFileDialog->setVisible(false);
	}

	void MainMenuControl::load()
	{
		SkinManager::getInstance().clear();

		MyGUI::xml::Document doc;
		if (doc.open(mFileName))
		{
			MyGUI::xml::Element* root = doc.getRoot();
			if (root->getName() == "Root")
			{
				MyGUI::xml::ElementEnumerator nodes = root->getElementEnumerator();
				while (nodes.next("SkinManager"))
				{
					SkinManager::getInstance().deserialization(nodes.current(), MyGUI::Version());
					break;
				}
			}
		}
		else
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"Ошибка",
				MyGUI::utility::toString(L"Ошибка открытия файла '", mFileName, L"'"),
				MyGUI::MessageBoxStyle::IconError
					| MyGUI::MessageBoxStyle::Yes);
		}

		ActionManager::getInstance().setChanges(false);
	}

	void MainMenuControl::notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
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

	void MainMenuControl::showSaveAsWindow()
	{
		mOpenSaveFileDialog->setDialogInfo("SaveAs", "Save");
		mOpenSaveFileDialog->setModeSave(true);
		mOpenSaveFileDialog->setVisible(true);
	}

	void MainMenuControl::updateWidgetCaption()
	{
		MainWindowManager::getInstance().setFileName(mFileName);
	}

} // namespace tools
