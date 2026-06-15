/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	MYGUI_SINGLETON_DEFINITION(FactoryManager);

	FactoryManager::FactoryManager() :
		mSingletonHolder(this)
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

	void FactoryManager::registerFactory(
		std::string_view _category,
		std::string_view _type,
		Delegate::IDelegate* _delegate)
	{
		//FIXME
		auto category = mRegisterFactoryItems.find(_category);
		if (category == mRegisterFactoryItems.end())
			category = mRegisterFactoryItems.emplace(_category, MapFactoryItem()).first;
		auto type = category->second.find(_type);
		if (type == category->second.end())
			type = category->second.emplace(_type, Delegate()).first;
		type->second = _delegate;
	}

	void FactoryManager::unregisterFactory(std::string_view _category, std::string_view _type)
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

	void FactoryManager::unregisterFactory(std::string_view _category)
	{
		MapRegisterFactoryItem::iterator category = mRegisterFactoryItems.find(_category);
		if (category == mRegisterFactoryItems.end())
		{
			return;
		}
		mRegisterFactoryItems.erase(category);
	}

	IObject* FactoryManager::createObject(std::string_view _category, std::string_view _type)
	{
		MapRegisterFactoryItem::iterator category = mRegisterFactoryItems.find(_category);
		if (category == mRegisterFactoryItems.end())
		{
			return nullptr;
		}

		std::string_view typeName = BackwardCompatibility::getFactoryRename(_category, _type);
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

	bool FactoryManager::isFactoryExist(std::string_view _category, std::string_view _type)
	{
		MapRegisterFactoryItem::iterator category = mRegisterFactoryItems.find(_category);
		if (category == mRegisterFactoryItems.end())
		{
			return false;
		}
		return category->second.find(_type) != category->second.end();
	}

} // namespace MyGUI
