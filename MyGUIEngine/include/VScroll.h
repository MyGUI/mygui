#ifndef _VSCROLL_H_
#define _VSCROLL_H_

#include "ScrollBase.h"

namespace MyGUI
{

	class _MyGUIExport VScroll : public ScrollBase
	{
		// для вызова закрытого конструктора
		friend WidgetFactory<VScroll>;

	protected:
		VScroll(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("VScroll"); return type;};

		void notifyTrackMove(int _x, int _y);

		void updateTrack();


	}; // class VScroll : public ScrollBase

	typedef VScroll * VScrollPtr;

} // namespace MyGUI


#endif // _VSCROLL_H_