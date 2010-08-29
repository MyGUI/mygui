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
	const size_t MAX_RECENT_FILES = 8;

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
				{
					/*if (field->getName() == "PropertiesPanelView") mPropertiesPanelView->load(field);
					else if (field->getName() == "SettingsWindow") mSettingsWindow->load(field);
					else if (field->getName() == "WidgetsWindow") mWidgetsWindow->load(field);
					else if (field->getName() == "MetaSolutionWindow")
					{
						if (isNeedSolutionLoad(field))
						{
							clearWidgetWindow();
							mMetaSolutionWindow->load(field);
						}
					}
					else */if (field->getName() == "RecentFile")
					{
						std::string name;
						if (!field->findAttribute("name", name)) continue;
						addRecentFile(name);
						//mRecentFiles.push_back(name);
					}
					else if (field->getName() == "AdditionalPath")
					{
						std::string name;
						if (!field->findAttribute("name", name)) continue;
						mAdditionalPaths.push_back(name);
					}
					else
					{
						loadSector(field.current());
					}
				}
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

		// cleanup for duplicates
		/*std::reverse(mRecentFiles.begin(), mRecentFiles.end());
		for (size_t i = 0; i < mRecentFiles.size(); ++i)
			mRecentFiles.erase(std::remove(mRecentFiles.begin() + i + 1, mRecentFiles.end(), mRecentFiles[i]), mRecentFiles.end());

		// remove old files
		while (mRecentFiles.size() > MAX_RECENT_FILES)
			mRecentFiles.pop_back();
		std::reverse(mRecentFiles.begin(), mRecentFiles.end());*/

		for (std::vector<MyGUI::UString>::iterator iter = mRecentFiles.begin(); iter != mRecentFiles.end(); ++iter)
		{
			MyGUI::xml::ElementPtr nodeProp = root->createChild("RecentFile");
			nodeProp->addAttribute("name", *iter);
		}

		for (std::vector<MyGUI::UString>::iterator iter = mAdditionalPaths.begin(); iter != mAdditionalPaths.end(); ++iter)
		{
			MyGUI::xml::ElementPtr nodeProp = root->createChild("AdditionalPath");
			nodeProp->addAttribute("name", *iter);
		}

		if (!doc.save(_fileName))
		{
			MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
			return;
		}
	}

	void SettingsManager::addRecentFile(const MyGUI::UString& _fileName)
	{
		VectorUString::iterator item = std::remove(mRecentFiles.begin(), mRecentFiles.end(), _fileName);
		if (item != mRecentFiles.end())
			mRecentFiles.erase(item);

		mRecentFiles.push_back(_fileName);

		while (mRecentFiles.size() > MAX_RECENT_FILES)
			mRecentFiles.erase(mRecentFiles.begin());

		eventSettingsChanged("Main", "RecentFiles");
	}

	void SettingsManager::loadSector(MyGUI::xml::ElementPtr _sectorNode)
	{
		SettingsSector* sector = new SettingsSector();
		sector->eventSettingsChanged = MyGUI::newDelegate(this, &SettingsManager::notifySettingsChanged);

		sector->deserialization(_sectorNode, MyGUI::Version());

		mSettings.push_back(sector);
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
