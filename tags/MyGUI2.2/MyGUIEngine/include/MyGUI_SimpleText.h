/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_SIMPLE_TEXT_H__
#define __MYGUI_SIMPLE_TEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_DrawItem.h"
#include "MyGUI_Font.h"
#include "MyGUI_EnumCharInfo.h"
#include "MyGUI_EditText.h"

namespace MyGUI
{

	class RenderItem;

	class MYGUI_EXPORT SimpleText : public EditText
	{
		MYGUI_RTTI_CHILD_HEADER(SimpleText, EditText);

	public:
		SimpleText(const SubWidgetInfo &_info, ICroppedRectangle * _parent);
		virtual ~SimpleText();

		virtual void setViewOffset(IntPoint _point);

		// обновляет все данные связанные с тектом
		virtual void updateRawData();

		// метод для генерации данных из описания xml
		static StateInfo * createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version);

	};

} // namespace MyGUI

#endif // __MYGUI_SIMPLE_TEXT_H__
