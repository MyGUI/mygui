/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __DEMO_RESOURCE_ITEM_INFO_H__
#define __DEMO_RESOURCE_ITEM_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_ResourceManager.h"

namespace demo
{

	class ResourceItemInfo;
	typedef ResourceItemInfo * ResourceItemInfoPtr;

	class ResourceItemInfo :
		public MyGUI::IResource,
		public MyGUI::GenericFactory<ResourceItemInfo>
	{
		friend class MyGUI::GenericFactory<ResourceItemInfo>;

		MYGUI_RTTI_DERIVED( ResourceItemInfo );

	private:
		ResourceItemInfo() { }
		virtual ~ResourceItemInfo() { }

		virtual void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version);

	public:
		const std::string& getItemName() { return mItemName; }
		const std::string& getItemDescription() { return mItemDescription; }
		const std::string& getItemResourceImage() { return mItemResourceImage; }

	private:
		std::string mItemName;
		std::string mItemDescription;
		std::string mItemResourceImage;

	};

} // namespace demo

#endif // __DEMO_RESOURCE_ITEM_INFO_H__
