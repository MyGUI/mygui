#ifndef __VSCROLL_H__
#define __VSCROLL_H__

#include "Prerequest.h"
#include "VScrollFactory.h"
#include "ScrollBase.h"

namespace MyGUI
{

	class _MyGUIExport VScroll : public ScrollBase
	{
		// для вызова закрытого конструктора
		friend factory::VScrollFactory;

	protected:
		VScroll(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:

		void notifyTrackMove(int _x, int _y);
		void updateTrack();


	}; // class VScroll : public ScrollBase

	typedef VScroll* VScrollPtr;

} // namespace MyGUI

#endif // __VSCROLL_H__