/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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

	MYGUI_SINGLETON_DEFINITION(SkinManager);

	SkinManager::SkinManager() :
		mXmlSkinTagName("Skin"),
		mXmlDefaultSkinValue("Default"),
		mSingletonHolder(this)
	{
	}

	void SkinManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		ResourceManager::getInstance().registerLoadXmlDelegate(mXmlSkinTagName) =
			newDelegate(this, &SkinManager::_load);

		const std::string& resourceCategory = ResourceManager::getInstance().getCategoryName();
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

		const std::string& resourceCategory = ResourceManager::getInstance().getCategoryName();
		FactoryManager::getInstance().unregisterFactory<ResourceSkin>(resourceCategory);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void SkinManager::_load(xml::ElementPtr _node, std::string_view _file, Version _version)
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		loadOldSkinFormat(_node, _file, _version, mXmlSkinTagName);
#endif // MYGUI_DONT_USE_OBSOLETE
	}

	void SkinManager::createDefault(std::string_view _value)
	{
		const std::string& resourceCategory = ResourceManager::getInstance().getCategoryName();
		ResourceSkin* skin = FactoryManager::getInstance().createObject<ResourceSkin>(resourceCategory);

		skin->setResourceName(_value);
		ResourceManager::getInstance().addResource(skin);
	}

	ResourceSkin* SkinManager::getByName(std::string_view _name) const
	{
		std::string_view skinName = BackwardCompatibility::getSkinRename(_name);
		IResource* result = nullptr;
		if (!skinName.empty() && skinName != mXmlDefaultSkinValue)
			result = ResourceManager::getInstance().getByName(skinName, false);

		if (result == nullptr)
		{
			result = ResourceManager::getInstance().getByName(mDefaultName, false);
			if (!skinName.empty() && skinName != mXmlDefaultSkinValue)
			{
				MYGUI_LOG(
					Error,
					"Skin '" << skinName << "' not found. Replaced with default skin."
							 << " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
			}
		}

		return result ? result->castType<ResourceSkin>(false) : nullptr;
	}

	bool SkinManager::isExist(std::string_view _name) const
	{
		std::string_view skinName = BackwardCompatibility::getSkinRename(_name);
		IResource* result = ResourceManager::getInstance().getByName(skinName, false);
		return (result != nullptr) && (result->isType<ResourceSkin>());
	}

	void SkinManager::setDefaultSkin(std::string_view _value)
	{
		mDefaultName = _value;
	}

	const std::string& SkinManager::getDefaultSkin() const
	{
		return mDefaultName;
	}

} // namespace MyGUI
