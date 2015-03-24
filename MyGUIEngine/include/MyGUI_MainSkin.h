/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_MAIN_SKIN_H_
#define MYGUI_MAIN_SKIN_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_SubSkin.h"

namespace MyGUI
{

	class MYGUI_EXPORT MainSkin :
		public SubSkin
	{
		MYGUI_RTTI_DERIVED( MainSkin )

	public:
		MainSkin();
		virtual ~MainSkin();

		/*internal:*/
		virtual void _setAlign(const IntSize& _oldsize);
	};

} // namespace MyGUI

#endif // MYGUI_SUB_SKIN_H_
