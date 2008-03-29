/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_SIMPLE_TEXT_H__
#define __MYGUI_SIMPLE_TEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_SubWidgetTextInterface.h"
#include "MyGUI_DrawItem.h"
#include "MyGUI_Font.h"
#include "MyGUI_EnumCharInfo.h"
#include "MyGUI_EditText.h"

namespace MyGUI
{

	class RenderItem;

	class _MyGUIExport SimpleText : public EditText
	{

	public:
		SimpleText(const SubWidgetInfo &_info, CroppedRectanglePtr _parent);
		virtual ~SimpleText();

	};

} // namespace MyGUI

#endif // __MYGUI_SIMPLE_TEXT_H__
