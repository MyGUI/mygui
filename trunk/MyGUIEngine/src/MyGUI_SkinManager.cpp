/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_SubWidgetManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_IStateInfo.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	template <> SkinManager* Singleton<SkinManager>::msInstance = nullptr;
	template <> const char* Singleton<SkinManager>::mClassTypeName = "SkinManager";

	SkinManager::SkinManager() :
		mIsInitialise(false),
		mXmlSkinTagName("Skin"),
		mXmlDefaultSkinValue("Default")
	{
	}

	void SkinManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		ResourceManager::getInstance().registerLoadXmlDelegate(mXmlSkinTagName) = newDelegate(this, &SkinManager::_load);

		std::string resourceCategory = ResourceManager::getInstance().getCategoryName();
		FactoryManager::getInstance().registerFactory<ResourceSkin>(resourceCategory);

		mDefaultName = "skin_Default";
		createDefault(mDefaultName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void SkinManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		ResourceManager::getInstance().unregisterLoadXmlDelegate(mXmlSkinTagName);

		std::string resourceCategory = ResourceManager::getInstance().getCategoryName();
		FactoryManager::getInstance().unregisterFactory<ResourceSkin>(resourceCategory);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void SkinManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		loadOldSkinFormat(_node, _file, _version, mXmlSkinTagName);
#endif // MYGUI_DONT_USE_OBSOLETE
	}

	void SkinManager::createDefault(const std::string& _value)
	{
		std::string resourceCategory = ResourceManager::getInstance().getCategoryName();
		ResourceSkin* skin = FactoryManager::getInstance().createObject<ResourceSkin>(resourceCategory);

		skin->setResourceName(_value);
		ResourceManager::getInstance().addResource(skin);
	}

	ResourceSkin* SkinManager::getByName(const std::string& _name) const
	{
		std::string skinName = BackwardCompatibility::getSkinRename(_name);
		IResource* result = nullptr;
		if (!skinName.empty() && skinName != mXmlDefaultSkinValue)
			result = ResourceManager::getInstance().getByName(skinName, false);

		if (result == nullptr)
		{
			result = ResourceManager::getInstance().getByName(mDefaultName, false);
			if (!skinName.empty() && skinName != mXmlDefaultSkinValue)
			{
				MYGUI_LOG(Error, "Skin '" << skinName << "' not found. Replaced with default skin." << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
		}

		return result ? result->castType<ResourceSkin>(false) : nullptr;
	}

	bool SkinManager::isExist(const std::string& _name) const
	{
		std::string skinName = BackwardCompatibility::getSkinRename(_name);
		IResource* result = ResourceManager::getInstance().getByName(skinName, false);
		return (result != nullptr) && (result->isType<ResourceSkin>());
	}

	void SkinManager::setDefaultSkin(const std::string& _value)
	{
		mDefaultName = _value;
	}

	const std::string SkinManager::getDefaultSkin() const
	{
		return mDefaultName;
	}

} // namespace MyGUI
