/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "MonitorPanel.h"
#include "CommandManager.h"

namespace demo
{

	MonitorPanel::MonitorPanel()
	{
		initialiseByAttributes(this);

		mMainWidget->setPosition(0, 0);

		CommandManager::getInstance().registryCommand("KeyboardClick", MyGUI::newDelegate(this, &MonitorPanel::notifyCommand));
	}

	MonitorPanel::~MonitorPanel()
	{
		CommandManager::getInstance().unregistryCommand("KeyboardClick");
	}

	void MonitorPanel::notifyCommand(const std::string& _name, MyGUI::Any _data)
	{
		std::string* data = _data.castType<std::string>(false);
		if (data != nullptr)
		{
			mShipImage->setItemName(*data);
		}
	}

 } // namespace demo
