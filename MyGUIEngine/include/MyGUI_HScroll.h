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

	class MYGUI_EXPORT HScroll : public VScroll
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<HScroll>;

		MYGUI_RTTI_CHILD_HEADER( HScroll, VScroll );

	public:
		//! @copydoc VScroll::getLineSize()
		virtual int getLineSize();

		//! @copydoc VScroll::setTrackSize(int _size)
		virtual void setTrackSize(int _size);
		//! @copydoc VScroll::getTrackSize()
		virtual int getTrackSize();

	protected:
		HScroll(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~HScroll();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);
		
	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

		void TrackMove(int _left, int _top);
		void updateTrack();

	}; // class HScroll : public VScroll

} // namespace MyGUI

#endif // __MYGUI_HSCROLL_H__
