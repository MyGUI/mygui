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
#include "MyGUI_Enumerator.h"
#include "MyGUI_Guid.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{
	class IResource;
	typedef IResource * IResourcePtr;

	typedef delegates::CDelegate2<IResourcePtr &, xml::xmlNodeIterator> CreatorDelegate;
	typedef delegates::IDelegate2<IResourcePtr &, xml::xmlNodeIterator> * CreatorDelegatePtr;

	typedef delegates::CDelegate2<xml::xmlNodePtr, const std::string &> LoadXmlDelegate;
	typedef std::map<Ogre::String, LoadXmlDelegate> MapLoadXmlDelegate;

	typedef std::map<std::string, CreatorDelegate> MapDelegate;
	typedef std::map<Guid, IResourcePtr> MapResource;
	typedef std::map<std::string, IResourcePtr> MapResourceName;
	typedef Enumerator<MapResource> EnumeratorMapResource;

	class _MyGUIExport ResourceManager
	{
		INSTANCE_HEADER(ResourceManager);

	public:
		void initialise(const Ogre::String & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void shutdown();

	public:
		/** Register delegate for parsing XML blocks */
		LoadXmlDelegate & registerLoadXmlDelegate(const Ogre::String & _key);
		void unregisterLoadXmlDelegate(const Ogre::String & _key);

		/** Load config with any info (file can have different data such other config files that will be loaded, skins, layers, pointers, etc) */
		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		bool _loadImplement(const std::string & _file, const std::string & _group, bool _match, const std::string & _type, const std::string & _instance);
		void _load(xml::xmlNodePtr _node, const std::string & _file);
		void _loadLocation(xml::xmlNodePtr _node, const std::string & _file);
		void _loadList(xml::xmlNodePtr _node, const std::string & _file);

		/** Get name of ResourceGroup*/
		const std::string& getResourceGroup() { return mResourceGroup; }

		IResourcePtr getResource(const Guid & _id, bool _throw = true)
		{
			MapResource::iterator iter = mResources.find(_id);
			if (iter == mResources.end()) {
				if (_throw) MYGUI_EXCEPT("resource '" << _id.print() << "' not found");
				MYGUI_LOG(Warning, "resource '" << _id.print() << "' not found");
				return null;
			}
			return iter->second;
		}

		IResourcePtr getResource(const std::string & _name, bool _throw = true)
		{
			MapResourceName::iterator iter = mResourceNames.find(_name);
			if (iter == mResourceNames.end()) {
				if (_throw) MYGUI_EXCEPT("resource '" << _name << "' not found");
				MYGUI_LOG(Warning, "resource '" << _name << "' not found");
				return null;
			}
			return iter->second;
		}

		template <typename T>
		std::vector<T*> getResources()
		{
			std::vector<T*> ret;
			for (MapResource::const_iterator iter=mResources.begin(); iter!=mResources.end(); ++iter) {
				T* resource = iter->second->castType<T>(false);
				if (resource != null) ret.push_back(resource);
			}
			return ret;
		}

		void registerType(const std::string & _type, CreatorDelegatePtr _delegate)
		{
			MYGUI_ASSERT(mHolders.find(_type) == mHolders.end(), "dublicate resource type '" << _type << "'");
			mHolders[_type] = _delegate;
		}

		void unregisterType(const std::string & _type)
		{
			MapDelegate::iterator iter = mHolders.find(_type);
			MYGUI_ASSERT(iter != mHolders.end(), "delegate resource type '" << _type << "' not found");
			mHolders.erase(iter);
		}

		void clear();

		size_t getResourceCount() { return mResources.size(); }
		bool isExist(const Guid & _id) { return mResources.find(_id) != mResources.end(); }
		bool isExist(const std::string & _name) { return mResourceNames.find(_name) != mResourceNames.end(); }
		EnumeratorMapResource getEnumerator() { return EnumeratorMapResource(mResources); }

	private:

		MapDelegate mHolders;
		MapResource mResources;
		MapResourceName mResourceNames;

		// ����� � ���������� ��� �������� ��� ������
		MapLoadXmlDelegate mMapLoadXmlDelegate;

		std::string mResourceGroup;
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_MANAGER_H__
