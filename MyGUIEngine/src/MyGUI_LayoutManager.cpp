/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	template <> LayoutManager* Singleton<LayoutManager>::msInstance = nullptr;
	template <> const char* Singleton<LayoutManager>::mClassTypeName = "LayoutManager";

	LayoutManager::LayoutManager() :
		mIsInitialise(false),
		mXmlLayoutTagName("Layout")
	{
	}

	void LayoutManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		ResourceManager::getInstance().registerLoadXmlDelegate(mXmlLayoutTagName) = newDelegate(this, &LayoutManager::_load);

		std::string resourceCategory = ResourceManager::getInstance().getCategoryName();
		FactoryManager::getInstance().registerFactory<ResourceLayout>(resourceCategory);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void LayoutManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		ResourceManager::getInstance().unregisterLoadXmlDelegate(mXmlLayoutTagName);

		std::string resourceCategory = ResourceManager::getInstance().getCategoryName();
		FactoryManager::getInstance().unregisterFactory<ResourceLayout>(resourceCategory);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void LayoutManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		ResourceLayout* resource = new ResourceLayout(_node, _file);
		ResourceManager::getInstance().addResource(resource);
	}

	VectorWidgetPtr LayoutManager::loadLayout(const std::string& _file, const std::string& _prefix, Widget* _parent)
	{
		mCurrentLayoutName = _file;

		ResourceLayout* resource = getByName(_file, false);
		if (!resource)
		{
			ResourceManager::getInstance().load(_file);
			resource = getByName(_file, false);
		}

		VectorWidgetPtr result;
		if (resource)
			result = resource->createLayout(_prefix, _parent);
		else
			MYGUI_LOG(Warning, "Layout '" << _file << "' couldn't be loaded");

		mCurrentLayoutName = "";

		return result;
	}

	void LayoutManager::unloadLayout(VectorWidgetPtr& _widgets)
	{
		WidgetManager::getInstance().destroyWidgets(_widgets);
	}

	ResourceLayout* LayoutManager::getByName(const std::string& _name, bool _throw) const
	{
		std::string skinName = BackwardCompatibility::getSkinRename(_name);
		IResource* result = ResourceManager::getInstance().getByName(skinName, false);

		if (result != nullptr)
		{
			ResourceLayout* resource = result->castType<ResourceLayout>(false);
			if (resource == nullptr)
			{
				MYGUI_ASSERT(!_throw, "Resource '" << skinName << "' is not ResourceLayout type");
			}
			return resource;
		}

		MYGUI_ASSERT(!_throw, "ResourceLayout '" << skinName << "' not found");
		return nullptr;
	}

	const std::string& LayoutManager::getCurrentLayout() const
	{
		return mCurrentLayoutName;
	}

	bool LayoutManager::isExist(const std::string& _name) const
	{
		return getByName(_name, false) != nullptr;
	}

} // namespace MyGUI
