/*!
	@file
	@author		George Evmenov
	@date		06/2010
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
#ifndef __MYGUI_RESOURCE_LAYOUT_H__
#define __MYGUI_RESOURCE_LAYOUT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IResource.h"

namespace MyGUI
{

	// вспомогательный класс для инициализации одного скина
	class MYGUI_EXPORT ResourceLayout :
		public IResource
	{
		MYGUI_RTTI_DERIVED( ResourceLayout )

	public:
		ResourceLayout(xml::ElementPtr _node, const std::string& _file);

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		/** Create widgets described in layout */
		VectorWidgetPtr& create(const std::string& _prefix = "", Widget* _parent = nullptr);

	private:
		void parseLayout(VectorWidgetPtr* _widgets, xml::ElementPtr _root, const std::string& _prefix, Widget* _parent);
		void parseWidget(VectorWidgetPtr* _widgets, xml::ElementEnumerator& _widget, Widget* _parent, const std::string& _prefix);

		xml::ElementPtr mXmlNode;
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_LAYOUT_H__
