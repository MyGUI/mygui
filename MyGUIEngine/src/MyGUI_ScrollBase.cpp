/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_ScrollBase.h"

namespace MyGUI
{

	ScrollBase::ScrollBase(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mWidgetStart(null), mWidgetEnd(null), mWidgetTrack(null),
		mScrollPosition(0), mScrollRange(0)
	{

		// при нуле, будет игнорировать кнопки
		mScrollPage = 1;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->getInternalString() == "Start") {
				mWidgetStart = castWidget(Button, *iter);
				mWidgetStart->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);
			}
			else if ((*iter)->getInternalString() == "End") {
				mWidgetEnd = castWidget(Button, *iter);
				mWidgetEnd->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);
			}
			else if ((*iter)->getInternalString() == "Track") {
				mWidgetTrack = castWidget(Button, *iter);
				mWidgetTrack->eventMouseMove = newDelegate(this, &ScrollBase::notifyMouseMove);
				mWidgetTrack->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);
				mWidgetTrack->eventMouseButtonReleased = newDelegate(this, &ScrollBase::notifyMouseReleased);
				mWidgetTrack->hide();
			}
		}
		MYGUI_ASSERT(null != mWidgetStart);
		MYGUI_ASSERT(null != mWidgetEnd);
		MYGUI_ASSERT(null != mWidgetTrack);

		// парсим свойства
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("SkinTrackRange");
		if (iter != param.end()) {
			IntSize range = IntSize::parse(iter->second);
			mSkinRangeStart = range.width;
			mSkinRangeEnd = range.height;
		}

	}

	void ScrollBase::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (false == _left) return;

		// диспечерезируем нажатие своих детей как свое
		eventMouseButtonPressed(this, _left);

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

		} else {
			mPreActionRect.left = _sender->getLeft();
			mPreActionRect.top = _sender->getTop();
		}
	}

	void ScrollBase::notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left)
	{
		updateTrack();
	}

	void ScrollBase::notifyMouseMove(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		notifyTrackMove(_left, _top);
	}

	void ScrollBase::setScrollRange(size_t _range)
	{
		if (_range == mScrollRange) return;
		mScrollRange = _range;
		mScrollPosition = 0;
		updateTrack();
	}

	void ScrollBase::setScrollPosition(size_t _position)
	{
		if (_position == mScrollPosition) return;
		if (_position >= mScrollRange) _position = 0;
		mScrollPosition = _position;
		updateTrack();
	}

	void ScrollBase::setSize(const IntSize& _size)
	{
		Widget::setSize(_size);
		// обновляем трек
		updateTrack();
	}

	void ScrollBase::setPosition(const IntCoord& _coord)
	{
		Widget::setPosition(_coord);
		// обновляем трек
		updateTrack();
	}

} // namespace MyGUI