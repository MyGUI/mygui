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

	protected:
		virtual ~IResource() { };

	};

#define MYGUI_RESOURCE_HEADER \
	MYGUI_RTTI_CHILD_HEADER; \
	private: \
		static void createResource(IResourcePtr & _resource, xml::xmlNodeIterator _node); \
	public: \
		static void registryType(); \
		static void unregistryType();

#define MYGUI_RESOURCE_IMPLEMENT(name, base) \
	MYGUI_RTTI_CHILD_IMPLEMENT(name, base); \
	void name::registryType() \
	{ \
		ResourceManager::getInstance().registerType(name::getClassTypeName(), newDelegate(name::createResource)); \
	} \
	void ResourceImageSet::unregistryType() \
	{ \
		ResourceManager::getInstance().unregisterType(name::getClassTypeName()); \
	} \
	void name::createResource(IResourcePtr & _resource, xml::xmlNodeIterator _node) \
	{ \
		_resource = new name(_node); \
	}

} // namespace MyGUI

#endif // __MYGUI_I_RESOURCE_H__
