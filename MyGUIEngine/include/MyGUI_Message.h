/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_MESSAGE_H__
#define __MYGUI_MESSAGE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Window.h"

namespace MyGUI
{

	class _MyGUIExport Message : public Window
	{
		// для вызова закрытого конструктора
		friend class factory::MessageFactory;

	protected:
		Message(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// тип данного виджета
		inline static const Ogre::String & _getType() {static Ogre::String type("Message"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }


	}; // class _MyGUIExport Message : public Window

} // namespace MyGUI

#endif // __MYGUI_MESSAGE_H__
