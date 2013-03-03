/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_I_SERIALIZABLE_H__
#define __MYGUI_I_SERIALIZABLE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IObject.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Version.h"

namespace MyGUI
{

	class MYGUI_EXPORT ISerializable :
		public IObject
	{
		MYGUI_RTTI_DERIVED( ISerializable )

	public:
		ISerializable() { }
		virtual ~ISerializable() { }

		virtual void serialization(xml::ElementPtr _node, Version _version) { }
		virtual void deserialization(xml::ElementPtr _node, Version _version) { }
	};

} // namespace MyGUI

#endif // __MYGUI_I_SERIALIZABLE_H__
