/*!
	@file
	@author		Albert Semenov
	@date		06/2009
	@module
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
#include "MyGUI_FactoryManager.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT( FactoryManager );

	void FactoryManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);


		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void FactoryManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void FactoryManager::registryFactory(const std::string& _category, const std::string& _type, Delegate::IDelegate* _delegate)
	{
		//FIXME
		mRegistryFactoryItems[_category][_type] = _delegate;
	}

	void FactoryManager::unregistryFactory(const std::string& _category, const std::string& _type)
	{
		MapRegistryFactoryItem::iterator category = mRegistryFactoryItems.find(_category);
		if (category == mRegistryFactoryItems.end())
		{
			return;
		}
		MapFactoryItem::iterator type = category->second.find(_type);
		if (type == category->second.end())
		{
			return;
		}

		category->second.erase(type);
	}

	Object* FactoryManager::createObject(const std::string& _category, const std::string& _type)
	{
		MapRegistryFactoryItem::iterator category = mRegistryFactoryItems.find(_category);
		if (category == mRegistryFactoryItems.end())
		{
			return nullptr;
		}
		MapFactoryItem::iterator type = category->second.find(_type);
		if (type == category->second.end())
		{
			return nullptr;
		}
		if (type->second.empty())
		{
			return nullptr;
		}

		Object* result = nullptr;
		type->second(result);
		return result;
	}

	void FactoryManager::destroyObject(Object* _object)
	{
		delete _object;

		/*MapRegistryFactoryItem::iterator category = mRegistryFactoryItems.find(_category);
		if (category == mRegistryFactoryItems.end())
		{
			return;
		}
		MapFactoryItem::iterator type = category->second.find(_type);
		if (type == category->second.end())
		{
			return;
		}
		if (type->second.empty())
		{
			return;
		}

		type->second(_object, nullptr, _version);*/
	}

} // namespace MyGUI
