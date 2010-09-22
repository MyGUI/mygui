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
