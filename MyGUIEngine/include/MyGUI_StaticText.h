/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_STATIC_TEXT_H__
#define __MYGUI_STATIC_TEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class _MyGUIExport StaticText : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<StaticText>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		StaticText(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

	}; // class _MyGUIExport StaticText : public Widget

} // namespace MyGUI

#endif // __MYGUI_STATIC_TEXT_H__
