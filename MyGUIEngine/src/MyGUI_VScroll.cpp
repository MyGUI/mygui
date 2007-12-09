/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_VScroll.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	VScroll::VScroll(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		ScrollBase(_coord, _align, _info, _parent, _name)
	{
	}

	void VScroll::updateTrack()
	{
		// ������ ��������� � ��������
		int pos = mCoord.height - (int)(mSkinRangeStart + mSkinRangeEnd);

		// �������� ���� �������� ��������� ��� ����� ����
		if ((mScrollRange < 2) || (1 > (int)pos)) {
			mWidgetTrack->hide();
			return;
		}
		// ���� ����� �� �������
		if (false == mWidgetTrack->isShow()) mWidgetTrack->show();

		// � ��������� �������
		pos = (int)(((size_t)pos * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart);
		if (mWidgetTrack->getTop() != pos) mWidgetTrack->setPosition(mWidgetTrack->getLeft(), pos);
	}

	void VScroll::notifyTrackMove(int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		// ����������� ������� �������
		int start = mPreActionRect.top + (_top - point.top);
		if (start < (int)mSkinRangeStart) start = (int)mSkinRangeStart;
		else if (start > (mCoord.height - (int)mSkinRangeEnd)) start = (mCoord.height - (int)mSkinRangeEnd);
		if (mWidgetTrack->getTop() != start) mWidgetTrack->setPosition(mWidgetTrack->getLeft(), start);

		// ����������� ��������� ��������������� �������
		// ���� ��� �������
		int pos = start - (int)mSkinRangeStart + (mCoord.height - (int)(mSkinRangeStart + mSkinRangeEnd)) / (((int)mScrollRange-1) * 2);
		// ����������� ��������� �������� � ���������
		pos = pos * (int)(mScrollRange-1) / (mCoord.height - (int)(mSkinRangeStart + mSkinRangeEnd));

		// ��������� �� ������ � ���������
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		// �������� �������
		eventScrollChangePosition(this, (int)mScrollPosition);
	}

} // namespace MyGUI
