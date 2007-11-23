
#include "HScroll.h"
#include "InputManager.h"

namespace MyGUI
{

	HScroll::HScroll(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		ScrollBase(_x, _y, _cx, _cy, _align, _info, _parent, _name)
	{
	}

	void HScroll::updateTrack()
	{
		// размер диапазана в пикселях
		int pos = m_cx - (int)(mSkinRangeStart + mSkinRangeEnd);

		// скрываем если диапазан маленький или места мало
		if ((mScrollRange < 2) || (1 > (int)pos)) {
			mWidgetTrack->show(false);
			return;
		}
		// если скрыт то покажем
		if (!mWidgetTrack->isShow()) mWidgetTrack->show(true);

		// и обновляем позицию
		pos = (int)(((size_t)pos * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart);
		if (mWidgetTrack->left() != (int)pos) mWidgetTrack->move((int)pos, mWidgetTrack->top());

	}

	void HScroll::notifyTrackMove(int _x, int _y)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		// расчитываем позицию виджета
		int start = m_preActionRect.left + (_x - point.left);
		if (start < (int)mSkinRangeStart) start = (int)mSkinRangeStart;
		else if (start > (m_cx - (int)mSkinRangeEnd)) start = (m_cx - (int)mSkinRangeEnd);
		if (mWidgetTrack->left() != start) mWidgetTrack->move(start, mWidgetTrack->top());

		// расчитываем положение соответствующее позиции
		// плюс пол позиции
		int pos = start - (int)mSkinRangeStart + (m_cx - (int)(mSkinRangeStart + mSkinRangeEnd)) / (((int)mScrollRange-1) * 2);
		// высчитываем ближайшее значение и обновляем
		pos = pos * (int)(mScrollRange-1) / (m_cx - (int)(mSkinRangeStart + mSkinRangeEnd));

		// проверяем на выходы и изменения
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		// отсылаем событие
		eventScrollChangePosition(this, (int)mScrollPosition);
	}

} // namespace MyGUI
