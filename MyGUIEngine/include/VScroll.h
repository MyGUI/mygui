#ifndef __VSCROLL_H__
#define __VSCROLL_H__

#include "Prerequest.h"
#include "VScrollFactory.h"
#include "ScrollBase.h"

namespace MyGUI
{

	class VScroll;
	typedef VScroll* VScrollPtr;

	class _MyGUIExport VScroll : public ScrollBase
	{
		// для вызова закрытого конструктора
		friend factory::VScrollFactory;

	protected:
		VScroll(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:

		void notifyTrackMove(int _left, int _top);
		void updateTrack();

	}; // class VScroll : public ScrollBase

} // namespace MyGUI

#endif // __VSCROLL_H__