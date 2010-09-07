/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "precompiled.h"
#include "BackwardCompatibilityManager.h"
#include "SettingsManager.h"

template <> tools::BackwardCompatibilityManager* MyGUI::Singleton<tools::BackwardCompatibilityManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::BackwardCompatibilityManager>::mClassTypeName("BackwardCompatibilityManager");

namespace tools
{
	BackwardCompatibilityManager::BackwardCompatibilityManager()
	{
		mVersions.push_back("V3.2.0");
		mVersions.push_back("V3.1.0");

		mPropertyNames["V3.1.0-StaticImage-ImageTexture"] = "Image_Texture";
		mPropertyNames["V3.1.0-StaticImage-ImageRegion"] = "Image_Region";
		mPropertyNames["V3.1.0-StaticImage-ImageTile"] = "Image_Tile";
		mPropertyNames["V3.1.0-StaticImage-ImageIndex"] = "Image_Index";
		mPropertyNames["V3.1.0-StaticImage-ImageResource"] = "Image_Resource";
		mPropertyNames["V3.1.0-StaticImage-ImageGroup"] = "Image_Group";
		mPropertyNames["V3.1.0-StaticImage-ImageName"] = "Image_Name";
	}

	BackwardCompatibilityManager::~BackwardCompatibilityManager()
	{
	}

	void BackwardCompatibilityManager::initialise()
	{
		if (SettingsManager::getInstance().getSector("Settings")->getExistProperty("LayoutVersion"))
		{
			mCurrentVersion = SettingsManager::getInstance().getSector("Settings")->getPropertyValue("LayoutVersion");
		}
		else
		{
			mCurrentVersion = getDefaultVersion();
			SettingsManager::getInstance().getSector("Settings")->setPropertyValue("LayoutVersion", mCurrentVersion);
		}

		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &BackwardCompatibilityManager::notifySettingsChanged);
	}

	void BackwardCompatibilityManager::shutdown()
	{
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &BackwardCompatibilityManager::notifySettingsChanged);
	}

	void BackwardCompatibilityManager::serialiseProperty(MyGUI::xml::Element* _node, const std::string& _widgetType, const MyGUI::PairString& _property, bool _compatibility)
	{
		std::string propertyName = _property.first;

		if (_compatibility && getDefaultVersion() != getCurrentVersion())
		{
			std::string key = MyGUI::utility::toString(getCurrentVersion(), "-", _widgetType, "-", propertyName);

			MyGUI::MapString::const_iterator item = mPropertyNames.find(key);
			if (item != mPropertyNames.end())
			{
				propertyName = (*item).second;
			}
			else
			{
				MYGUI_LOG(Warning, "Property key not found '" << key << "'");
			}
		}

		MyGUI::xml::ElementPtr nodeProp = _node->createChild("Property");
		nodeProp->addAttribute("key", propertyName);
		nodeProp->addAttribute("value", _property.second);
	}

	const MyGUI::VectorString& BackwardCompatibilityManager::getVersions()
	{
		return mVersions;
	}

	void BackwardCompatibilityManager::notifySettingsChanged(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName)
	{
		if (_sectionName == "Settings")
		{
			if (_propertyName == "LayoutVersion")
				mCurrentVersion = SettingsManager::getInstance().getSector("Settings")->getPropertyValue("LayoutVersion");
		}
	}

	const std::string& BackwardCompatibilityManager::getDefaultVersion()
	{
		return mVersions.front();
	}

	const std::string& BackwardCompatibilityManager::getCurrentVersion()
	{
		return mCurrentVersion;
	}

} // namespace tools
