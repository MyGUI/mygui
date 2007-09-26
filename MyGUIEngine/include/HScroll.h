#ifndef _HSCROLL_H_
#define _HSCROLL_H_

#include "ScrollBase.h"

namespace MyGUI
{

	class _MyGUIExport HScroll : public ScrollBase
	{
		// для вызова закрытого конструктора
		friend WidgetFactory<HScroll>;

	protected:
		HScroll(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("HScroll"); return type;};

		void notifyTrackMove(int _x, int _y);

		void updateTrack();


	}; // class HScroll : public ScrollBase

	typedef HScroll * HScrollPtr;

} // namespace MyGUI


#endif // _HSCROLL_H_