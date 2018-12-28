/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_PROGRESS_BAR_H_
#define MYGUI_PROGRESS_BAR_H_

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
		void setPosition(const IntPoint& _value) override;
		//! @copydoc Widget::setSize(const IntSize& _value)
		void setSize(const IntSize& _value) override;
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		void setCoord(const IntCoord& _value) override;

		using Widget::setPosition;
		using Widget::setSize;
		using Widget::setCoord;

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void setPropertyOverride(const std::string& _key, const std::string& _value) override;

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

		Widget* mTrackPlace;
	};

} // namespace MyGUI

#endif // MYGUI_PROGRESS_BAR_H_
