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
	}

	void SettingsManager::initialise()
	{
		loadSettings(settingsFile, true);
		loadSettings(userSettingsFile, false);
	}

	void SettingsManager::shutdown()
	{
		saveSettings(userSettingsFile);
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
						mRecentFiles.push_back(name);
					}
					else if (field->getName() == "AdditionalPath")
					{
						std::string name;
						if (!field->findAttribute("name", name)) continue;
						mAdditionalPaths.push_back(name);
					}
					else
					{
						std::string key, value;

						MyGUI::xml::ElementEnumerator properties = field->getElementEnumerator();
						while (properties.next("Property"))
						{
							if (!properties->findAttribute("key", key)) continue;
							if (!properties->findAttribute("value", value)) continue;

							setProperty(field->getName(), key, value, false);
						}
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

		for (MapSection::const_iterator section=mSections.begin(); section!=mSections.end(); ++section)
		{
			MyGUI::xml::Element* sectionNode = root->createChild((*section).first);

			const MapUString& properties = (*section).second;
			for (MapUString::const_iterator prop=properties.begin(); prop!=properties.end(); ++prop)
			{
				MyGUI::xml::Element* propertyNode = sectionNode->createChild("Property");
				propertyNode->addAttribute("key", (*prop).first);
				propertyNode->addAttribute("value", (*prop).second);
			}
		}

		// cleanup for duplicates
		std::reverse(mRecentFiles.begin(), mRecentFiles.end());
		for (size_t i = 0; i < mRecentFiles.size(); ++i)
			mRecentFiles.erase(std::remove(mRecentFiles.begin() + i + 1, mRecentFiles.end(), mRecentFiles[i]), mRecentFiles.end());

		// remove old files
		while (mRecentFiles.size() > MAX_RECENT_FILES)
			mRecentFiles.pop_back();
		std::reverse(mRecentFiles.begin(), mRecentFiles.end());

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

	/*bool SettingsManager::isNeedSolutionLoad(MyGUI::xml::ElementEnumerator _field)
	{
		MyGUI::xml::ElementEnumerator field = _field->getElementEnumerator();
		while (field.next())
		{
			std::string key, value;

			if (field->getName() == "Property")
			{
				if (!field->findAttribute("key", key)) continue;
				if (!field->findAttribute("value", value)) continue;

				if (key == "MetaSolutionName")
				{
					return !value.empty();
				}
			}
		}
		return false;
	}*/

	void SettingsManager::addRecentFile(const MyGUI::UString& _fileName)
	{
		mRecentFiles.push_back(_fileName);
	}

	void SettingsManager::setProperty(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName, const MyGUI::UString& _propertyValue)
	{
		setProperty(_sectionName, _propertyName, _propertyValue, true);
	}

	void SettingsManager::setProperty(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName, const MyGUI::UString& _propertyValue, bool _event)
	{
		MapSection::iterator sectionIter = mSections.find(_sectionName);
		if (sectionIter == mSections.end())
			sectionIter = mSections.insert(std::make_pair(_sectionName, MapUString())).first;

		MapUString& section = (*sectionIter).second;
		MapUString::iterator propertyIter = section.find(_propertyName);
		if (propertyIter == section.end())
			section.insert(std::make_pair(_propertyName, _propertyValue));
		else
			(*propertyIter).second = _propertyValue;

		eventSettingsChanged(_sectionName, _propertyName);
	}

	MyGUI::UString SettingsManager::getProperty(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName)
	{
		MapSection::const_iterator sectionIter = mSections.find(_sectionName);
		if (sectionIter != mSections.end())
		{
			const MapUString& section = (*sectionIter).second;
			MapUString::const_iterator propertyIter = section.find(_propertyName);
			if (propertyIter != section.end())
				return (*propertyIter).second;
		}

		return "";
	}

} // namespace tools
