/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Resource.h"

namespace MyGUI
{

	const std::string XML_TYPE("Resource");

	INSTANCE_IMPLEMENT(ResourceManager);

	void ResourceManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &ResourceManager::_load);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void ResourceManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		clear();
		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool ResourceManager::load(const std::string & _file, const std::string & _group)
	{
		return Gui::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void ResourceManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator root = _node->getNodeIterator();
		while (root.nextNode(XML_TYPE)) {
			// парсим атрибуты
			std::string id, type;
			root->findAttribute("type", type);
			root->findAttribute("id", id);

			MapDelegate::iterator iter = mHolders.find(type);// == mHolders.end(), "dublicate resource type '" << _type << "'");
			if (iter == mHolders.end()) {
				MYGUI_LOG(Error, "resource type '" << type << "not found");
			}
			else {
				Guid guid(id);
				if (guid.empty()) {
					MYGUI_LOG(Error, "error load resource type '" << type << "' ,  id " << id);
				}
				else {
					MYGUI_ASSERT(mResources.find(guid) == mResources.end(), "dublicate resource id " << guid.print());
					ResourcePtr resource = null;
					iter->second(resource, root);
					mResources[guid] = resource;
				}
			}

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

} // namespace MyGUI	
