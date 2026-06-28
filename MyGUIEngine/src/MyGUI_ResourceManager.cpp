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

#include <pugixml.hpp>

namespace MyGUI
{

	MYGUI_SINGLETON_DEFINITION(ResourceManager);

	ResourceManager::ResourceManager() :
		mCategoryName("Resource"),
		mXmlListTagName("List"),
		mSingletonHolder(this)
	{
	}

	void ResourceManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		registerLoadXmlDelegate(mCategoryName) = newDelegate(this, &ResourceManager::loadFromXmlNode);
		registerLoadXmlDelegate(mXmlListTagName) = newDelegate(this, &ResourceManager::_loadList);

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
		return _loadImplement(_file, false, {}, getClassTypeName());
	}

	void ResourceManager::loadFromXmlNode(xml::ElementPtr _node, std::string_view /*unused*/, Version _version)
	{
		FactoryManager& factory = FactoryManager::getInstance();

		for (auto root : _node.node().children(mCategoryName.c_str()))
		{
			std::string_view type = root.attribute("type").value();
			std::string name{root.attribute("name").value()};

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
				MYGUI_LOG(Warning, "duplicate resource name '" << name << "'");

				// resources may be in use, keep for later cleanup
				mRemovedResources.push_back((*item).second);
				mResources.erase(item);
			}

			IResourcePtr resource = object->castType<IResource>();
			resource->deserialization(root, _version);

			mResources[name] = resource;
		}
	}

	void ResourceManager::_loadList(xml::ElementPtr _node, std::string_view /*unused*/, Version _version)
	{
		for (auto child : _node.node().children(mXmlListTagName.c_str()))
		{
			std::string source{child.attribute("file").value()};
			if (source.empty())
				continue;
			MYGUI_LOG(Info, "Load ini file '" << source << "'");
			_loadImplement(source, false, {}, getClassTypeName());
		}
	}

	ResourceManager::LoadXmlDelegate& ResourceManager::registerLoadXmlDelegate(std::string_view _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		MYGUI_ASSERT(iter == mMapLoadXmlDelegate.end(), "name delegate is exist");
		return mMapLoadXmlDelegate.emplace(_key, LoadXmlDelegate()).first->second;
	}

	void ResourceManager::unregisterLoadXmlDelegate(std::string_view _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		if (iter != mMapLoadXmlDelegate.end())
			mMapLoadXmlDelegate.erase(iter);
	}

	bool ResourceManager::_loadImplement(
		const std::string& _file,
		bool _match,
		std::string_view _type,
		std::string_view _instance)
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

		pugi::xml_node root = doc.getRoot();
		if ((!root) || (root.name() != std::string_view("MyGUI")))
		{
			MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'MyGUI' not found");
			return false;
		}

		std::string_view type = root.attribute("type").value();
		if (!type.empty())
		{
			Version version = Version::parse(root.attribute("version").value());
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
		// assume there will be nested elements
		else if (!_match)
		{
			for (auto node : root.children("MyGUI"))
			{
				type = node.attribute("type").value();
				if (!type.empty())
				{
					Version version = Version::parse(root.attribute("version").value());
					MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
					if (iter != mMapLoadXmlDelegate.end())
					{
						(*iter).second(node, _file, version);
					}
					else
					{
						MYGUI_LOG(
							Error,
							_instance << " : '" << _file << "', delegate for type '" << type << "'not found");
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

	bool ResourceManager::isExist(std::string_view _name) const
	{
		return mResources.find(_name) != mResources.end();
	}

	IResource* ResourceManager::findByName(std::string_view _name) const
	{
		MapResource::const_iterator item = mResources.find(_name);
		return (item == mResources.end()) ? nullptr : item->second;
	}

	IResource* ResourceManager::getByName(std::string_view _name, bool _throw) const
	{
		IResource* result = findByName(_name);
		MYGUI_ASSERT(result || !_throw, "Resource '" << _name << "' not found");
		return result;
	}

	bool ResourceManager::removeByName(std::string_view _name)
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
		for (auto& resource : mResources)
			delete resource.second;
		mResources.clear();

		for (auto& removedResource : mRemovedResources)
			delete removedResource;
		mRemovedResources.clear();
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
