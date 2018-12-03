/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_RESOURCE_H_
#define MYGUI_I_RESOURCE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Version.h"
#include "MyGUI_ISerializable.h"

namespace MyGUI
{

	class IResource;
	using IResourcePtr = IResource*;

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

	protected:
		IResource() = default;
		~IResource() override = default;
		IResource(IResource const&) = delete;
		IResource& operator = (IResource const&) = delete;

		void deserialization(xml::ElementPtr _node, Version _version) override
		{
			_node->findAttribute("name", mResourceName);
		}

	protected:
		std::string mResourceName;
	};

} // namespace MyGUI

#endif // MYGUI_I_RESOURCE_H_
