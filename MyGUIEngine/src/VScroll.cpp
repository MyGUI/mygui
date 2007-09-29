
#include "VScroll.h"

namespace MyGUI
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<VScroll> VScrollFactoryInstance("VScroll"); }

	VScroll::VScroll(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		ScrollBase(_x, _y, _cx, _cy, _align, _info, _parent, _name)
	{
	}

	void VScroll::updateTrack()
	{
		// размер диапазана в пикселях
		size_t pos = m_cy - (mSkinRangeStart + mSkinRangeEnd);

		// скрываем если диапазан маленький или места мало
		if ((mScrollRange < 2) || (mWidgetTrack->height() > (int)pos)) {
			mWidgetTrack->show(false);
			return;
		}
		// если скрыт то покажем
		if (!mWidgetTrack->isShow()) mWidgetTrack->show(true);

		// и обновляем позицию
		pos = (pos * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart;
		if (mWidgetTrack->top() != (int)pos) mWidgetTrack->move(mWidgetTrack->left(), (int)pos);
	}

	void VScroll::notifyTrackMove(int _x, int _y)
	{
		const intPoint & point = InputManager::getInstance().getLastLeftPressed();

		// расчитываем позицию виджета
		int start = m_preActionRect.top + (_y - point.top);
		if (start < (int)mSkinRangeStart) start = (int)mSkinRangeStart;
		else if (start > (m_cy - (int)mSkinRangeEnd)) start = (m_cy - (int)mSkinRangeEnd);
		if (mWidgetTrack->top() != start) mWidgetTrack->move(mWidgetTrack->left(), start);

		// расчитываем положение соответствующее позиции
		// плюс пол позиции
		int pos = start - (int)mSkinRangeStart + (m_cy - (int)(mSkinRangeStart + mSkinRangeEnd)) / (((int)mScrollRange-1) * 2);
		// высчитываем ближайшее значение и обновляем
		pos = pos * (int)(mScrollRange-1) / (m_cy - (int)(mSkinRangeStart + mSkinRangeEnd));

		// проверяем на выходы и изменения
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		// отсылаем событие
		eventScrollChangePosition(this, (int)mScrollPosition);
	}

} // namespace MyGUI
