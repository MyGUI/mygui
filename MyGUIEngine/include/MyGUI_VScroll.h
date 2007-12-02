/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_VSCROLL_H__
#define __MYGUI_VSCROLL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_VScrollFactory.h"
#include "MyGUI_ScrollBase.h"

namespace MyGUI
{

	class VScroll;
	typedef VScroll* VScrollPtr;

	class _MyGUIExport VScroll : public ScrollBase
	{
		// для вызова закрытого конструктора
		friend factory::VScrollFactory;

	protected:
		VScroll(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:

		void notifyTrackMove(int _left, int _top);
		void updateTrack();

	}; // class VScroll : public ScrollBase

} // namespace MyGUI

#endif // __MYGUI_VSCROLL_H__