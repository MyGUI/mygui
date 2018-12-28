/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef DEMO_RESOURCE_ITEM_INFO_H_
#define DEMO_RESOURCE_ITEM_INFO_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_ResourceManager.h"

namespace demo
{

	class ResourceItemInfo;
	typedef ResourceItemInfo* ResourceItemInfoPtr;

	class ResourceItemInfo :
		public MyGUI::IResource,
		public MyGUI::GenericFactory<ResourceItemInfo>
	{
		friend class MyGUI::GenericFactory<ResourceItemInfo>;

		MYGUI_RTTI_DERIVED( ResourceItemInfo )

	private:
		void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version) override;

	public:
		const std::string& getItemName();
		const std::string& getItemDescription();
		const std::string& getItemResourceImage();

	private:
		std::string mItemName;
		std::string mItemDescription;
		std::string mItemResourceImage;
	};

} // namespace demo

#endif // DEMO_RESOURCE_ITEM_INFO_H_
