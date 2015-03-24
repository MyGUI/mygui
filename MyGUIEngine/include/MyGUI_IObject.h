/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_OBJECT_H_
#define MYGUI_I_OBJECT_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RTTI.h"

namespace MyGUI
{

	class MYGUI_EXPORT IObject
	{
		MYGUI_RTTI_BASE( IObject )

	public:
		IObject() { }
		virtual ~IObject() { }
	};

} // namespace MyGUI

#endif // MYGUI_I_OBJECT_H_
