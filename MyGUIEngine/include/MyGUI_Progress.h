/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_PROGRESS_H__
#define __MYGUI_PROGRESS_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Align.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class MYGUI_EXPORT Progress : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<Progress>;

		MYGUI_RTTI_CHILD_HEADER( Progress, Widget );

	public:
		/** Set progress range */
		void setProgressRange(size_t _range);
		/** Get progress range */
		size_t getProgressRange() {return mRange;}

		/** Set progress position */
		void setProgressPosition(size_t _pos);
		/** Get progress position */
		size_t getProgressPosition() {return mEndPosition;}

		/** FIXME что оно делает? почему нет в фактори метода? */
		void setProgressFillTrack(bool _fill);
		/** Get progress fill track flag */
		bool getProgressFillTrack() {return mFillTrack;}

		/** Enable or disable progress auto tracking */
		void setProgressAutoTrack(bool _auto);
		/** Get progress auto tracking flag */
		bool getProgressAutoTrack() {return mAutoTrack;}

		/** Set progress start point
			For example with Align::Top if will be filled from top to bottom.
		*/
		void setProgressStartPoint(Align _align = Align::Left);
		/** Get progress start point */
		Align getProgressStartPoint() {return mStartPoint;}

		//! @copydoc Widget::setPosition(const IntPoint & _point)
		virtual void setPosition(const IntPoint & _point);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize & _size);
		//! @copydoc Widget::setCoord(const IntCoord & _coord)
		virtual void setCoord(const IntCoord & _coord);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord & _coord) { setCoord(_coord); }
		MYGUI_OBSOLETE("use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height) { setCoord(_left, _top, _width, _height); }

#endif // MYGUI_DONT_USE_OBSOLETE

	protected:
		Progress(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~Progress();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

		void frameEntered(float _time);
		void updateTrack();

		int getClientWidth() {return ((mStartPoint.isLeft()) || (mStartPoint.isRight())) ? mWidgetClient->getWidth() : mWidgetClient->getHeight();}
		int getClientHeight() {return ((mStartPoint.isLeft()) || (mStartPoint.isRight())) ? mWidgetClient->getHeight() : mWidgetClient->getWidth();}

		void setTrackPosition(WidgetPtr _widget, int _left, int _top, int _width, int _height);

	private:
		std::string mTrackSkin;
		int mTrackWidth;
		int mTrackStep;
		int mTrackMin;

		VectorWidgetPtr mVectorTrack;
		size_t mRange;
		size_t mStartPosition, mEndPosition;
		float mAutoPosition;
		bool mAutoTrack;
		bool mFillTrack;

		Align mStartPoint;

	}; // class MYGUI_EXPORT Progress : public Widget

} // namespace MyGUI

#endif // __MYGUI_PROGRESS_H__
