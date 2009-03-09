/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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
