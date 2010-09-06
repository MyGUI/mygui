/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SettingsManager.h"

template <> tools::SettingsManager* MyGUI::Singleton<tools::SettingsManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::SettingsManager>::mClassTypeName("SettingsManager");

namespace tools
{
	const std::string LogSection = "LayoutEditor";

	const std::wstring settingsFile = L"settings.xml";
	const std::wstring userSettingsFile = L"le_user_settings.xml";

	SettingsManager::SettingsManager()
	{
	}

	SettingsManager::~SettingsManager()
	{
		destroyAllSectors();
	}

	void SettingsManager::initialise()
	{
		loadSettings(settingsFile, true);
		loadSettings(userSettingsFile, false);
	}

	void SettingsManager::shutdown()
	{
		saveSettings(userSettingsFile);
		destroyAllSectors();
	}

	void SettingsManager::loadSettings(const MyGUI::UString& _fileName, bool _internal)
	{
		std::string _instance = "Editor";

		MyGUI::xml::Document doc;
		if (_internal)
		{
			MyGUI::DataStreamHolder data = MyGUI::DataManager::getInstance().getData(_fileName);
			if (data.getData() != nullptr)
			{
				if (!doc.open(data.getData()))
				{
					MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
					return;
				}
			}
		}
		else
		{
			if (!doc.open(_fileName))
			{
				MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
				return;
			}
		}

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if (!root || (root->getName() != "MyGUI"))
		{
			MYGUI_LOGGING(LogSection, Error, _instance << " : '" << _fileName << "', tag 'MyGUI' not found");
			return;
		}

		std::string type;
		if (root->findAttribute("type", type))
		{
			if (type == "Settings")
			{
				// берем детей и крутимся
				MyGUI::xml::ElementEnumerator field = root->getElementEnumerator();
				while (field.next())
					loadSector(field.current());
			}
		}
	}

	void SettingsManager::saveSettings(const MyGUI::UString& _fileName)
	{
		std::string _instance = "Editor";

		MyGUI::xml::Document doc;
		doc.createDeclaration();
		MyGUI::xml::ElementPtr root = doc.createRoot("MyGUI");
		root->addAttribute("type", "Settings");

		saveSectors(root);

		if (!doc.save(_fileName))
		{
			MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
			return;
		}
	}

	void SettingsManager::loadSector(MyGUI::xml::ElementPtr _sectorNode)
	{
		SettingsSector* sector = getSector(_sectorNode->getName());
		sector->deserialization(_sectorNode, MyGUI::Version());
	}

	void SettingsManager::saveSectors(MyGUI::xml::ElementPtr _rootNode)
	{
		for (VectorSettingsSector::iterator item = mSettings.begin(); item != mSettings.end(); ++item)
			(*item)->serialization(_rootNode, MyGUI::Version());
	}

	void SettingsManager::destroyAllSectors()
	{
		for (VectorSettingsSector::iterator item = mSettings.begin(); item != mSettings.end(); ++item)
			delete (*item);
		mSettings.clear();
	}

	SettingsSector* SettingsManager::getSector(const MyGUI::UString& _sectorName)
	{
		for (VectorSettingsSector::iterator item = mSettings.begin(); item != mSettings.end(); ++item)
		{
			if ((*item)->getName() == _sectorName)
				return (*item);
		}

		SettingsSector* sector = new SettingsSector();
		sector->eventSettingsChanged = MyGUI::newDelegate(this, &SettingsManager::notifySettingsChanged);

		sector->setName(_sectorName);

		mSettings.push_back(sector);
		return sector;
	}

	void SettingsManager::notifySettingsChanged(SettingsSector* _sector, const MyGUI::UString& _propertyName)
	{
		eventSettingsChanged(_sector->getName(), _propertyName);
	}

} // namespace tools
