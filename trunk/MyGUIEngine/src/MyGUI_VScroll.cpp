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

	VScroll::VScroll(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name)
	{
		// при нуле, будет игнорировать кнопки
		mScrollPage = 1;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
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
		}
		MYGUI_ASSERT(null != mWidgetStart, "Child Button Start not found in skin (VScroll must have Start)");
		MYGUI_ASSERT(null != mWidgetEnd, "Child Button End not found in skin (VScroll must have End)");
		MYGUI_ASSERT(null != mWidgetTrack, "Child Button Track not found in skin (VScroll must have Track)");

		// парсим свойства
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("SkinTrackRange");
		if (iter != param.end()) {
			IntSize range = IntSize::parse(iter->second);
			mSkinRangeStart = range.width;
			mSkinRangeEnd = range.height;
		}
	}

	void VScroll::updateTrack()
	{
		// размер диапазана в пикселях
		int pos = mCoord.height - (int)(mSkinRangeStart + mSkinRangeEnd);

		// скрываем если диапазан маленький или места мало
		if ((mScrollRange < 2) || (1 > (int)pos)) {
			mWidgetTrack->hide();
			return;
		}
		// если скрыт то покажем
		if (false == mWidgetTrack->isShow()) mWidgetTrack->show();

		// и обновляем позицию
		pos = (int)(((size_t)pos * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart);
		if (mWidgetTrack->getTop() != pos) mWidgetTrack->setPosition(mWidgetTrack->getLeft(), pos);
	}

	void VScroll::TrackMove(int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		// расчитываем позицию виджета
		int start = mPreActionRect.top + (_top - point.top);
		if (start < (int)mSkinRangeStart) start = (int)mSkinRangeStart;
		else if (start > (mCoord.height - (int)mSkinRangeEnd)) start = (mCoord.height - (int)mSkinRangeEnd);
		if (mWidgetTrack->getTop() != start) mWidgetTrack->setPosition(mWidgetTrack->getLeft(), start);

		// расчитываем положение соответствующее позиции
		// плюс пол позиции
		int pos = start - (int)mSkinRangeStart + (mCoord.height - (int)(mSkinRangeStart + mSkinRangeEnd)) / (((int)mScrollRange-1) * 2);
		// высчитываем ближайшее значение и обновляем
		pos = pos * (int)(mScrollRange-1) / (mCoord.height - (int)(mSkinRangeStart + mSkinRangeEnd));

		// проверяем на выходы и изменения
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		// отсылаем событие
		eventScrollChangePosition(this, (int)mScrollPosition);
	}

	void VScroll::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
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

	void VScroll::notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left)
	{
		updateTrack();
	}

	void VScroll::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		TrackMove(_left, _top);
	}

	void VScroll::setScrollRange(size_t _range)
	{
		if (_range == mScrollRange) return;
		mScrollRange = _range;
		mScrollPosition = 0;
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

} // namespace MyGUI
