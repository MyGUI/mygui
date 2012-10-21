/*!
	@file
	@author		Albert Semenov
	@date		06/2009
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
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	template <> FactoryManager* Singleton<FactoryManager>::msInstance = nullptr;
	template <> const char* Singleton<FactoryManager>::mClassTypeName = "FactoryManager";

	FactoryManager::FactoryManager() :
		mIsInitialise(false)
	{
	}

	void FactoryManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void FactoryManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void FactoryManager::registerFactory(const std::string& _category, const std::string& _type, Delegate::IDelegate* _delegate)
	{
		//FIXME
		mRegisterFactoryItems[_category][_type] = _delegate;
	}

	void FactoryManager::unregisterFactory(const std::string& _category, const std::string& _type)
	{
		MapRegisterFactoryItem::iterator category = mRegisterFactoryItems.find(_category);
		if (category == mRegisterFactoryItems.end())
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

	void FactoryManager::unregisterFactory(const std::string& _category)
	{
		MapRegisterFactoryItem::iterator category = mRegisterFactoryItems.find(_category);
		if (category == mRegisterFactoryItems.end())
		{
			return;
		}
		mRegisterFactoryItems.erase(category);
	}

	IObject* FactoryManager::createObject(const std::string& _category, const std::string& _type)
	{
		MapRegisterFactoryItem::iterator category = mRegisterFactoryItems.find(_category);
		if (category == mRegisterFactoryItems.end())
		{
			return nullptr;
		}

		std::string typeName = BackwardCompatibility::getFactoryRename(_category, _type);
		MapFactoryItem::iterator type = category->second.find(typeName);
		if (type == category->second.end())
		{
			return nullptr;
		}
		if (type->second.empty())
		{
			return nullptr;
		}

		IObject* result = nullptr;
		type->second(result);
		return result;
	}

	void FactoryManager::destroyObject(IObject* _object)
	{
		delete _object;

		/*MapRegisterFactoryItem::iterator category = mRegisterFactoryItems.find(_category);
		if (category == mRegisterFactoryItems.end())
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

	bool FactoryManager::isFactoryExist(const std::string& _category, const std::string& _type)
	{
		MapRegisterFactoryItem::iterator category = mRegisterFactoryItems.find(_category);
		if (category == mRegisterFactoryItems.end())
		{
			return false;
		}
		MapFactoryItem::iterator type = category->second.find(_type);
		if (type == category->second.end())
		{
			return false;
		}

		return true;
	}

} // namespace MyGUI
