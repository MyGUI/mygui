/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_I_RESOURCE_H__
#define __MYGUI_I_RESOURCE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Rtti.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Guid.h"

namespace MyGUI
{

	class IResource;
	typedef IResource * IResourcePtr;

	class ResourceManager;

	class _MyGUIExport IResource
	{
		// для удаления
		friend class ResourceManager;

		MYGUI_RTTI_BASE_HEADER;

	public:
		const std::string & getResourceName() { return mResourceName; }
		const Guid & getResourceID() { return mResourceID; }

	private:
		IResource() { }
		IResource(IResource const &) { }
		IResource & operator = (IResource const &) { return *this; }
		
	protected:
		IResource(xml::xmlNodeIterator _node)
		{
			mResourceID = Guid::parse(_node->findAttribute("id"));
			mResourceName = _node->findAttribute("name");
		}

		virtual ~IResource() = 0;

	private:
		std::string mResourceName;
		Guid mResourceID;

	};

#define MYGUI_RESOURCE_HEADER \
	MYGUI_RTTI_CHILD_HEADER; \
	private: \
		static void createResource(MyGUI::IResourcePtr & _resource, MyGUI::xml::xmlNodeIterator _node); \
	public: \
		static void registryType(); \
		static void unregistryType();

#define MYGUI_RESOURCE_IMPLEMENT(name, base) \
	MYGUI_RTTI_CHILD_IMPLEMENT(name, base); \
	void name::registryType() \
	{ \
		MyGUI::ResourceManager::getInstance().registerType(name::getClassTypeName(), MyGUI::newDelegate(name::createResource)); \
	} \
	void name::unregistryType() \
	{ \
		MyGUI::ResourceManager::getInstance().unregisterType(name::getClassTypeName()); \
	} \
	void name::createResource(MyGUI::IResourcePtr & _resource, MyGUI::xml::xmlNodeIterator _node) \
	{ \
		_resource = new name(_node); \
	}

} // namespace MyGUI

#endif // __MYGUI_I_RESOURCE_H__
