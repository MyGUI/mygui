/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Progress.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	Ogre::String Progress::WidgetTypeName = "Progress";

	const size_t PROGRESS_AUTO_WIDTH = 200;
	const size_t PROGRESS_AUTO_RANGE = 1000;
	const float PROGRESS_AUTO_COEF = 400;

	Progress::Progress(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mClient(null),
		mTrackWidth(1),
		mRange(0), mEndPosition(0), mStartPosition(0),
		mAutoTrack(false)
	{
		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mClient = (*iter);
			}
		}
		if (null == mClient) mClient = this;

		const MapString & param = _info->getParams();
		MapString::const_iterator iterS = param.find("TrackSkin");
		if (iterS != param.end()) mTrackSkin = iterS->second;
		iterS = param.find("TrackWidth");
		if (iterS != param.end()) mTrackWidth = utility::parseInt(iterS->second);
		if (1 > mTrackWidth) mTrackWidth = 1;

	}

	Progress::~Progress()
	{
		if (mAutoTrack) Gui::getInstance().removeFrameListener(this);
	}

	void Progress::updateTrack()
	{
		// сначала проверяем виджеты для трека
		int width = mClient->getWidth();
		int count = width / mTrackWidth;
		int ost = (width % mTrackWidth);
		if (ost > 0) {
			width += mTrackWidth - ost;
			count ++;
		}

		while ((int)mVectorTrack.size() < count) {
			WidgetPtr widget = mClient->createWidget<Widget>(mTrackSkin, IntCoord((int)mVectorTrack.size() * mTrackWidth, 0, mTrackWidth, mClient->getHeight()), ALIGN_LEFT | ALIGN_VSTRETCH);
			widget->hide();
			mVectorTrack.push_back(widget);
		}

		// все скрыто
		if ((0 == mRange) || (0 == mEndPosition)) {
			for (VectorWidgetPtr::iterator iter=mVectorTrack.begin(); iter!=mVectorTrack.end(); ++iter) {
				(*iter)->hide();
			}
		}
		// все видно
		else if ((0 == mStartPosition) && (mRange == mEndPosition)) {
			for (VectorWidgetPtr::iterator iter=mVectorTrack.begin(); iter!=mVectorTrack.end(); ++iter) {
				(*iter)->setAlpha(ALPHA_MAX);
				(*iter)->show();
			}
		}
		// эх, придется считать
		else {
			// сколько не видно
			int hide_pix = (width * (int)mStartPosition / (int)mRange);
			int hide = hide_pix / mTrackWidth;
			// сколько видно
			int show_pix = (width * (int)mEndPosition / (int)mRange);
			int show = show_pix / mTrackWidth;

			for (VectorWidgetPtr::iterator iter=mVectorTrack.begin(); iter!=mVectorTrack.end(); ++iter) {
				if (0 > show) {
					(*iter)->hide();
				}
				else if (0 == show) {
					(*iter)->setAlpha((float)(show_pix % mTrackWidth) / (float)mTrackWidth);
					(*iter)->show();
				}
				else {
					if (0 < hide) {
						(*iter)->hide();
					}
					else if (0 == hide) {
						(*iter)->setAlpha(1.0f - ((float)(hide_pix % mTrackWidth) / (float)mTrackWidth));
						(*iter)->show();
					}
					else {
						(*iter)->setAlpha(ALPHA_MAX);
						(*iter)->show();
					}
				}
				hide --;
				show --;
			}
		}
	}

	void Progress::setProgressRange(size_t _range)
	{
		if (mAutoTrack) return;
		mRange = _range;
		if (mEndPosition > mRange) mEndPosition = mRange;
		if (mStartPosition > mRange) mStartPosition = mRange;
		updateTrack();
	}

	void Progress::setProgressPosition(size_t _pos)
	{
		if (mAutoTrack) return;
		mEndPosition = _pos;
		if (mEndPosition > mRange) mEndPosition = mRange;
		updateTrack();
	}

	void Progress::setProgressAutoTrack(bool _auto)
	{
		if (mAutoTrack == _auto) return;
		mAutoTrack = _auto;

		if (mAutoTrack) {
			Gui::getInstance().addFrameListener(this);
			mRange = PROGRESS_AUTO_RANGE;
			mEndPosition = mStartPosition = 0;
			mAutoPosition = 0.0f;
		}
		else {
			Gui::getInstance().removeFrameListener(this);
			mRange = mEndPosition = mStartPosition = 0;
		}
		updateTrack();
	}

	void Progress::_frameEntered(float _time)
	{
		if (false == mAutoTrack) return;
		mAutoPosition += (PROGRESS_AUTO_COEF * _time);
		size_t pos = (size_t)mAutoPosition;

		if (pos > (mRange + PROGRESS_AUTO_WIDTH)) mAutoPosition = 0.0f;

		if (pos > mRange) mEndPosition = mRange;
		else mEndPosition = mAutoPosition;

		if (pos < PROGRESS_AUTO_WIDTH) mStartPosition = 0;
		else mStartPosition = pos - PROGRESS_AUTO_WIDTH;

		updateTrack();
	}

	void Progress::setPosition(const IntCoord& _coord)
	{
		updateTrack();
		Widget::setPosition(_coord);
	}

	void Progress::setSize(const IntSize& _size)
	{
		updateTrack();
		Widget::setSize(_size);
	}

} // namespace MyGUI
