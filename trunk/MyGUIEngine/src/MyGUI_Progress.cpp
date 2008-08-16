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
#include "MyGUI_SkinManager.h"

namespace MyGUI
{

	Ogre::String Progress::WidgetTypeName = "Progress";

	const size_t PROGRESS_AUTO_WIDTH = 200;
	const size_t PROGRESS_AUTO_RANGE = 1000;
	const float PROGRESS_AUTO_COEF = 400;

	Progress::Progress(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mTrackWidth(1),
		mTrackStep(0),
		mRange(0), mEndPosition(0), mStartPosition(0),
		mAutoTrack(false),
		mFillTrack(false),
		mStartPoint(ALIGN_LEFT)
	{
		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				MYGUI_DEBUG_ASSERT( ! mWidgetClient, "widget already assigned");
				mWidgetClient = (*iter);
			}
		}
		if (null == mWidgetClient) mWidgetClient = this;

		const MapString & param = _info->getParams();
		MapString::const_iterator iterS = param.find("TrackSkin");
		if (iterS != param.end()) mTrackSkin = iterS->second;
		iterS = param.find("TrackWidth");
		if (iterS != param.end()) mTrackWidth = utility::parseInt(iterS->second);
		if (1 > mTrackWidth) mTrackWidth = 1;
		iterS = param.find("TrackStep");
		if (iterS != param.end()) mTrackStep = utility::parseInt(iterS->second);
		else mTrackStep = mTrackWidth;
		iterS = param.find("TrackFill");
		if (iterS != param.end()) mFillTrack = utility::parseBool(iterS->second);
		iterS = param.find("StartPoint");
		if (iterS != param.end()) setProgressStartPoint(SkinManager::parseAlign(iterS->second));

	}

	Progress::~Progress()
	{
		//Gui::getInstance().removeFrameListener(newDelegate(this, &Progress::frameEntered));
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
			Gui::getInstance().eventFrameStart += newDelegate(this, &Progress::frameEntered);
			//Gui::getInstance().addFrameListener(newDelegate(this, &Progress::frameEntered), this);
			mRange = PROGRESS_AUTO_RANGE;
			mEndPosition = mStartPosition = 0;
			mAutoPosition = 0.0f;
		}
		else {
			Gui::getInstance().eventFrameStart -= newDelegate(this, &Progress::frameEntered);
			//Gui::getInstance().removeFrameListener(newDelegate(this, &Progress::frameEntered));
			mRange = mEndPosition = mStartPosition = 0;
		}
		updateTrack();
	}

	void Progress::frameEntered(float _time)
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

	void Progress::setProgressFillTrack(bool _fill)
	{
		mFillTrack = _fill;
		updateTrack();
	}

	void Progress::updateTrack()
	{
		// все скрыто
		if ((0 == mRange) || (0 == mEndPosition)) {
			for (VectorWidgetPtr::iterator iter=mVectorTrack.begin(); iter!=mVectorTrack.end(); ++iter) {
				(*iter)->hide();
			}
			return;
		}

		// тут попроще расчеты
		if (mFillTrack) {

			if (mVectorTrack.empty()) {
				WidgetPtr widget = mWidgetClient->createWidget<Widget>(mTrackSkin, IntCoord(/*(int)mVectorTrack.size() * mTrackStep, 0, mTrackWidth, getClientHeight()*/), ALIGN_LEFT | ALIGN_VSTRETCH);
				mVectorTrack.push_back(widget);
			}
			else {
				// первый показываем и ставим норм альфу
				VectorWidgetPtr::iterator iter=mVectorTrack.begin();
				(*iter)->show();
				(*iter)->setAlpha(ALPHA_MAX);

				// все начиная со второго скрываем
				++iter;
				for (; iter!=mVectorTrack.end(); ++iter) {
					(*iter)->hide();
				}
			}

			WidgetPtr wid = mVectorTrack.front();

			// полностью виден
			if ((0 == mStartPosition) && (mRange == mEndPosition)) {
				setTrackPosition(wid, 0, 0, getClientWidth(), getClientHeight());
			}
			// эх
			else {
				int pos = (int)mStartPosition * getClientWidth() / (int)mRange;
				setTrackPosition(wid, pos, 0, ((int)mEndPosition * getClientWidth() / (int)mRange) - pos, getClientHeight());
			}

			return;
		}

		// сначала проверяем виджеты для трека
		int width = getClientWidth();
		int count = width / mTrackStep;
		int ost = (width % mTrackStep);
		if (ost > 0) {
			width += mTrackStep - ost;
			count ++;
		}

		while ((int)mVectorTrack.size() < count) {
			WidgetPtr widget = mWidgetClient->createWidget<Widget>(mTrackSkin, IntCoord(/*(int)mVectorTrack.size() * mTrackStep, 0, mTrackWidth, getClientHeight()*/), ALIGN_LEFT | ALIGN_VSTRETCH);
			widget->hide();
			mVectorTrack.push_back(widget);
		}

		// все видно
		if ((0 == mStartPosition) && (mRange == mEndPosition)) {
			int pos = 0;
			for (VectorWidgetPtr::iterator iter=mVectorTrack.begin(); iter!=mVectorTrack.end(); ++iter) {
				(*iter)->setAlpha(ALPHA_MAX);
				(*iter)->show();
				setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
				pos++;
			}
		}
		// эх, придется считать
		else {
			// сколько не видно
			int hide_pix = (width * (int)mStartPosition / (int)mRange);
			int hide = hide_pix / mTrackStep;
			// сколько видно
			int show_pix = (width * (int)mEndPosition / (int)mRange);
			int show = show_pix / mTrackStep;

			int pos = 0;
			for (VectorWidgetPtr::iterator iter=mVectorTrack.begin(); iter!=mVectorTrack.end(); ++iter) {
				if (0 > show) {
					(*iter)->hide();
				}
				else if (0 == show) {
					(*iter)->setAlpha((float)(show_pix % mTrackStep) / (float)mTrackStep);
					(*iter)->show();
					setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
				}
				else {
					if (0 < hide) {
						(*iter)->hide();
					}
					else if (0 == hide) {
						(*iter)->setAlpha(1.0f - ((float)(hide_pix % mTrackStep) / (float)mTrackStep));
						(*iter)->show();
						setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
					}
					else {
						(*iter)->setAlpha(ALPHA_MAX);
						(*iter)->show();
						setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
					}
				}
				hide --;
				show --;
				pos ++;
			}
		}
	}

	void Progress::setTrackPosition(WidgetPtr _widget, int _left, int _top, int _width, int _height)
	{
		if (IS_ALIGN_LEFT(mStartPoint)) _widget->setPosition(_left, _top, _width, _height);
		else if (IS_ALIGN_RIGHT(mStartPoint)) _widget->setPosition(mWidgetClient->getWidth() - _left - _width, _top, _width, _height);
		else if (IS_ALIGN_TOP(mStartPoint)) _widget->setPosition(_top, _left, _height, _width);
		else if (IS_ALIGN_BOTTOM(mStartPoint)) _widget->setPosition(_top, mWidgetClient->getHeight() - _left - _width, _height, _width);
	}

	void Progress::setProgressStartPoint(Align _align)
	{
		if ((_align == ALIGN_LEFT) || (_align == ALIGN_RIGHT) || (_align == ALIGN_TOP) || (_align == ALIGN_BOTTOM)) {
			mStartPoint = _align;
		}
		else {
			mStartPoint = ALIGN_LEFT;
			MYGUI_LOG(Warning, "Progress bar support only ALIGN_LEFT, ALIGN_RIGHT, ALIGN_TOP or ALIGN_BOTTOM align values");
		}
		updateTrack();
	}

} // namespace MyGUI
