/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_I_OBJECT_H__
#define __MYGUI_I_OBJECT_H__

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

#endif // __MYGUI_I_OBJECT_H__
