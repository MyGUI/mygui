/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "MainMenuControl.h"
//#include "ActionManager.h"
#include "SkinManager.h"

namespace tools
{

	/*enum MenuCommand
	{
		MenuCommandClear
	};*/

	MainMenuControl::MainMenuControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("MainMenuControl.layout", _parent),
		mMainMenu(nullptr)//,
		//mMessageBox(nullptr)
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
		/*if (ActionManager::getInstance().hasChanges())
		{
		}
		else
		{
		}*/
	}

	void MainMenuControl::commandSave()
	{
		MyGUI::xml::Document doc;
		doc.open(std::string("test.xml"));
		doc.clear();
		doc.createDeclaration();
		MyGUI::xml::Element* root = doc.createRoot("Root");
		MyGUI::xml::Element* skins = root->createChild("SkinManager");

		SkinManager::getInstance().serialization(skins, MyGUI::Version());

		doc.save(std::string("test.xml"));
	}

	void MainMenuControl::commandSaveAs()
	{
	}

	void MainMenuControl::commandClear()
	{
		SkinManager::getInstance().clear();
		/*if (ActionManager::getInstance().hasChanges())
		{
			showMessageBox(MenuCommandClear, "¬ы уверены что хотите все тут удалить?");
		}*/
	}

	void MainMenuControl::commandQuit()
	{
	}

	/*void MainMenuControl::hideMessageBox()
	{
	}

	void MainMenuControl::showMessageBox(MenuCommand _command, const MyGUI::UString& _text)
	{
		hideMessageBox();
	}*/

} // namespace tools
