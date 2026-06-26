/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_SERIALIZABLE_H_
#define MYGUI_I_SERIALIZABLE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IObject.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ObsoleteXmlTypes.h"
#include "MyGUI_Version.h"

namespace MyGUI
{

	class MYGUI_EXPORT ISerializable : public IObject
	{
		MYGUI_RTTI_DERIVED(ISerializable)

	public:
		virtual void serialize(pugi::xml_node /*_node*/, Version /*_version*/)
		{
		}
		virtual void deserialize(pugi::xml_node /*_node*/, Version /*_version*/)
		{
		}

		void deserializationWrapper(pugi::xml_node _node, Version _version)
		{
			this->deserialize(_node, _version);
#ifndef MYGUI_DONT_USE_OBSOLETE
			this->deserialization(xml::ElementPtr{_node}, _version);
#endif
		}

	private:
#ifndef MYGUI_DONT_USE_OBSOLETE
		// if you got here due to compiler error then replace
		//		BaseClass::deserialization(_node, _version) call with
		//		BaseClass::deserialize(_node, _version)
		// or switch to non-deprecated interface: virtual void deserialize(pugi::xml_node /*_node*/, Version /*_version*/)
		virtual void deserialization(xml::ElementPtr _node, Version _version)
		{
			this->deserialize(_node, _version);
		}
#endif
	};

} // namespace MyGUI

#endif // MYGUI_I_SERIALIZABLE_H_
