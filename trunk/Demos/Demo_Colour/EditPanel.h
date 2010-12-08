/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __EDIT_PANEL_H__
#define __EDIT_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	ATTRIBUTE_CLASS_LAYOUT(EditPanel, "EditPanel.layout");
	class EditPanel :
		public wraps::BaseLayout
	{
	public:
		EditPanel();

		void setColourSelected(const MyGUI::Colour& _colour);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(EditPanel, mEdit, "Edit");
		MyGUI::EditBox* mEdit;
	};

} // namespace demo

#endif // __EDIT_PANEL_H__
