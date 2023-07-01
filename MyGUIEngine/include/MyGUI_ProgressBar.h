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
	class MYGUI_EXPORT ProgressBar : public Widget, public MemberObsolete<ProgressBar>
	{
		MYGUI_RTTI_DERIVED(ProgressBar)

	public:
		/** Set progress range */
		void setProgressRange(size_t _range);
		/** Get progress range */
		size_t getProgressRange() const;

		/** Set progress position */
		void setProgressPosition(size_t _pos);
		/** Get progress position */
		size_t getProgressPosition() const;

		/** Enable or disable progress auto tracking */
		void setProgressAutoTrack(bool _auto);
		/** Get progress auto tracking flag */
		bool getProgressAutoTrack() const;

		void setFlowDirection(FlowDirection _value);
		FlowDirection getFlowDirection() const;

		//! @copydoc Widget::setPosition(const IntPoint& _point)
		void setPosition(const IntPoint& _point) override;
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size) override;
		//! @copydoc Widget::setCoord(const IntCoord& _coord)
		void setCoord(const IntCoord& _coord) override;

		using Widget::setPosition;
		using Widget::setSize;
		using Widget::setCoord;

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void setPropertyOverride(std::string_view _key, std::string_view _value) override;

	private:
		void frameEntered(float _time);
		void updateTrack();

		int getClientWidth() const;
		int getClientHeight() const;

		void setTrackPosition(Widget* _widget, int _left, int _top, int _width, int _height);

	private:
		std::string mTrackSkin;
		int mTrackWidth{1};
		int mTrackStep{0};
		int mTrackMin{0};

		VectorWidgetPtr mVectorTrack;
		size_t mRange{0};
		size_t mStartPosition{0}, mEndPosition{0};
		float mAutoPosition{0.0f};
		bool mAutoTrack{false};
		bool mFillTrack{false};

		FlowDirection mFlowDirection;

		Widget* mTrackPlace{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_PROGRESS_BAR_H_
