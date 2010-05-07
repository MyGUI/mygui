/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "MonitorPanel.h"
#include "CommandManager.h"
#include "ResourceDevice.h"

namespace demo
{

	MonitorPanel::MonitorPanel()
	{
		initialiseByAttributes(this);

		mMainWidget->setPosition(0, 0);

		CommandManager::getInstance().registerCommand("KeyboardClick", MyGUI::newDelegate(this, &MonitorPanel::notifyCommand));
	}

	MonitorPanel::~MonitorPanel()
	{
		CommandManager::getInstance().unregisterCommand("KeyboardClick");
	}

	void MonitorPanel::notifyCommand(const std::string& _name, MyGUI::Any _data)
	{
		std::string* data = _data.castType<std::string>(false);
		if (data != nullptr)
		{
			MyGUI::IResource* base_resource = MyGUI::ResourceManager::getInstance().getByName(*data, false);
			if (base_resource != nullptr)
			{
				ResourceDevice* device = base_resource->castType<ResourceDevice>(false);
				if (device != nullptr)
				{
					mShipImage->setItemName(device->getDeviceName());
					mDescription->setProperty("Widget_Caption", device->getDeviceDescription());
					mEnergy->setProperty("Progress_Position", MyGUI::utility::toString(device->getValueEnergy()));
					mExplosion->setProperty("Progress_Position", MyGUI::utility::toString(device->getValueExplosion()));
					mTarget->setProperty("Progress_Position", MyGUI::utility::toString(device->getValueTarget()));
					mHP->setProperty("Progress_Position", MyGUI::utility::toString(device->getValueHP()));
				}
			}

		}
	}

 } // namespace demo
