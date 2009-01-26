/*!
	@file
	@author		Alexander Ptakhin
	@date		01/2009
	@module
*/
#ifndef __MYGUI_CANVAS_FACTORY_H__
#define __MYGUI_CANVAS_FACTORY_H__

#include "MyGUI_Canvas.h"
#include "MyGUI_IWidgetFactory.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT CanvasFactory : public BaseWidgetFactory<Canvas>
		{
		public:
			CanvasFactory();
			~CanvasFactory();
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_CANVAS_FACTORY_H__
