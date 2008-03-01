/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_HSCROLL_H__
#define __MYGUI_HSCROLL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_VScroll.h"

namespace MyGUI
{

	class _MyGUIExport HScroll : public VScroll
	{
		// ��� ������ ��������� ������������
		friend class factory::HScrollFactory;

	protected:
		HScroll(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);

	public:
		// ��� ������� �������
		inline static const Ogre::String & _getType() {static Ogre::String type("HScroll"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		void TrackMove(int _left, int _top);
		void updateTrack();

	}; // class HScroll : public VScroll

} // namespace MyGUI

#endif // __MYGUI_HSCROLL_H__
