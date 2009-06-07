/*!
	@file
	@author		Albert Semenov
	@date		06/2009
	@module
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
#ifndef __MYGUI_SERIALIZABLE_H__
#define __MYGUI_SERIALIZABLE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_RTTI.h"
#include "MyGUI_Object.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Version.h"

namespace MyGUI
{

	class MYGUI_EXPORT Serializable : public Object
	{
		MYGUI_RTTI_DERIVED( Serializable );

	public:
		Serializable() { }
		virtual ~Serializable() { }

		virtual void serialization(xml::ElementPtr _node, Version _version) { }
		virtual void deserialization(xml::ElementPtr _node, Version _version) { }

	};

} // namespace MyGUI

#endif // __MYGUI_SERIALIZABLE_H__
