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
#ifndef __MYGUI_RESOURCE_MANAGER_H__
#define __MYGUI_RESOURCE_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_Guid.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"

namespace MyGUI
{
	typedef delegates::CDelegate3<IResourcePtr &, xml::ElementEnumerator, Version> CreatorDelegate;

	typedef delegates::CDelegate3<xml::ElementPtr, const std::string &, Version> LoadXmlDelegate;
	typedef std::map<Ogre::String, LoadXmlDelegate> MapLoadXmlDelegate;

	typedef std::map<std::string, CreatorDelegate> MapDelegate;
	typedef std::map<Guid, IResourcePtr> MapResource;
	typedef std::map<std::string, IResourcePtr> MapResourceName;
	typedef Enumerator<MapResource> EnumeratorMapResource;

	class MYGUI_EXPORT ResourceManager
	{
		MYGUI_INSTANCE_HEADER(ResourceManager);

	public:
		void initialise(const Ogre::String & _group = MyGUI::ResourceManager::GUIResourceGroupName);
		void shutdown();

	public:
		/** Register delegate for parsing XML blocks */
		LoadXmlDelegate & registerLoadXmlDelegate(const Ogre::String & _key);
		void unregisterLoadXmlDelegate(const Ogre::String & _key);

		/** Load additional MyGUI *_resource.xml file */
		bool load(const std::string & _file, const std::string & _group = MyGUI::ResourceManager::GUIResourceGroupName);

		bool _loadImplement(const std::string & _file, const std::string & _group, bool _match, const std::string & _type, const std::string & _instance);
		void _load(xml::ElementPtr _node, const std::string & _file, Version _version);
		void _loadLocation(xml::ElementPtr _node, const std::string & _file, Version _version);
		void _loadList(xml::ElementPtr _node, const std::string & _file, Version _version);

		/** Get name of ResourceGroup */
		const std::string& getResourceGroup() { return mResourceGroup; }

		/** Get resource by GUID */
		IResourcePtr getResource(const Guid & _id, bool _throw = true)
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
		IResourcePtr getResource(const std::string & _name, bool _throw = true)
		{
			MapResourceName::iterator iter = mResourceNames.find(_name);
			if (iter == mResourceNames.end()) {
				if (_throw) MYGUI_EXCEPT("resource '" << _name << "' not found");
				MYGUI_LOG(Warning, "resource '" << _name << "' not found");
				return nullptr;
			}
			return iter->second;
		}

		template <typename T>
		std::vector<T*> getResources()
		{
			std::vector<T*> ret;
			for (MapResource::const_iterator iter=mResources.begin(); iter!=mResources.end(); ++iter) {
				T* resource = iter->second->castType<T>(false);
				if (resource != nullptr) ret.push_back(resource);
			}
			return ret;
		}

		void registerType(const std::string & _type, CreatorDelegate::IDelegate * _delegate)
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

		/** Check is resource exist */
		bool isExist(const Guid & _id) { return mResources.find(_id) != mResources.end(); }
		/** Check is resource exist */
		bool isExist(const std::string & _name) { return mResourceNames.find(_name) != mResourceNames.end(); }
		/** Get resources Enumerator */
		EnumeratorMapResource getEnumerator() { return EnumeratorMapResource(mResources); }

		std::string getFileNameByID(const Guid& _id);

		static const std::string GUIResourceGroupName;
	private:

		MapDelegate mHolders;
		MapResource mResources;
		MapResourceName mResourceNames;

		// карта с делегатами для парсинга хмл блоков
		MapLoadXmlDelegate mMapLoadXmlDelegate;

		std::string mResourceGroup;
		typedef std::vector<Guid> VectorGuid;
		typedef std::map<std::string, VectorGuid> MapVectorString;

		MapVectorString mListFileGuid;
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_MANAGER_H__
