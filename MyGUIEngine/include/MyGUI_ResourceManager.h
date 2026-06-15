/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_RESOURCE_MANAGER_H_
#define MYGUI_RESOURCE_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_Delegate.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class MYGUI_EXPORT ResourceManager : public MemberObsolete<ResourceManager>
	{
		MYGUI_SINGLETON_DECLARATION(ResourceManager);

	public:
		ResourceManager();

		void initialise();
		void shutdown();

	public:
		/** Load additional MyGUI *_resource.xml file */
		bool load(const std::string& _file);

		void loadFromXmlNode(xml::ElementPtr _node, std::string_view _file, Version _version);

		/** Add resource item to resources */
		void addResource(IResourcePtr _item);

		/** Remove resource item from resources */
		void removeResource(IResourcePtr _item);

		using LoadXmlDelegate = EventPairConvertStringView<
			delegates::Delegate<xml::ElementPtr, const std::string&, Version>,
			delegates::Delegate<xml::ElementPtr, std::string_view, Version>>;

		/** Register delegate that parse XML node with specified tag (_key) */
		LoadXmlDelegate& registerLoadXmlDelegate(std::string_view _key);

		/** Unregister delegate that parse XML node with specified tag (_key) */
		void unregisterLoadXmlDelegate(std::string_view _key);

		/** Check is resource exist */
		bool isExist(std::string_view _name) const;

		/** Find resource by name*/
		IResource* findByName(std::string_view _name) const;

		/** Get resource by name*/
		IResource* getByName(std::string_view _name, bool _throw = true) const;

		bool removeByName(std::string_view _name);

		void clear();

		using MapResource = std::map<std::string, IResource*, std::less<>>;
		using EnumeratorPtr = Enumerator<MapResource>;

		EnumeratorPtr getEnumerator() const;

		size_t getCount() const;

		const std::string& getCategoryName() const;

	private:
		void _loadList(xml::ElementPtr _node, std::string_view _file, Version _version);
		bool _loadImplement(const std::string& _file, bool _match, std::string_view _type, std::string_view _instance);

	private:
		// карта с делегатами для парсинга хмл блоков
		using MapLoadXmlDelegate = std::map<std::string, LoadXmlDelegate, std::less<>>;
		MapLoadXmlDelegate mMapLoadXmlDelegate;

		MapResource mResources;

		using VectorResource = std::vector<IResource*>;
		VectorResource mRemovedResources;

		bool mIsInitialise{false};
		std::string mCategoryName;
		std::string mXmlListTagName;
	};

} // namespace MyGUI

#endif // MYGUI_RESOURCE_MANAGER_H_
