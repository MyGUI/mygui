/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/
#ifndef __PANEL_H__
#define __PANEL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class Panel :
		public Widget
	{
		MYGUI_RTTI_DERIVED( Panel )

	public:
		Panel();

	};

} // namespace MyGUI

#endif // __PANEL_H__
