/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_VScroll.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_CastWidget.h"
#include "MyGUI_Button.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{
	Ogre::String VScroll::WidgetTypeName = "VScroll";

	VScroll::VScroll(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mWidgetStart(null),
		mWidgetEnd(null),
		mWidgetTrack(null),
		mWidgetFirstPart(null),
		mWidgetSecondPart(null)
	{
		// при нуле, будет игнорировать кнопки
		mScrollPage = 1;
		mScrollViewPage = 1;

		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Start") {
				mWidgetStart = castWidget<Button>(*iter);
				mWidgetStart->eventMouseButtonPressed = newDelegate(this, &VScroll::notifyMousePressed);
			}
			else if ((*iter)->_getInternalString() == "End") {
				mWidgetEnd = castWidget<Button>(*iter);
				mWidgetEnd->eventMouseButtonPressed = newDelegate(this, &VScroll::notifyMousePressed);
			}
			else if ((*iter)->_getInternalString() == "Track") {
				mWidgetTrack = castWidget<Button>(*iter);
				mWidgetTrack->eventMouseDrag = newDelegate(this, &VScroll::notifyMouseDrag);
				mWidgetTrack->eventMouseButtonPressed = newDelegate(this, &VScroll::notifyMousePressed);
				mWidgetTrack->eventMouseButtonReleased = newDelegate(this, &VScroll::notifyMouseReleased);
				mWidgetTrack->hide();
			}
			else if ((*iter)->_getInternalString() == "FirstPart") {
				mWidgetFirstPart = castWidget<Button>(*iter);
				mWidgetFirstPart->eventMouseButtonPressed = newDelegate(this, &VScroll::notifyMousePressed);
			}
			else if ((*iter)->_getInternalString() == "SecondPart") {
				mWidgetSecondPart = castWidget<Button>(*iter);
				mWidgetSecondPart->eventMouseButtonPressed = newDelegate(this, &VScroll::notifyMousePressed);
			}
		}

		// slider don't have buttons
		//MYGUI_ASSERT(null != mWidgetStart, "Child Button Start not found in skin (Scroll must have Start)");
		//MYGUI_ASSERT(null != mWidgetEnd, "Child Button End not found in skin (Scroll must have End)");
		MYGUI_ASSERT(null != mWidgetTrack, "Child Button Track not found in skin (Scroll must have Track)");

		// парсим свойства
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("TrackRangeMargins");
		if (iter != param.end())
		{
			IntSize range = IntSize::parse(iter->second);
			mSkinRangeStart = range.width;
			mSkinRangeEnd = range.height;
		}
		else
		{
			mSkinRangeStart = 0;
			mSkinRangeEnd = 0;
		}
		iter = param.find("MinTrackSize");
		if (iter != param.end()) mMinTrackSize = utility::parseInt(iter->second);
		else mMinTrackSize = 0;
	}

	void VScroll::updateTrack()
	{
		_forcePeek(mWidgetTrack);
		// размер диапазана в пикселях
		int pos = getLineSize();

		// скрываем если диапазан маленький или места мало
		if ((mScrollRange < 2) || (pos <= mWidgetTrack->getHeight())) {
			mWidgetTrack->hide();
			if ( null != mWidgetFirstPart ) mWidgetFirstPart->setSize(mWidgetFirstPart->getWidth(), pos/2);
			if ( null != mWidgetSecondPart ) mWidgetSecondPart->setPosition(mWidgetSecondPart->getLeft(), pos/2 + mSkinRangeStart, mWidgetSecondPart->getWidth(), pos - pos/2);
			if ( pos < 0 )
			{
				if ( null != mWidgetStart ) mWidgetStart->setSize(mWidgetStart->getWidth(), mSkinRangeStart + pos/2);
				if ( null != mWidgetEnd ) mWidgetEnd->setPosition(mWidgetEnd->getLeft(), pos/2 + mSkinRangeStart, mWidgetEnd->getWidth(), mCoord.height - (pos/2 + mSkinRangeStart));
			}
			else
			{
				if ( null != mWidgetStart ) mWidgetStart->setSize(mWidgetStart->getWidth(), mSkinRangeStart);
				if ( null != mWidgetEnd ) mWidgetEnd->setPosition(mWidgetEnd->getLeft(), mCoord.height - mSkinRangeEnd, mWidgetEnd->getWidth(), mSkinRangeEnd);
			}
			return;
		}
		// если скрыт то покажем
		if (false == mWidgetTrack->isShow())
		{
			mWidgetTrack->show();
			if ( null != mWidgetStart ) mWidgetStart->setSize(mWidgetStart->getWidth(), mSkinRangeStart);
			if ( null != mWidgetEnd ) mWidgetEnd->setPosition(mWidgetEnd->getLeft(), mCoord.height - mSkinRangeEnd, mWidgetEnd->getWidth(), mSkinRangeEnd);
		}

		// и обновляем позицию
		pos = (int)(((size_t)(pos-getTrackSize()) * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart);

		mWidgetTrack->setPosition(mWidgetTrack->getLeft(), pos);
		if ( null != mWidgetFirstPart )
		{
			int height = pos + mWidgetTrack->getHeight()/2 - mWidgetFirstPart->getTop();
			mWidgetFirstPart->setSize(mWidgetFirstPart->getWidth(), height);
		}
		if ( null != mWidgetSecondPart )
		{
			int top = pos + mWidgetTrack->getHeight()/2;
			int height = mWidgetSecondPart->getHeight() + mWidgetSecondPart->getTop() - top;
			mWidgetSecondPart->setPosition(mWidgetSecondPart->getLeft(), top, mWidgetSecondPart->getWidth(), height);
		}
	}

	void VScroll::TrackMove(int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		// расчитываем позицию виджета
		int start = mPreActionRect.top + (_top - point.top);
		if (start < (int)mSkinRangeStart) start = (int)mSkinRangeStart;
		else if (start > (mCoord.height - (int)mSkinRangeEnd - mWidgetTrack->getHeight())) start = (mCoord.height - (int)mSkinRangeEnd - mWidgetTrack->getHeight());
		if (mWidgetTrack->getTop() != start) mWidgetTrack->setPosition(mWidgetTrack->getLeft(), start);

		// расчитываем положение соответствующее позиции
		// плюс пол позиции
		int pos = start - (int)mSkinRangeStart + (getLineSize() - getTrackSize()) / (((int)mScrollRange-1) * 2);
		// высчитываем ближайшее значение и обновляем
		pos = pos * (int)(mScrollRange-1) / (getLineSize() - getTrackSize());

		// проверяем на выходы и изменения
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		// отсылаем событие
		eventScrollChangePosition(this, (int)mScrollPosition);
	}

	void VScroll::notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		if (MB_Left != _id) return;

		// диспечерезируем нажатие своих детей как свое
		eventMouseButtonPressed(this, _left, _top, _id);

		if (_sender == mWidgetStart) {
			// минимальное значение
			if (mScrollPosition == 0) return;

			// расчитываем следующее положение
			if (mScrollPosition > mScrollPage) mScrollPosition -= mScrollPage;
			else mScrollPosition = 0;

			// оповещаем
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();

		} else if (_sender == mWidgetEnd){
			// максимальное значение
			if ( (mScrollRange < 2) || (mScrollPosition >= (mScrollRange-1)) ) return;

			// расчитываем следующее положение
			if ((mScrollPosition + mScrollPage) < (mScrollRange-1)) mScrollPosition += mScrollPage;
			else mScrollPosition = mScrollRange - 1;

			// оповещаем
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();

		} else if (_sender == mWidgetFirstPart) {
			// минимальное значение
			if (mScrollPosition == 0) return;

			// расчитываем следующее положение
			if (mScrollPosition > mScrollPage) mScrollPosition -= mScrollViewPage;
			else mScrollPosition = 0;

			// оповещаем
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();

		} else if (_sender == mWidgetSecondPart){
			// максимальное значение
			if ( (mScrollRange < 2) || (mScrollPosition >= (mScrollRange-1)) ) return;

			// расчитываем следующее положение
			if ((mScrollPosition + mScrollPage) < (mScrollRange-1)) mScrollPosition += mScrollViewPage;
			else mScrollPosition = mScrollRange - 1;

			// оповещаем
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();

		} else {
			mPreActionRect.left = _sender->getLeft();
			mPreActionRect.top = _sender->getTop();
		}
	}

	void VScroll::notifyMouseReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		updateTrack();
	}

	void VScroll::notifyMouseDrag(WidgetPtr _sender, int _left, int _top)
	{
		TrackMove(_left, _top);
	}

	void VScroll::setScrollRange(size_t _range)
	{
		if (_range == mScrollRange) return;
		mScrollRange = _range;
		mScrollPosition = (mScrollPosition < mScrollRange) ? mScrollPosition : 0;
		updateTrack();
	}

	void VScroll::setScrollPosition(size_t _position)
	{
		if (_position == mScrollPosition) return;
		if (_position >= mScrollRange) _position = 0;
		mScrollPosition = _position;
		updateTrack();
	}

	void VScroll::setSize(const IntSize& _size)
	{
		Widget::setSize(_size);
		// обновляем трек
		updateTrack();
	}

	void VScroll::setPosition(const IntCoord& _coord)
	{
		Widget::setPosition(_coord);
		// обновляем трек
		updateTrack();
	}

	void VScroll::setTrackSize(size_t _size)
	{
		mWidgetTrack->setSize(mWidgetTrack->getWidth(), ((int)_size < (int)mMinTrackSize)? (int)mMinTrackSize : (int)_size);
		updateTrack();
	}

	int VScroll::getTrackSize()
	{
		return mWidgetTrack->getHeight();
	}

	int VScroll::getLineSize()
	{
		return mCoord.height - (int)(mSkinRangeStart + mSkinRangeEnd);
	}

} // namespace MyGUI
