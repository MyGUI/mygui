/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ProgressBar.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Gui.h"
#include "MyGUI_SkinManager.h"

namespace MyGUI
{

	const size_t PROGRESS_AUTO_WIDTH = 200;
	const size_t PROGRESS_AUTO_RANGE = 1000;
	const float PROGRESS_AUTO_COEF = 400;

	ProgressBar::ProgressBar() :
		mTrackWidth(1),
		mTrackStep(0),
		mTrackMin(0),
		mRange(0),
		mStartPosition(0),
		mEndPosition(0),
		mAutoPosition(0.0f),
		mAutoTrack(false),
		mFillTrack(false),
		mClient(nullptr)
	{
	}

	void ProgressBar::initialiseOverride()
	{
		Base::initialiseOverride();

		///@wskin_child{MultiListBox, Widget, TrackPlace} Место для трекера.
		assignWidget(mClient, "TrackPlace");

		if (nullptr == mClient)
		{
			//OBSOLETE
			assignWidget(mClient, "Client");

			if (nullptr == mClient)
				mClient = this;
		}

		if (isUserString("TrackSkin"))
			mTrackSkin = getUserString("TrackSkin");

		if (isUserString("TrackWidth"))
			mTrackWidth = utility::parseValue<int>(getUserString("TrackWidth"));

		//OBSOLETE
		if (isUserString("TrackMin"))
			mTrackMin = utility::parseValue<int>(getUserString("TrackMin"));

		else
			mTrackMin = mTrackWidth;

		if (isUserString("TrackStep"))
			mTrackStep = utility::parseValue<int>(getUserString("TrackStep"));

		if (isUserString("TrackFill"))
			mFillTrack = utility::parseValue<bool>(getUserString("TrackFill"));

		if (!isUserString("TrackStep"))
			mTrackStep = mTrackWidth;

		if (1 > mTrackWidth)
			mTrackWidth = 1;
	}

	void ProgressBar::shutdownOverride()
	{
		mClient = nullptr;

		Base::shutdownOverride();
	}

	void ProgressBar::setProgressRange(size_t _range)
	{
		if (mAutoTrack) return;
		mRange = _range;
		if (mEndPosition > mRange) mEndPosition = mRange;
		if (mStartPosition > mRange) mStartPosition = mRange;
		updateTrack();
	}

	void ProgressBar::setProgressPosition(size_t _pos)
	{
		if (mAutoTrack) return;
		mEndPosition = _pos;
		if (mEndPosition > mRange) mEndPosition = mRange;
		updateTrack();
	}

	void ProgressBar::setProgressAutoTrack(bool _auto)
	{
		if (mAutoTrack == _auto) return;
		mAutoTrack = _auto;

		if (mAutoTrack)
		{
			Gui::getInstance().eventFrameStart += newDelegate(this, &ProgressBar::frameEntered);
			mRange = PROGRESS_AUTO_RANGE;
			mEndPosition = mStartPosition = 0;
			mAutoPosition = 0.0f;
		}
		else
		{
			Gui::getInstance().eventFrameStart -= newDelegate(this, &ProgressBar::frameEntered);
			mRange = mEndPosition = mStartPosition = 0;
		}
		updateTrack();
	}

	void ProgressBar::frameEntered(float _time)
	{
		if (!mAutoTrack) return;
		mAutoPosition += (PROGRESS_AUTO_COEF * _time);
		size_t pos = (size_t)mAutoPosition;

		if (pos > (mRange + PROGRESS_AUTO_WIDTH)) mAutoPosition = 0.0f;

		if (pos > mRange) mEndPosition = mRange;
		else mEndPosition = size_t(mAutoPosition);

		if (pos < PROGRESS_AUTO_WIDTH) mStartPosition = 0;
		else mStartPosition = pos - PROGRESS_AUTO_WIDTH;

		updateTrack();
	}

