/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_RESOURCE_MANAGER_H_
#define MYGUI_RESOURCE_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_Delegate.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class MYGUI_EXPORT ResourceManager :
		public Singleton<ResourceManager>,
		public MemberObsolete<ResourceManager>
	{
	public:
		ResourceManager();

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

		typedef delegates::CDelegate3<xml::ElementPtr, const std::string&, Version> LoadXmlDelegate;

		/** Register delegate that parse XML node with specified tag (_key) */
		LoadXmlDelegate& registerLoadXmlDelegate(const std::string& _key);

		/** Unregister delegate that parse XML node with specified tag (_key) */
		void unregisterLoadXmlDelegate(const std::string& _key);

		/** Check is resource exist */
		bool isExist(const std::string& _name) const;

		/** Find resource by name*/
		IResource* findByName(const std::string& _name) const;

		/** Get resource by name*/
		IResource* getByName(const std::string& _name, bool _throw = true) const;

		bool removeByName(const std::string& _name);

		void clear();

		typedef std::map<std::string, IResource*> MapResource;
		typedef Enumerator<MapResource> EnumeratorPtr;

		EnumeratorPtr getEnumerator() const;

		size_t getCount() const;

		const std::string& getCategoryName() const;

	private:
		void _loadList(xml::ElementPtr _node, const std::string& _file, Version _version);
		bool _loadImplement(const std::string& _file, bool _match, const std::string& _type, const std::string& _instance);

	private:
		// карта с делегатами для парсинга хмл блоков
		typedef std::map<std::string, LoadXmlDelegate> MapLoadXmlDelegate;
		MapLoadXmlDelegate mMapLoadXmlDelegate;

		MapResource mResources;

		typedef std::vector<IResource*> VectorResource;
		VectorResource mRemovedResoures;

		bool mIsInitialise;
		std::string mCategoryName;
		std::string mXmlListTagName;
	};

} // namespace MyGUI

#endif // MYGUI_RESOURCE_MANAGER_H_
