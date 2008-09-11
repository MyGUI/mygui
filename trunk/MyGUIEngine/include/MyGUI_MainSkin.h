/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_MAIN_SKIN_H__
#define __MYGUI_MAIN_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_CroppedRectangleInterface.h"
#include "MyGUI_DrawItem.h"
#include "MyGUI_SubSkin.h"

namespace MyGUI
{

	class RenderItem;

	class _MyGUIExport MainSkin : public SubSkin
	{
	public:
		MainSkin(const SubWidgetInfo &_info, CroppedRectangleInterface * _parent);
		virtual ~MainSkin();

		// метод для генерации данных из описания xml
		static void * createStateData(xml::xmlNodePtr _node, xml::xmlNodePtr _root);
	};

} // namespace MyGUI

#endif // __MYGUI_SUB_SKIN_H__
