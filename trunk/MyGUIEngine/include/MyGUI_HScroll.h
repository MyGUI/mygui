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
		// для вызова закрытого конструктора
		friend class factory::HScrollFactory;

	protected:
		HScroll(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		/** Get size in pixels of area where scroll moves */
		int getLineSize();

		/** Set size of track in pixels
			@param _size in pixels, if less than MinTrackSize, MinTrackSize used
		*/
		void setTrackSize(size_t _size);
		/** Get size of track in pixels */
		int getTrackSize();

	private:
		void TrackMove(int _left, int _top);
		void updateTrack();

	}; // class HScroll : public VScroll

} // namespace MyGUI

#endif // __MYGUI_HSCROLL_H__
