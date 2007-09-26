
#include "HScroll.h"

namespace MyGUI
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<HScroll> HScrollFactoryInstance("HScroll"); }

	HScroll::HScroll(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		ScrollBase(_x, _y, _cx, _cy, _align, _info, _parent, _name)
	{
	}

	void HScroll::updateTrack()
	{
		// размер диапазана в пикселях
		size_t pos = m_cx - (mSkinRangeStart + mSkinRangeEnd);

		// скрываем если диапазан маленький или места мало
		if ((mScrollRange < 2) || (mWidgetTrack->width() > (int)pos)) {
			mWidgetTrack->show(false);
			return;
		}
		// если скрыт то покажем
		if (!mWidgetTrack->isShow()) mWidgetTrack->show(true);

		// и обновляем позицию
		pos = (pos * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart;
		if (mWidgetTrack->left() != (int)pos) mWidgetTrack->move((int)pos, mWidgetTrack->top());

	}

	void HScroll::notifyTrackMove(int _x, int _y)
	{
		const intPoint & point = InputManager::getInstance().getLastLeftPressed();
		int pos = m_preActionRect.left + (_x - point.left) - (int)mSkinRangeStart;

		// плюс пол позиции
		pos += (m_cx - (int)(mSkinRangeStart + mSkinRangeEnd)) / (((int)mScrollRange-1) * 2);
		// высчитываем ближайшее значение и обновляем
		pos = pos * (int)(mScrollRange-1) / (m_cx - (int)(mSkinRangeStart + mSkinRangeEnd));

		// проверяем на выходы и изменения
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		updateTrack();
	}

} // namespace MyGUI
