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
#ifndef __MYGUI_RESOURCE_MANAGER_H__
#define __MYGUI_RESOURCE_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_Guid.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_ResourceHolder.h"

namespace MyGUI
{
	typedef delegates::CDelegate3<IResourcePtr &, xml::ElementEnumerator, Version> CreatorDelegate;

	typedef delegates::CDelegate3<xml::ElementPtr, const std::string &, Version> LoadXmlDelegate;
	typedef std::map<std::string, LoadXmlDelegate> MapLoadXmlDelegate;

	typedef std::map<std::string, CreatorDelegate> MapDelegate;
	typedef std::map<Guid, IResourcePtr> MapResourceID;
	//typedef std::map<std::string, IResourcePtr> MapResourceName;
	//typedef Enumerator<MapResource> EnumeratorMapResource;

	class MYGUI_EXPORT ResourceManager :
		public ResourceHolder<IResource>
	{
		MYGUI_INSTANCE_HEADER(ResourceManager);

	public:
		void initialise(/*const std::string& _group = MyGUI::ResourceManager::GUIResourceGroupName*/);
		void shutdown();

	public:

		/** Load additional MyGUI *_resource.xml file */
		bool load(const std::string& _file/*, const std::string& _group = MyGUI::ResourceManager::GUIResourceGroupName*/);

		bool _loadImplement(const std::string& _file, /*const std::string& _group, */bool _match, const std::string& _type, const std::string& _instance);
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);
		void _loadList(xml::ElementPtr _node, const std::string& _file, Version _version);

		/** Get name of ResourceGroup */
		//const std::string& getResourceGroup() { return mResourceGroup; }

		/** Get resource by GUID */
		IResourcePtr getByID(const Guid& _id, bool _throw = true);

		/** Get resource by name */
		//IResourcePtr getByName(const std::string& _name, bool _throw = true);

		/*template <typename T>
		std::vector<T*> getResources()
		{
			std::vector<T*> ret;
			for (MapResource::const_iterator iter=mResourcesID.begin(); iter!=mResourcesID.end(); ++iter)
			{
				T* resource = iter->second->castType<T>(false);
				if (resource != nullptr) ret.push_back(resource);
			}
			return ret;
		}*/

		//void registerType(const std::string& _type, CreatorDelegate::IDelegate * _delegate);

		//void unregisterType(const std::string& _type);

		//void clear();

		//size_t getCount() { return mResourcesID.size(); }

		/** Check is resource exist */
		//bool isExist(const Guid & _id) { return mResourcesID.find(_id) != mResourcesID.end(); }
		/** Check is resource exist */
		//bool isExist(const std::string& _name) { return mResources.find(_name) != mResources.end(); }
		/** Get resources Enumerator */
		//EnumeratorMapResource getEnumerator() { return EnumeratorMapResource(mResourcesID); }

		std::string getFileNameByID(const Guid& _id);

		//static const std::string GUIResourceGroupName;

	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : size_t ResourceManager::getCount()")
		size_t getResourceCount() { return getCount(); }

		MYGUI_OBSOLETE("use : IResourcePtr ResourceManager::getByName(const std::string& _name, bool _throw)")
		IResourcePtr getResource(const std::string& _name, bool _throw = true) { return getByName(_name, _throw); }

		MYGUI_OBSOLETE("use : IResourcePtr ResourceManager::getByID(const Guid& _id, bool _throw)")
		IResourcePtr getResource(const Guid& _id, bool _throw = true) { return getByID(_id, _throw); }

		MYGUI_OBSOLETE("use : void FactoryManager::registryFactory(const std::string& _category, const std::string& _type, Delegate::IDelegate* _delegate)")
		LoadXmlDelegate & registerLoadXmlDelegate(const std::string& _key);

		MYGUI_OBSOLETE("use : void FactoryManager::unregistryFactory(const std::string& _category, const std::string& _type)")
		void unregisterLoadXmlDelegate(const std::string& _key);

#endif // MYGUI_DONT_USE_OBSOLETE

	private:
		//MapDelegate mHolders;
		MapResourceID mResourcesID;
		//MapResourceName mResources;

		// карта с делегатами для парсинга хмл блоков
		MapLoadXmlDelegate mMapLoadXmlDelegate;

		std::string mResourceGroup;
		typedef std::vector<Guid> VectorGuid;
		typedef std::map<std::string, VectorGuid> MapVectorString;

		MapVectorString mListFileGuid;
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_MANAGER_H__
