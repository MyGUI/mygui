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
#include "MyGUI_LayoutManager.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	const std::string XML_TYPE("Layout");
	const std::string XML_TYPE_RESOURCE("Resource");

	template <> LayoutManager* Singleton<LayoutManager>::msInstance = nullptr;
	template <> const char* Singleton<LayoutManager>::mClassTypeName("LayoutManager");

	LayoutManager::LayoutManager() :
		mIsInitialise(false)
	{
	}

	void LayoutManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LayoutManager::_load);
		FactoryManager::getInstance().registerFactory<ResourceLayout>(XML_TYPE_RESOURCE);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void LayoutManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);
		FactoryManager::getInstance().unregisterFactory<ResourceLayout>(XML_TYPE_RESOURCE);

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
