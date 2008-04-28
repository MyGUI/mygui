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
		friend class factory::StaticTextFactory;

	protected:
		StaticText(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

	}; // class _MyGUIExport StaticText : public Widget

} // namespace MyGUI

#endif // __MYGUI_STATIC_TEXT_H__
