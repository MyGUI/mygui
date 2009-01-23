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
#include "MyGUI_Common.h"
#include "MyGUI_Version.h"

namespace MyGUI
{

	class IResource;
	typedef IResource * IResourcePtr;

	class ResourceManager;

	class MYGUI_EXPORT IResource
	{
		// для удаления
		friend class ResourceManager;

		MYGUI_RTTI_BASE_HEADER( IResource );

	public:
		const std::string & getResourceName() { return mResourceName; }
		const Guid & getResourceID() { return mResourceID; }

	private:
		IResource() { }
		IResource(IResource const &) { }
		IResource & operator = (IResource const &) { return *this; }
		
	protected:
		IResource(xml::ElementEnumerator _node, Version _version)
		{
			mResourceID = Guid::parse(_node->findAttribute("id"));
			mResourceName = _node->findAttribute("name");
		}

		virtual ~IResource() { }

	private:
		std::string mResourceName;
		Guid mResourceID;
	};


	#define MYGUI_RESOURCE_HEADER( T , BT ) \
		MYGUI_RTTI_CHILD_HEADER(T, BT); \
		private: \
			 static void createResource(MyGUI::IResourcePtr & _resource, MyGUI::xml::ElementEnumerator _node, MyGUI::Version _version) { _resource = new T(_node, _version); } \
		public: \
			static void registryType() { MyGUI::ResourceManager::getInstance().registerType(T::getClassTypeName(), MyGUI::newDelegate(T::createResource)); } \
			static void unregistryType() { MyGUI::ResourceManager::getInstance().unregisterType(T::getClassTypeName()); }

} // namespace MyGUI

#endif // __MYGUI_I_RESOURCE_H__
