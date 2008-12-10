/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "MyGUI_ResourceManager.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_ResourceImageSet.h"

namespace MyGUI
{

	const std::string XML_TYPE("Resource");
	const std::string XML_TYPE_LOCATION("Location");
	const std::string XML_TYPE_LIST("List");

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

	void ResourceManager::_load(xml::xmlNodePtr _node, const std::string & _file, Version _version)
	{
		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator root = _node->getNodeIterator();
		while (root.nextNode(XML_TYPE)) {
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

					IResourcePtr resource = null;
					iter->second(resource, root);

					mResources[guid] = resource;
					if (!name.empty()) mResourceNames[name] = resource;
				}
			}

		};
	}

	void ResourceManager::_loadLocation(xml::xmlNodePtr _node, const std::string & _file, Version _version)
	{
		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator root = _node->getNodeIterator();
		while (root.nextNode(XML_TYPE_LOCATION)) {
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
			if (group.empty()) group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

			helper::addResourceLocation(name, type, group, recursive, subdirs);

		};
	}

	void ResourceManager::_loadList(xml::xmlNodePtr _node, const std::string & _file, Version _version)
	{
		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator node = _node->getNodeIterator();
		while (node.nextNode(XML_TYPE_LIST)) {
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
		xml::xmlDocument doc;
		if (false == doc.open(_file, _group)) {
			MYGUI_LOG(Error, _instance << " : '" << _file << "', " << doc.getLastError());
			return false;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (null == root) || (root->getName() != "MyGUI") ) {
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
			xml::xmlNodeIterator node = root->getNodeIterator();
			while (node.nextNode("MyGUI")) {
				if (node->findAttribute("type", type)) {
					Version version = Version::parse(root->findAttribute("version"));
					MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
					if (iter != mMapLoadXmlDelegate.end()) {
						(*iter).second(node.currentNode(), _file, version);
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
