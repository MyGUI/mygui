/*!
	@file
	@author		Albert Semenov
	@date		09/2008
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
#include "MyGUI_Singleton.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_ResourceHolder.h"
#include "MyGUI_Delegate.h"

namespace MyGUI
{

	class MYGUI_EXPORT ResourceManager :
		public Singleton<ResourceManager>,
		public ResourceHolder<IResource>
	{
	public:
		void initialise();
		void shutdown();

	public:

		/** Load additional MyGUI *_resource.xml file */
		bool load(const std::string& _file);

		void loadFromXmlNode(xml::ElementPtr _node, const std::string& _file, Version _version);

		/** Add resource item to resources */
		void addResource(IResourcePtr _item);

		/** Remove resource item from resources */
		void removeResource(IResourcePtr _item);

		typedef delegates::CDelegate3<xml::ElementPtr, const std::string &, Version> LoadXmlDelegate;

		/** Register delegate that parse XML node with specified tag (_key) */
		LoadXmlDelegate& registerLoadXmlDelegate(const std::string& _key);

		/** Unregister delegate that parse XML node with specified tag (_key) */
		void unregisterLoadXmlDelegate(const std::string& _key);

	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : size_t ResourceManager::getCount()")
		size_t getResourceCount() { return getCount(); }

		MYGUI_OBSOLETE("use : IResourcePtr ResourceManager::getByName(const std::string& _name, bool _throw)")
		IResourcePtr getResource(const std::string& _name, bool _throw = true) { return getByName(_name, _throw); }

#endif // MYGUI_DONT_USE_OBSOLETE

	private:
		void _loadList(xml::ElementPtr _node, const std::string& _file, Version _version);
		bool _loadImplement(const std::string& _file, bool _match, const std::string& _type, const std::string& _instance);

	private:
		// карта с делегатами для парсинга хмл блоков
		typedef std::map<std::string, LoadXmlDelegate> MapLoadXmlDelegate;
		MapLoadXmlDelegate mMapLoadXmlDelegate;

		std::string mResourceGroup;
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_MANAGER_H__
