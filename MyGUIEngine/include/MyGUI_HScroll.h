/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_HSCROLL_H__
#define __MYGUI_HSCROLL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_HScrollFactory.h"
#include "MyGUI_ScrollBase.h"

namespace MyGUI
{

	class HScroll;
	typedef HScroll * HScrollPtr;

	class _MyGUIExport HScroll : public ScrollBase
	{
		// для вызова закрытого конструктора
		friend factory::HScrollFactory;

	protected:
		HScroll(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// тип данного виджета
		inline static const Ogre::String & getType() {static Ogre::String type("HScroll"); return type;}

		void notifyTrackMove(int _left, int _top);
		void updateTrack();

	}; // class HScroll : public ScrollBase

} // namespace MyGUI

#endif // __MYGUI_HSCROLL_H__