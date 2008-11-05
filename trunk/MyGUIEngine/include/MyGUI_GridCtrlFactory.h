/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#ifndef __MYGUI_GRID_CTRL_FACTORY_H__
#define __MYGUI_GRID_CTRL_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_GridCtrl.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport GridCtrlFactory : public BaseWidgetFactory<GridCtrl>
		{
		public:
			GridCtrlFactory();
			~GridCtrlFactory();
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_GRID_CTRL_FACTORY_H__
