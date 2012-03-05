/*!
	@file
	@author		Albert Semenov
	@date		01/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_PROGRESS_BAR_H__
#define __MYGUI_PROGRESS_BAR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Align.h"
#include "MyGUI_FlowDirection.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	/** \brief @wpage{ProgressBar}
		ProgressBar widget description should be here.
	*/
	class MYGUI_EXPORT ProgressBar :
		public Widget,
		public MemberObsolete<ProgressBar>
	{
		MYGUI_RTTI_DERIVED( ProgressBar )

	public:
		ProgressBar();

		/** Set progress range */
		void setProgressRange(size_t _value);
		/** Get progress range */
		size_t getProgressRange() const;

		/** Set progress position */
		void setProgressPosition(size_t _value);
		/** Get progress position */
		size_t getProgressPosition() const;

		/** Enable or disable progress auto tracking */
		void setProgressAutoTrack(bool _value);
		/** Get progress auto tracking flag */
		bool getProgressAutoTrack() const;

		void setFlowDirection(FlowDirection _value);
		FlowDirection getFlowDirection() const;

		//! @copydoc Widget::setPosition(const IntPoint& _value)
		virtual void setPosition(const IntPoint& _value);
		//! @copydoc Widget::setSize(const IntSize& _value)
		virtual void setSize(const IntSize& _value);
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		virtual void setCoord(const IntCoord& _value);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top);
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height);
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height);

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		void frameEntered(float _time);
		void updateTrack();

		int getClientWidth();
		int getClientHeight();

		void setTrackPosition(Widget* _widget, int _left, int _top, int _width, int _height);

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

		FlowDirection mFlowDirection;

		Widget* mClient;
	};

} // namespace MyGUI

#endif // __MYGUI_PROGRESS_BAR_H__
