/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __MYGUI_DDCONTAINER_H__
#define __MYGUI_DDCONTAINER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class _MyGUIExport DDContainer : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::DDContainerFactory;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		DDContainer(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

	};

} // namespace MyGUI

#endif // __MYGUI_DDCONTAINER_H__
