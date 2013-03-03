/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_SIMPLE_TEXT_H__
#define __MYGUI_SIMPLE_TEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_EditText.h"

namespace MyGUI
{

	class MYGUI_EXPORT SimpleText :
		public EditText
	{
		MYGUI_RTTI_DERIVED( SimpleText )

	public:
		SimpleText();
		virtual ~SimpleText();

		virtual void setViewOffset(const IntPoint& _point);

		virtual void doRender();
	};

} // namespace MyGUI

#endif // __MYGUI_SIMPLE_TEXT_H__
