#ifndef __HSCROLL_H__
#define __HSCROLL_H__

#include "Prerequest.h"
#include "HScrollFactory.h"
#include "ScrollBase.h"

namespace MyGUI
{

	class HScroll;
	typedef HScroll * HScrollPtr;

	class _MyGUIExport HScroll : public ScrollBase
	{
		// для вызова закрытого конструктора
		friend factory::HScrollFactory;

	protected:
		HScroll(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:

		void notifyTrackMove(int _left, int _top);
		void updateTrack();

	}; // class HScroll : public ScrollBase

} // namespace MyGUI

#endif // __HSCROLL_H__