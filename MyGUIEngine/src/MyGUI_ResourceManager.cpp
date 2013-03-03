/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_DataStreamHolder.h"
#include "MyGUI_ResourceImageSet.h"

namespace MyGUI
{

	template <> ResourceManager* Singleton<ResourceManager>::msInstance = nullptr;
	template <> const char* Singleton<ResourceManager>::mClassTypeName = "ResourceManager";

	ResourceManager::ResourceManager() :
		mIsInitialise(false),
		mCategoryName("Resource"),
		mXmlListTagName("List")
	{
	}

	void ResourceManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		registerLoadXmlDelegate(mCategoryName) = newDelegate(this, &ResourceManager::loadFromXmlNode);
		registerLoadXmlDelegate(mXmlListTagName) = newDelegate(this, &ResourceManager::_loadList);

		// регестрируем дефолтные ресурсы
		FactoryManager::getInstance().registerFactory<ResourceImageSet>(mCategoryName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void ResourceManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		FactoryManager::getInstance().unregisterFactory<ResourceImageSet>(mCategoryName);

		clear();
		unregisterLoadXmlDelegate(mCategoryName);
		unregisterLoadXmlDelegate(mXmlListTagName);

		mMapLoadXmlDelegate.clear();

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	bool ResourceManager::load(const std::string& _file)
	{
		return _loadImplement(_file, false, "", getClassTypeName());
	}

	void ResourceManager::loadFromXmlNode(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		FactoryManager& factory = FactoryManager::getInstance();

		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator root = _node->getElementEnumerator();
		while (root.next(mCategoryName))
		{
			// парсим атрибуты
			std::string type, name;
			root->findAttribute("type", type);
			root->findAttribute("name", name);

			if (name.empty())
				continue;

			IObject* object = factory.createObject(mCategoryName, type);
			if (object == nullptr)
			{
				MYGUI_LOG(Error, "resource type '" << type << "' not found");
				continue;
			}

			MapResource::iterator item = mResources.find(name);
			if (item != mResources.end())
			{
				MYGUI_LOG(Warning, "dublicate resource name '" << name << "'");

				// ресурсами могут пользоваться
				mRemovedResoures.push_back((*item).second);
				mResources.erase(item);
			}

			IResourcePtr resource = object->castType<IResource>();
			resource->deserialization(root.current(), _version);

			mResources[name] = resource;
		}
	}

	void ResourceManager::_loadList(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next(mXmlListTagName))
		{
			std::string source;
			if (!node->findAttribute("file", source)) continue;
			MYGUI_LOG(Info, "Load ini file '" << source << "'");
			_loadImplement(source, false, "", getClassTypeName());
		}
	}

	ResourceManager::LoadXmlDelegate& ResourceManager::registerLoadXmlDelegate(const std::string& _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		MYGUI_ASSERT(iter == mMapLoadXmlDelegate.end(), "name delegate is exist");
		return (mMapLoadXmlDelegate[_key] = LoadXmlDelegate());
	}

	void ResourceManager::unregisterLoadXmlDelegate(const std::string& _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		if (iter != mMapLoadXmlDelegate.end()) mMapLoadXmlDelegate.erase(iter);
	}

	bool ResourceManager::_loadImplement(const std::string& _file, bool _match, const std::string& _type, const std::string& _instance)
	{
		DataStreamHolder data = DataManager::getInstance().getData(_file);
		if (data.getData() == nullptr)
		{
			MYGUI_LOG(Error, _instance << " : '" << _file << "', not found");
			return false;
		}

		xml::Document doc;
		if (!doc.open(data.getData()))
		{
			MYGUI_LOG(Error, _instance << " : '" << _file << "', " << doc.getLastError());
			return false;
		}

		xml::ElementPtr root = doc.getRoot();
		if ( (nullptr == root) || (root->getName() != "MyGUI") )
		{
			MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'MyGUI' not found");
			return false;
		}

		std::string type;
		if (root->findAttribute("type", type))
		{
			Version version = Version::parse(root->findAttribute("version"));
			MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
			if (iter != mMapLoadXmlDelegate.end())
			{
				if ((!_match) || (type == _type))
					(*iter).second(root, _file, version);
				else
				{
					MYGUI_LOG(Error, _instance << " : '" << _file << "', type '" << _type << "' not found");
					return false;
				}
			}
			else
			{
				MYGUI_LOG(Error, _instance << " : '" << _file << "', delegate for type '" << type << "'not found");
				return false;
			}
		}
		// предпологаем что будут вложенные
		else if (!_match)
		{
			xml::ElementEnumerator node = root->getElementEnumerator();
			while (node.next("MyGUI"))
			{
				if (node->findAttribute("type", type))
				{
					Version version = Version::parse(root->findAttribute("version"));
					MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
					if (iter != mMapLoadXmlDelegate.end())
					{
						(*iter).second(node.current(), _file, version);
					}
					else
					{
						MYGUI_LOG(Error, _instance << " : '" << _file << "', delegate for type '" << type << "'not found");
					}
				}
				else
				{
					MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'type' not found");
				}
			}
		}

		return true;
	}

	void ResourceManager::addResource(IResourcePtr _item)
	{
		if (!_item->getResourceName().empty())
			mResources[_item->getResourceName()] = _item;
	}

	void ResourceManager::removeResource(IResourcePtr _item)
	{
		if (_item == nullptr)
			return;

		if (!_item->getResourceName().empty())
		{
			MapResource::iterator item = mResources.find(_item->getResourceName());
			if (item != mResources.end())
				mResources.erase(item);
		}
	}

	bool ResourceManager::isExist(const std::string& _name) const
	{
		return mResources.find(_name) != mResources.end();
	}

	IResource* ResourceManager::findByName(const std::string& _name) const
	{
		MapResource::const_iterator item = mResources.find(_name);
		return (item == mResources.end()) ? nullptr : item->second;
	}

	IResource* ResourceManager::getByName(const std::string& _name, bool _throw) const
	{
		IResource* result = findByName(_name);
		MYGUI_ASSERT(result || !_throw, "Resource '" << _name << "' not found");
		return result;
	}

	bool ResourceManager::removeByName(const std::string& _name)
	{
		MapResource::const_iterator item = mResources.find(_name);
		if (item != mResources.end())
		{
			delete item->second;
			mResources.erase(item->first);
			return true;
		}
		return false;
	}

	void ResourceManager::clear()
	{
		for (MapResource::iterator item = mResources.begin(); item != mResources.end(); ++ item)
			delete item->second;
		mResources.clear();

		for (VectorResource::iterator item = mRemovedResoures.begin(); item != mRemovedResoures.end(); ++ item)
			delete (*item);
		mRemovedResoures.clear();
	}

	ResourceManager::EnumeratorPtr ResourceManager::getEnumerator() const
	{
		return EnumeratorPtr(mResources);
	}

	size_t ResourceManager::getCount() const
	{
		return mResources.size();
	}

	const std::string& ResourceManager::getCategoryName() const
	{
		return mCategoryName;
	}

} // namespace MyGUI
