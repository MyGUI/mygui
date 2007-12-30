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
		// ��� ������ ��������� ������������
		friend class factory::VScrollFactory;

	protected:
		VScroll(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// ��� ������� �������
		inline static const Ogre::String & getType() {static Ogre::String type("VScroll"); return type;}

		void notifyTrackMove(int _left, int _top);
		void updateTrack();

	}; // class VScroll : public ScrollBase

} // namespace MyGUI

#endif // __MYGUI_VSCROLL_H__
