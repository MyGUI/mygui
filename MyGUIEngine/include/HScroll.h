#ifndef __HSCROLL_H__
#define __HSCROLL_H__

#include "Prerequest.h"
#include "HScrollFactory.h"
#include "ScrollBase.h"

namespace MyGUI
{

	class _MyGUIExport HScroll : public ScrollBase
	{
		// для вызова закрытого конструктора
		friend factory::HScrollFactory;

	protected:
		HScroll(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:

		void notifyTrackMove(int _x, int _y);
		void updateTrack();

	}; // class HScroll : public ScrollBase

	typedef HScroll * HScrollPtr;

} // namespace MyGUI

#endif // __HSCROLL_H__