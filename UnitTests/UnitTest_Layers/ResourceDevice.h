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

	class ResourceDevice :
		public MyGUI::IResource,
		public MyGUI::GenericFactory<ResourceDevice>
	{
		friend class MyGUI::GenericFactory<ResourceDevice>;

		MYGUI_RTTI_DERIVED( ResourceDevice )

	private:
		ResourceDevice() :
			mValueEnergy(0),
			mValueExplosion(0),
			mValueTarget(0),
			mValueHP(0)
		{
		}
		virtual ~ResourceDevice() { }

		virtual void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
		{
			Base::deserialization(_node, _version);

			MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
			while (node.next())
			{
				if (node->getName() == "Name") mDeviceName = node->getContent();
				else if (node->getName() == "Description") mDeviceDescription = node->getContent();
				else if (node->getName() == "ValueEnergy") mValueEnergy = MyGUI::utility::parseValue<size_t>(node->getContent());
				else if (node->getName() == "ValueExplosion") mValueExplosion = MyGUI::utility::parseValue<size_t>(node->getContent());
				else if (node->getName() == "ValueTarget") mValueTarget = MyGUI::utility::parseValue<size_t>(node->getContent());
				else if (node->getName() == "ValueHP") mValueHP = MyGUI::utility::parseValue<size_t>(node->getContent());
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
		size_t getValueEnergy()
		{
			return mValueEnergy;
		}
		size_t getValueExplosion()
		{
			return mValueExplosion;
		}
		size_t getValueTarget()
		{
			return mValueTarget;
		}
		size_t getValueHP()
		{
			return mValueHP;
		}

	private:
		std::string mDeviceName;
		std::string mDeviceDescription;
		size_t mValueEnergy;
		size_t mValueExplosion;
		size_t mValueTarget;
		size_t mValueHP;

	};

} // namespace demo

#endif // DEMO_RESOURCE_DEVICE_H_
