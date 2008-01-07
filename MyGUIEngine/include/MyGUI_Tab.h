/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_TAB_H__
#define __MYGUI_TAB_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_TabFactory.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class Tab;
	typedef Tab* TabPtr;

	class _MyGUIExport Tab : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::TabFactory;

	protected:
		Tab(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// тип данного виджета
		inline static const Ogre::String & getType() {static Ogre::String type("Tab"); return type;}
	};

} // namespace MyGUI

#endif // __MYGUI_TAB_H__

