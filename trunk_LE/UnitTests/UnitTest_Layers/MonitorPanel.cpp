/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
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
					mDescription->setProperty("Caption", device->getDeviceDescription());
					mEnergy->setProperty("RangePosition", MyGUI::utility::toString(device->getValueEnergy()));
					mExplosion->setProperty("RangePosition", MyGUI::utility::toString(device->getValueExplosion()));
					mTarget->setProperty("RangePosition", MyGUI::utility::toString(device->getValueTarget()));
					mHP->setProperty("RangePosition", MyGUI::utility::toString(device->getValueHP()));
				}
			}

		}
	}

} // namespace demo
