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

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
namespace MyGUI
{
	std::string macBundlePath()
	{
		char path[1024];
		CFBundleRef mainBundle = CFBundleGetMainBundle();    assert(mainBundle);
		CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);    assert(mainBundleURL);
		CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);    assert(cfStringRef);
		CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);
		CFRelease(mainBundleURL);
		CFRelease(cfStringRef);
		return std::string(path);
	}
}
#endif

namespace MyGUI
{

	const std::string XML_TYPE("Resource");
	const std::string XML_TYPE_LOCATION("Location");
	const std::string XML_TYPE_LIST("List");

	INSTANCE_IMPLEMENT(ResourceManager);

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

	void ResourceManager::_load(xml::xmlNodePtr _node, const std::string & _file)
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

	void ResourceManager::_loadLocation(xml::xmlNodePtr _node, const std::string & _file)
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
			if (name.empty()) {
				MYGUI_LOG(Error, "error load resource location, tag 'name' is empty");
				continue;
			}
			if (type.empty()) type = "FileSystem";
			if (group.empty()) group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

			// ресурс менеджеры огра, не держут рекурсию, добавляем все пути ручками
			if (recursive) {
				Ogre::Archive* pArch = Ogre::ArchiveManager::getSingleton().load( name, type );
				Ogre::StringVectorPtr vec = pArch->find("*", true, true);
				for (size_t pos=0; pos<vec->size(); ++pos) {
					std::string new_filename = name + '/' + vec->at(pos);
					#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
						// OS X does not set the working directory relative to the app, In order to make things portable on OS X we need to provide the loading with it's own bundle path location
						Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Ogre::String(macBundlePath() + "/" + new_filename), type, group, false);
					#else
						Ogre::ResourceGroupManager::getSingleton().addResourceLocation(new_filename, type, group, false);
					#endif
				}
				vec.setNull();
				Ogre::ArchiveManager::getSingleton().unload(pArch);
			}

			#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
				// OS X does not set the working directory relative to the app, In order to make things portable on OS X we need to provide the loading with it's own bundle path location
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Ogre::String(macBundlePath() + "/" + name), type, group, false);
			#else
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, type, group, false);
			#endif

		};
	}

	void ResourceManager::_loadList(xml::xmlNodePtr _node, const std::string & _file)
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
		std::string file(_group.empty() ? _file : helper::getResourcePath(_file, _group));
		if (file.empty()) {
			MYGUI_LOG(Error, _instance << " : file '" << _file << "' not found");
			return false;
		}

		Ogre::DataStreamPtr fileStream;
		if (!_group.empty()) fileStream = Ogre::ResourceGroupManager::getSingletonPtr()->openResource(_file,_group);

		if ((_group.empty() && (false == doc.open(file))) || (!_group.empty() && (false == doc.open(fileStream)))) {
			MYGUI_LOG(Error, _instance << " : " << doc.getLastError());
			return false;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (null == root) || (root->getName() != "MyGUI") ) {
			MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'MyGUI' not found");
			return false;
		}

		std::string type;
		if (root->findAttribute("type", type)) {
			MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
			if (iter != mMapLoadXmlDelegate.end()) {
				if ((false == _match) || (type == _type)) (*iter).second(root, file);
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
					MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
					if (iter != mMapLoadXmlDelegate.end()) {
						(*iter).second(node.currentNode(), file);
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
