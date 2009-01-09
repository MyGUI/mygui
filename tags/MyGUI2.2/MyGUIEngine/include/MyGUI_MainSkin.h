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
#include "MyGUI_ICroppedRectangle.h"
#include "MyGUI_DrawItem.h"
#include "MyGUI_SubSkin.h"

namespace MyGUI
{

	class RenderItem;

	class MYGUI_EXPORT MainSkin : public SubSkin
	{
		MYGUI_RTTI_CHILD_HEADER(MainSkin, SubSkin);

	public:
		MainSkin(const SubWidgetInfo &_info, ICroppedRectangle * _parent);
		virtual ~MainSkin();

		// метод для генерации данных из описания xml
		static StateInfo * createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version);

	};

} // namespace MyGUI

#endif // __MYGUI_SUB_SKIN_H__
