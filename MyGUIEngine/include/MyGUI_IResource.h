/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_I_RESOURCE_H__
#define __MYGUI_I_RESOURCE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Version.h"
#include "MyGUI_ISerializable.h"

namespace MyGUI
{

	class IResource;
	typedef IResource* IResourcePtr;

	class ResourceManager;

	class MYGUI_EXPORT IResource :
		public ISerializable
	{
		// для серелизации и удаления
		friend class ResourceManager;

		MYGUI_RTTI_DERIVED( IResource )

	public:
		const std::string& getResourceName() const
		{
			return mResourceName;
		}

		void setResourceName(const std::string& _value)
		{
			mResourceName = _value;
		}

	private:
		// constructors and operator =, without implementation, just for private
		IResource(IResource const&);
		IResource& operator = (IResource const&);

	protected:
		IResource() { }
		virtual ~IResource() { }

		virtual void deserialization(xml::ElementPtr _node, Version _version)
		{
			_node->findAttribute("name", mResourceName);
		}

	protected:
		std::string mResourceName;
	};

} // namespace MyGUI

#endif // __MYGUI_I_RESOURCE_H__
