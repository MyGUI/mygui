/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#ifndef DEMO_RESOURCE_DEVICE_H_
#define DEMO_RESOURCE_DEVICE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_ResourceManager.h"

namespace demo
{

	class ResourceDevice : public MyGUI::IResource, public MyGUI::GenericFactory<ResourceDevice>
	{
		friend class MyGUI::GenericFactory<ResourceDevice>;

		MYGUI_RTTI_DERIVED(ResourceDevice)

	private:
		void deserialization(pugi::xml_node _node, MyGUI::Version _version) override
		{
			Base::deserialization(_node, _version);

			for (auto node : _node.children())
			{
				if (std::string_view{node.name()} == "Name")
					mDeviceName = node.text().as_string();
				else if (std::string_view{node.name()} == "Description")
					mDeviceDescription = node.text().as_string();
				else if (std::string_view{node.name()} == "ValueEnergy")
					mValueEnergy = MyGUI::utility::parseValue<size_t>(node.text().as_string());
				else if (std::string_view{node.name()} == "ValueExplosion")
					mValueExplosion = MyGUI::utility::parseValue<size_t>(node.text().as_string());
				else if (std::string_view{node.name()} == "ValueTarget")
					mValueTarget = MyGUI::utility::parseValue<size_t>(node.text().as_string());
				else if (std::string_view{node.name()} == "ValueHP")
					mValueHP = MyGUI::utility::parseValue<size_t>(node.text().as_string());
			}
		}

	public:
		const std::string& getDeviceName()
		{
			return mDeviceName;
		}
		const std::string& getDeviceDescription()
		{
			return mDeviceDescription;
		}
		size_t getValueEnergy() const
		{
			return mValueEnergy;
		}
		size_t getValueExplosion() const
		{
			return mValueExplosion;
		}
		size_t getValueTarget() const
		{
			return mValueTarget;
		}
		size_t getValueHP() const
		{
			return mValueHP;
		}

	private:
		std::string mDeviceName;
		std::string mDeviceDescription;
		size_t mValueEnergy{0};
		size_t mValueExplosion{0};
		size_t mValueTarget{0};
		size_t mValueHP{0};
	};

} // namespace demo

#endif // DEMO_RESOURCE_DEVICE_H_
