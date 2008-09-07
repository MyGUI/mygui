/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_RESOURCE_MANAGER_H__
#define __MYGUI_RESOURCE_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_Guid.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{
	class Resource;
	typedef Resource * ResourcePtr;

	typedef delegates::CDelegate2<ResourcePtr &, xml::xmlNodeIterator> CreatorDelegate;
	typedef delegates::IDelegate2<ResourcePtr &, xml::xmlNodeIterator> * CreatorDelegatePtr;

	class _MyGUIExport ResourceManager
	{
		INSTANCE_HEADER(ResourceManager);

	public:
		void initialise();
		void shutdown();

	public:

		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

		ResourcePtr getResource(const Guid & _id, bool _throw = true)
		{
			MapResource::iterator iter = mResources.find(_id);
			if (iter == mResources.end()) {
				if (_throw) MYGUI_EXCEPT("resource '" << _id.print() << "' not found");
				MYGUI_LOG(Warning, "resource '" << _id.print() << "' not found");
				return null;
			}
			return iter->second;
		}

		inline void registerType(const std::string & _type, CreatorDelegatePtr _delegate)
		{
			MYGUI_ASSERT(mHolders.find(_type) == mHolders.end(), "dublicate resource type '" << _type << "'");
			mHolders[_type] = _delegate;
		}

		inline void unregisterType(const std::string & _type)
		{
			MapDelegate::iterator iter = mHolders.find(_type);
			MYGUI_ASSERT(iter != mHolders.end(), "delegate resource type '" << _type << "' not found");
			mHolders.erase(iter);
		}

		void clear();

	private:
		typedef std::map<std::string, CreatorDelegate> MapDelegate;
		typedef std::map<Guid, ResourcePtr> MapResource;

		MapDelegate mHolders;
		MapResource mResources;
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_MANAGER_H__