	void ProgressBar::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);
	}

	void ProgressBar::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		updateTrack();
	}

	void ProgressBar::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);

		updateTrack();
	}

	void ProgressBar::updateTrack()
	{
		// все скрыто
		if ((0 == mRange) || (0 == mEndPosition))
		{
			for (VectorWidgetPtr::iterator iter = mVectorTrack.begin(); iter != mVectorTrack.end(); ++iter)
			{
				(*iter)->setVisible(false);
			}
			return;
		}

		// тут попроще расчеты
		if (mFillTrack)
		{
			if (mVectorTrack.empty())
			{
				Widget* widget = mClient->createWidget<Widget>(mTrackSkin, IntCoord(), Align::Left | Align::VStretch);
				mVectorTrack.push_back(widget);
			}
			else
			{
				// первый показываем и ставим норм альфу
				VectorWidgetPtr::iterator iter = mVectorTrack.begin();
				(*iter)->setVisible(true);
				(*iter)->setAlpha(ALPHA_MAX);

				// все начиная со второго скрываем
				++iter;
				for (; iter != mVectorTrack.end(); ++iter)
				{
					(*iter)->setVisible(false);
				}
			}

			Widget* wid = mVectorTrack.front();

			// полностью виден
			if ((0 == mStartPosition) && (mRange == mEndPosition))
			{
				setTrackPosition(wid, 0, 0, getClientWidth(), getClientHeight());
			}
			// эх
			else
			{
				int pos = (int)mStartPosition * (getClientWidth() - mTrackMin) / (int)mRange;
				setTrackPosition(wid, pos, 0, ((int)mEndPosition * (getClientWidth() - mTrackMin) / (int)mRange) - pos + mTrackMin, getClientHeight());
			}

			return;
		}

		// сначала проверяем виджеты для трека
		int width = getClientWidth() - mTrackWidth + mTrackStep;
		int count = width / mTrackStep;
		int ost = (width % mTrackStep);
		if (ost > 0)
		{
			width += mTrackStep - ost;
			count ++;
		}

		while ((int)mVectorTrack.size() < count)
		{
			Widget* widget = mClient->createWidget<Widget>(mTrackSkin, IntCoord(/*(int)mVectorTrack.size() * mTrackStep, 0, mTrackWidth, getClientHeight()*/), Align::Left | Align::VStretch);
			widget->setVisible(false);
			mVectorTrack.push_back(widget);
		}

		// все видно
		if ((0 == mStartPosition) && (mRange == mEndPosition))
		{
			int pos = 0;
			for (VectorWidgetPtr::iterator iter = mVectorTrack.begin(); iter != mVectorTrack.end(); ++iter)
			{
				(*iter)->setAlpha(ALPHA_MAX);
				(*iter)->setVisible(true);
				setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
				pos++;
			}
		}
		// эх, придется считать
		else
		{
			// сколько не видно
			int hide_pix = (width * (int)mStartPosition / (int)mRange);
			int hide_count = hide_pix / mTrackStep;
			// сколько видно
			int show_pix = (width * (int)mEndPosition / (int)mRange);
			int show_count = show_pix / mTrackStep;

			int pos = 0;
			for (VectorWidgetPtr::iterator iter = mVectorTrack.begin(); iter != mVectorTrack.end(); ++iter)
			{
				if (0 > show_count)
				{
					(*iter)->setVisible(false);
				}
				else if (0 == show_count)
				{
					(*iter)->setAlpha((float)(show_pix % mTrackStep) / (float)mTrackStep);
					(*iter)->setVisible(true);
					setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
				}
				else
				{
					if (0 < hide_count)
					{
						(*iter)->setVisible(false);
					}
					else if (0 == hide_count)
					{
						(*iter)->setAlpha(1.0f - ((float)(hide_pix % mTrackStep) / (float)mTrackStep));
						(*iter)->setVisible(true);
						setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
					}
					else
					{
						(*iter)->setAlpha(ALPHA_MAX);
						(*iter)->setVisible(true);
						setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
					}
				}
				hide_count --;
				show_count --;
				pos ++;
			}
		}
	}

	void ProgressBar::setTrackPosition(Widget* _widget, int _left, int _top, int _width, int _height)
	{
		if (mFlowDirection == FlowDirection::LeftToRight) _widget->setCoord(_left, _top, _width, _height);
		else if (mFlowDirection == FlowDirection::RightToLeft) _widget->setCoord(mClient->getWidth() - _left - _width, _top, _width, _height);
		else if (mFlowDirection == FlowDirection::TopToBottom) _widget->setCoord(_top, _left, _height, _width);
		else if (mFlowDirection == FlowDirection::BottomToTop) _widget->setCoord(_top, mClient->getHeight() - _left - _width, _height, _width);
	}

	int ProgressBar::getClientWidth()
	{
		return mFlowDirection.isHorizontal() ? mClient->getWidth() : mClient->getHeight();
	}

	int ProgressBar::getClientHeight()
	{
		return mFlowDirection.isHorizontal() ? mClient->getHeight() : mClient->getWidth();
	}

	void ProgressBar::setFlowDirection(FlowDirection _value)
	{
		mFlowDirection = _value;
		updateTrack();
	}

	void ProgressBar::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{ProgressBar, Range, size_t} Диапазон прогресса.
		if (_key == "Range")
			setProgressRange(utility::parseValue<size_t>(_value));

		/// @wproperty{ProgressBar, RangePosition, size_t} Позиция в прогрессе.
		else if (_key == "RangePosition")
			setProgressPosition(utility::parseValue<size_t>(_value));

		/// @wproperty{ProgressBar, AutoTrack, bool} Режим занятости прогресса, в котором трекер просто ходит по кругу.
		else if (_key == "AutoTrack")
			setProgressAutoTrack(utility::parseValue<bool>(_value));

		/// @wproperty{ProgressBar, FlowDirection, FlowDirection} Напревление движения трекера.
		else if (_key == "FlowDirection")
			setFlowDirection(utility::parseValue<FlowDirection>(_value));

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	size_t ProgressBar::getProgressRange() const
	{
		return mRange;
	}

	size_t ProgressBar::getProgressPosition() const
	{
		return mEndPosition;
	}

	bool ProgressBar::getProgressAutoTrack() const
	{
		return mAutoTrack;
	}

	FlowDirection ProgressBar::getFlowDirection() const
	{
		return mFlowDirection;
	}

	void ProgressBar::setPosition(int _left, int _top)
	{
		setPosition(IntPoint(_left, _top));
	}

	void ProgressBar::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void ProgressBar::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
	}

} // namespace MyGUI
