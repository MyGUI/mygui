/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*//*
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
#include "MyGUI_ResourceManager.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_ResourceImageSet.h"

namespace MyGUI
{

	const std::string XML_TYPE("Resource");
	const std::string XML_TYPE_LOCATION("Location");
	const std::string XML_TYPE_LIST("List");
	const std::string ResourceManager::GUIResourceGroupName = "MyGUIDefault";

	MYGUI_INSTANCE_IMPLEMENT(ResourceManager);

	void ResourceManager::initialise(const Ogre::String & _group)
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		// группа с которой работает весь гуй
		mResourceGroup = _group;

		registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &ResourceManager::_load);
		registerLoadXmlDelegate(XML_TYPE_LOCATION) = newDelegate(this, &ResourceManager::_loadLocation);
		registerLoadXmlDelegate(XML_TYPE_LIST) = newDelegate(this, &ResourceManager::_loadList);

		// регестрируем дефолтные ресурсы
		ResourceImageSet::registryType();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void ResourceManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		ResourceImageSet::unregistryType();

		clear();
		unregisterLoadXmlDelegate(XML_TYPE);
		unregisterLoadXmlDelegate(XML_TYPE_LOCATION);
		unregisterLoadXmlDelegate(XML_TYPE_LIST);

		mMapLoadXmlDelegate.clear();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool ResourceManager::load(const std::string & _file, const std::string & _group)
	{
		return _loadImplement(_file, _group, false, "", INSTANCE_TYPE_NAME);
	}

	void ResourceManager::_load(xml::ElementPtr _node, const std::string & _file, Version _version)
	{
		VectorGuid vector_guid;
		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator root = _node->getElementEnumerator();
		while (root.next(XML_TYPE)) {
			// парсим атрибуты
			std::string id, type, name;
			root->findAttribute("type", type);
			root->findAttribute("name", name);
			root->findAttribute("id", id);

			MapDelegate::iterator iter = mHolders.find(type);
			if (iter == mHolders.end()) {
				MYGUI_LOG(Error, "resource type '" << type << "' not found");
			}
			else {
				Guid guid(id);
				if (guid.empty()) {
					MYGUI_LOG(Error, "error load resource type '" << type << "' ,  id " << id << " ,  name '" << name << "'");
				}
				else {
					MYGUI_ASSERT(mResources.find(guid) == mResources.end(), "dublicate resource id " << guid.print());
					MYGUI_ASSERT(mResourceNames.find(name) == mResourceNames.end(), "dublicate resource name '" << name << "'");

					vector_guid.push_back(guid);

					IResourcePtr resource = nullptr;
					iter->second(resource, root, _version);

					mResources[guid] = resource;
					if (!name.empty()) mResourceNames[name] = resource;
				}
			}

		};

		if (!vector_guid.empty())
		{
			mListFileGuid[_file] = vector_guid;
		}

	}

	std::string ResourceManager::getFileNameByID(const Guid& _id)
	{
		for (MapVectorString::iterator item=mListFileGuid.begin(); item!=mListFileGuid.end(); ++item)
		{
			for (VectorGuid::iterator item2=item->second.begin(); item2!=item->second.end(); ++item2)
			{
				if (*item2 == _id)
				{
					return item->first;
				}
			}
		}
		return "";
	}

	void ResourceManager::_loadLocation(xml::ElementPtr _node, const std::string & _file, Version _version)
	{
		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator root = _node->getElementEnumerator();
		while (root.next(XML_TYPE_LOCATION)) {
			// парсим атрибуты
			std::string name, type, group;
			root->findAttribute("name", name);
			root->findAttribute("type", type);
			root->findAttribute("group", group);
			bool recursive= utility::parseBool(root->findAttribute("recursive"));
			bool subdirs = utility::parseBool(root->findAttribute("subdirs"));
			if (name.empty()) {
				MYGUI_LOG(Error, "error load resource location, tag 'name' is empty");
				continue;
			}
			if (type.empty()) type = "FileSystem";
			if (group.empty()) group = getResourceGroup();

			helper::addResourceLocation(name, type, group, recursive, subdirs);

		};
	}

	void ResourceManager::_loadList(xml::ElementPtr _node, const std::string & _file, Version _version)
	{
		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next(XML_TYPE_LIST)) {
			std::string source;
			if (false == node->findAttribute("file", source)) continue;
			std::string group = node->findAttribute("group");
			MYGUI_LOG(Info, "Load ini file '" << source << "' from " << (group.empty() ? "current path" : "resource group : ") << group);
			_loadImplement(source, group, false, "", INSTANCE_TYPE_NAME);
		};
	}

	void ResourceManager::clear()
	{
		for (MapResource::iterator iter=mResources.begin(); iter!=mResources.end(); ++iter) {
			delete (*iter).second;
		}
		mResources.clear();
		mHolders.clear();
	}

	LoadXmlDelegate & ResourceManager::registerLoadXmlDelegate(const Ogre::String & _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		MYGUI_ASSERT(iter == mMapLoadXmlDelegate.end(), "name delegate is exist");
		return (mMapLoadXmlDelegate[_key] = LoadXmlDelegate());
	}

	void ResourceManager::unregisterLoadXmlDelegate(const Ogre::String & _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		if (iter != mMapLoadXmlDelegate.end()) mMapLoadXmlDelegate.erase(iter);
	}

	bool ResourceManager::_loadImplement(const std::string & _file, const std::string & _group, bool _match, const std::string & _type, const std::string & _instance)
	{
		std::string group = _group;
		if (_group == GUIResourceGroupName) group = getResourceGroup();
		xml::Document doc;
		if (false == doc.open(_file, group)) {
			MYGUI_LOG(Error, _instance << " : '" << _file << "', " << doc.getLastError());
			return false;
		}

		xml::ElementPtr root = doc.getRoot();
		if ( (nullptr == root) || (root->getName() != "MyGUI") ) {
			MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'MyGUI' not found");
			return false;
		}

		std::string type;
		if (root->findAttribute("type", type)) {
			Version version = Version::parse(root->findAttribute("version"));
			MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
			if (iter != mMapLoadXmlDelegate.end()) {
				if ((false == _match) || (type == _type)) (*iter).second(root, _file, version);
				else {
					MYGUI_LOG(Error, _instance << " : '" << _file << "', type '" << _type << "' not found");
					return false;
				}
			}
			else {
				MYGUI_LOG(Error, _instance << " : '" << _file << "', delegate for type '" << type << "'not found");
				return false;
			}
		}
		// предпологаем что будут вложенные
		else if (false == _match) {
			xml::ElementEnumerator node = root->getElementEnumerator();
			while (node.next("MyGUI")) {
				if (node->findAttribute("type", type)) {
					Version version = Version::parse(root->findAttribute("version"));
					MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
					if (iter != mMapLoadXmlDelegate.end()) {
						(*iter).second(node.current(), _file, version);
					}
					else {
						MYGUI_LOG(Error, _instance << " : '" << _file << "', delegate for type '" << type << "'not found");
					}
				}
				else {
					MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'type' not found");
				}
			}
		}

		return true;
	}

} // namespace MyGUI
