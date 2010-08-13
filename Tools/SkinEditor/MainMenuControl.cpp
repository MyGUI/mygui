/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "MainMenuControl.h"
#include "ActionManager.h"
#include "SkinManager.h"

namespace tools
{

	MainMenuControl::MainMenuControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("MainMenuControl.layout", _parent),
		mMainMenu(nullptr),
		mFileName("test.xml")
	{
		assignWidget(mMainMenu, "MainMenu");

		mMainMenu->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenuControl::notifyMenuCtrlAccept);
	}

	MainMenuControl::~MainMenuControl()
	{
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
			//message->setUserData(MenuCommandLoad);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &MainMenuControl::notifyMessageBoxResultLoad);
		}
		else
		{
			showLoadWindow();
		}

		/*if (ActionManager::getInstance().getChanges())
		{
		}
		else
		{
			//load();
			SkinManager::getInstance().clear();

			MyGUI::xml::Document doc;
			if (doc.open(std::string("test.xml")))
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
		}*/
	}

	void MainMenuControl::commandSave()
	{
		if (ActionManager::getInstance().getChanges())
		{
			save();

			ActionManager::getInstance().setChanges(false);
		}
	}

	void MainMenuControl::commandSaveAs()
	{
	}

	void MainMenuControl::commandClear()
	{
		/*if (ActionManager::getInstance().getChanges())
			showMessageBox(MenuCommandClear, L"Вы уверены что хотите все тут удалить?");
		else
			clear();*/
	}

	void MainMenuControl::commandQuit()
	{
	}

	/*void MainMenuControl::save(const MyGUI::UString& _fileName)
	{
		MyGUI::xml::Document doc;
		doc.open(_fileName);
		doc.clear();
		doc.createDeclaration();
		MyGUI::xml::Element* root = doc.createRoot("Root");
		MyGUI::xml::Element* skins = root->createChild("SkinManager");

		SkinManager::getInstance().serialization(skins, MyGUI::Version());

		doc.save(_fileName);
	}

	void MainMenuControl::showMessageBox(MenuCommand _command, const MyGUI::UString& _text)
	{
		MyGUI::Message* message = MyGUI::Message::createMessageBox(
			"Message",
			L"Внимание",
			L"Вы уверены?",
			MyGUI::MessageBoxStyle::IconQuest | MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No);
		message->setUserData(_command);
		message->eventMessageBoxResult += MyGUI::newDelegate(this, &MainMenuControl::notifyMessageBoxResult);
	}

	void MainMenuControl::notifyMessageBoxResult(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			MenuCommand command = *_sender->getUserData<MenuCommand>();
			if (command == MenuCommandClear)
				clear();
		}
	}

	void MainMenuControl::clear()
	{
		SkinManager::getInstance().clear();
	}*/

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
	}

	void MainMenuControl::save()
	{
		MyGUI::xml::Document doc;
		doc.open(mFileName);
		doc.clear();
		doc.createDeclaration();
		MyGUI::xml::Element* root = doc.createRoot("Root");
		MyGUI::xml::Element* skins = root->createChild("SkinManager");

		SkinManager::getInstance().serialization(skins, MyGUI::Version());

		doc.save(mFileName);
	}

	void MainMenuControl::clear()
	{
		SkinManager::getInstance().clear();
		ActionManager::getInstance().setChanges(false);
	}

} // namespace tools
