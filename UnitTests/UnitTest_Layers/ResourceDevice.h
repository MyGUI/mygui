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
		void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version) override
		{
			Base::deserialization(_node, _version);

			MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
			while (node.next())
			{
				if (node->getName() == "Name")
					mDeviceName = node->getContent();
				else if (node->getName() == "Description")
					mDeviceDescription = node->getContent();
				else if (node->getName() == "ValueEnergy")
					mValueEnergy = MyGUI::utility::parseValue<size_t>(node->getContent());
				else if (node->getName() == "ValueExplosion")
					mValueExplosion = MyGUI::utility::parseValue<size_t>(node->getContent());
				else if (node->getName() == "ValueTarget")
					mValueTarget = MyGUI::utility::parseValue<size_t>(node->getContent());
				else if (node->getName() == "ValueHP")
					mValueHP = MyGUI::utility::parseValue<size_t>(node->getContent());
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
