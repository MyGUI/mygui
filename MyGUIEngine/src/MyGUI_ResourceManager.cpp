/*!
	@file
	@author		Albert Semenov
	@date		09/2008
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
#include "MyGUI_ResourceManager.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_ResourceImageSet.h"
#include "MyGUI_RenderManager.h"

namespace MyGUI
{

	const std::string XML_TYPE("Resource");
	const std::string XML_TYPE_LIST("List");
	const std::string ResourceManager::GUIResourceGroupName = "MyGUIDefault";

	MYGUI_INSTANCE_IMPLEMENT(ResourceManager);

	void ResourceManager::initialise(const std::string & _group)
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		// группа с которой работает весь гуй
		mResourceGroup = _group;

		registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &ResourceManager::_load);
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

	LoadXmlDelegate & ResourceManager::registerLoadXmlDelegate(const std::string & _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		MYGUI_ASSERT(iter == mMapLoadXmlDelegate.end(), "name delegate is exist");
		return (mMapLoadXmlDelegate[_key] = LoadXmlDelegate());
	}

	void ResourceManager::unregisterLoadXmlDelegate(const std::string & _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		if (iter != mMapLoadXmlDelegate.end()) mMapLoadXmlDelegate.erase(iter);
	}

	bool ResourceManager::_loadImplement(const std::string & _file, const std::string & _group, bool _match, const std::string & _type, const std::string & _instance)
	{
		std::string group = _group;
		if (_group == GUIResourceGroupName) group = getResourceGroup();
		xml::Document doc;
		if (false == doc.open(_file, group))
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
				if ((false == _match) || (type == _type)) (*iter).second(root, _file, version);
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
		else if (false == _match)
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

	IResourcePtr ResourceManager::getResource(const Guid & _id, bool _throw)
	{
		MapResource::iterator iter = mResources.find(_id);
		if (iter == mResources.end()) {
			if (_throw) MYGUI_EXCEPT("resource '" << _id.print() << "' not found");
			MYGUI_LOG(Warning, "resource '" << _id.print() << "' not found");
			return nullptr;
		}
		return iter->second;
	}

	/** Get resource by name */
	IResourcePtr ResourceManager::getResource(const std::string & _name, bool _throw)
	{
		MapResourceName::iterator iter = mResourceNames.find(_name);
		if (iter == mResourceNames.end()) {
			if (_throw) MYGUI_EXCEPT("resource '" << _name << "' not found");
			MYGUI_LOG(Warning, "resource '" << _name << "' not found");
			return nullptr;
		}
		return iter->second;
	}

	void ResourceManager::registerType(const std::string & _type, CreatorDelegate::IDelegate * _delegate)
	{
		MYGUI_ASSERT(mHolders.find(_type) == mHolders.end(), "dublicate resource type '" << _type << "'");
		mHolders[_type] = _delegate;
	}

	void ResourceManager::unregisterType(const std::string & _type)
	{
		MapDelegate::iterator iter = mHolders.find(_type);
		MYGUI_ASSERT(iter != mHolders.end(), "delegate resource type '" << _type << "' not found");
		mHolders.erase(iter);
	}

	bool ResourceManager::isFileExist(
		const std::string& _pattern,
		const std::string& _group,
		bool _unique,
		bool _fullmatch)
	{
		const VectorString& files = RenderManager::getInstance().getVectorResourcePath(_pattern, _group, false, _fullmatch);
		if ((_unique && files.size() == 1) || !files.empty()) return true;
		return false;
	}

	std::string ResourceManager::getResourcePath(
		const std::string& _pattern,
		const std::string& _group,
		bool _fullpath,
		bool _unique,
		bool _fullmatch)
	{
		const VectorString& files = RenderManager::getInstance().getVectorResourcePath(_pattern, _group, _fullpath, _fullmatch);
		if ((_unique && files.size() == 1) || !files.empty()) return files[0];
		return "";
	}

	const VectorString& ResourceManager::getVectorResourcePath(
		const std::string& _pattern,
		const std::string& _group,
		bool _fullpath,
		bool _fullmatch)
	{
		if (_group == GUIResourceGroupName) return RenderManager::getInstance().getVectorResourcePath(_pattern, mResourceGroup, _fullpath, _fullmatch);
		return RenderManager::getInstance().getVectorResourcePath(_pattern, _group, _fullpath, _fullmatch);
	}

} // namespace MyGUI
