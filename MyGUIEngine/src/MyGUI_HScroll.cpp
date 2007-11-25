/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_HScroll.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	HScroll::HScroll(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		ScrollBase(_left, _top, _width, _height, _align, _info, _parent, _name)
	{
	}

	void HScroll::updateTrack()
	{
		// размер диапазана в пикселях
		int pos = mWidth - (int)(mSkinRangeStart + mSkinRangeEnd);

		// скрываем если диапазан маленький или места мало
		if ((mScrollRange < 2) || (1 > (int)pos)) {
			mWidgetTrack->show(false);
			return;
		}
		// если скрыт то покажем
		if (!mWidgetTrack->isShow()) mWidgetTrack->show(true);

		// и обновляем позицию
		pos = (int)(((size_t)pos * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart);
		if (mWidgetTrack->getLeft() != (int)pos) mWidgetTrack->move((int)pos, mWidgetTrack->getTop());

	}

	void HScroll::notifyTrackMove(int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		// расчитываем позицию виджета
		int start = mPreActionRect.left + (_left - point.left);
		if (start < (int)mSkinRangeStart) start = (int)mSkinRangeStart;
		else if (start > (mWidth - (int)mSkinRangeEnd)) start = (mWidth - (int)mSkinRangeEnd);
		if (mWidgetTrack->getLeft() != start) mWidgetTrack->move(start, mWidgetTrack->getTop());

		// расчитываем положение соответствующее позиции
		// плюс пол позиции
		int pos = start - (int)mSkinRangeStart + (mWidth - (int)(mSkinRangeStart + mSkinRangeEnd)) / (((int)mScrollRange-1) * 2);
		// высчитываем ближайшее значение и обновляем
		pos = pos * (int)(mScrollRange-1) / (mWidth - (int)(mSkinRangeStart + mSkinRangeEnd));

		// проверяем на выходы и изменения
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		// отсылаем событие
		eventScrollChangePosition(this, (int)mScrollPosition);
	}

} // namespace MyGUI
