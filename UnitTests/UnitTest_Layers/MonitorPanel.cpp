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
			MyGUI::IResource* base_resource = MyGUI::ResourceManager::getInstance().getByName(*data, false);
			if (base_resource != nullptr)
			{
				ResourceDevice* device = base_resource->castType<ResourceDevice>(false);
				if (device != nullptr)
				{
					mShipImage->setItemName(device->getDeviceName());
					mDescription->setCaption(device->getDeviceDescription());
					mEnergy->setProgressPosition(device->getValueEnergy());
					mExplosion->setProgressPosition(device->getValueExplosion());
					mTarget->setProgressPosition(device->getValueTarget());
					mHP->setProgressPosition(device->getValueHP());
				}
			}

		}
	}

 } // namespace demo
