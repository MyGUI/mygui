/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_IResource.h"

#include <pugixml.hpp>

namespace MyGUI
{

	IResource::~IResource() = default;

	void IResource::deserialization(xml::ElementPtr _node, Version /*_version*/)
	{
		auto attr = _node.node().attribute("name");
		if (attr)
			mResourceName = attr.value();
	}

} // namespace MyGUI
