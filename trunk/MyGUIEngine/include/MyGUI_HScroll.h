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
		/** Get size in pixels of area where scroll moves */
		int getLineSize();

		/** Set size of track in pixels
			@param _size in pixels, if less than MinTrackSize, MinTrackSize used
		*/
		void setTrackSize(size_t _size);
		/** Get size of track in pixels */
		int getTrackSize();

	protected:
		HScroll(WidgetType _behaviour, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
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
