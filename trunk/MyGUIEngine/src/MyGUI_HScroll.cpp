/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_HScroll.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Button.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( HScroll, VScroll );

	HScroll::HScroll(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		VScroll(_coord, _align, _info, _parent, _creator, _name)
	{
	}

	void HScroll::updateTrack()
	{
		_forcePeek(mWidgetTrack);
		// ������ ��������� � ��������
		int pos = getLineSize();

		// �������� ���� �������� ��������� ��� ����� ����
		if ((mScrollRange < 2) || (pos <= mWidgetTrack->getWidth())) {
			mWidgetTrack->hide();
			if ( null != mWidgetFirstPart ) mWidgetFirstPart->setSize(pos/2, mWidgetFirstPart->getHeight());
			if ( null != mWidgetSecondPart ) mWidgetSecondPart->setPosition(pos/2 + mSkinRangeStart, mWidgetSecondPart->getTop(), pos - pos/2, mWidgetSecondPart->getHeight());
			if ( pos < 0 )
			{
				if ( null != mWidgetStart ) mWidgetStart->setSize(mSkinRangeStart + pos/2, mWidgetStart->getHeight());
				if ( null != mWidgetEnd ) mWidgetEnd->setPosition(pos/2 + mSkinRangeStart, mWidgetEnd->getTop(), mCoord.height - (pos/2 + mSkinRangeStart), mWidgetEnd->getHeight());
			}
			else
			{
				if ( null != mWidgetStart ) mWidgetStart->setSize(mSkinRangeStart, mWidgetStart->getHeight());
				if ( null != mWidgetEnd ) mWidgetEnd->setPosition(mCoord.width - mSkinRangeEnd, mWidgetEnd->getTop(), mSkinRangeEnd, mWidgetEnd->getHeight());
			}
			return;
		}
		// ���� ����� �� �������
		if (false == mWidgetTrack->isShow())
		{
			mWidgetTrack->show();
			if ( null != mWidgetStart ) mWidgetStart->setSize(mSkinRangeStart, mWidgetStart->getHeight());
			if ( null != mWidgetEnd ) mWidgetEnd->setPosition(mCoord.width - mSkinRangeEnd, mWidgetEnd->getTop(), mSkinRangeEnd, mWidgetEnd->getHeight());
		}

		// � ��������� �������
		pos = (int)(((size_t)(pos-getTrackSize()) * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart);

		mWidgetTrack->setPosition(pos, mWidgetTrack->getTop());
		if ( null != mWidgetFirstPart )
		{
			int height = pos + mWidgetTrack->getWidth()/2 - mWidgetFirstPart->getLeft();
			mWidgetFirstPart->setSize(height, mWidgetFirstPart->getHeight());
		}
		if ( null != mWidgetSecondPart )
		{
			int top = pos + mWidgetTrack->getWidth()/2;
			int height = mWidgetSecondPart->getWidth() + mWidgetSecondPart->getLeft() - top;
			mWidgetSecondPart->setPosition(top, mWidgetSecondPart->getTop(), height, mWidgetSecondPart->getHeight());
		}
	}

	void HScroll::TrackMove(int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		// ����������� ������� �������
		int start = mPreActionRect.left + (_left - point.left);
		if (start < (int)mSkinRangeStart) start = (int)mSkinRangeStart;
		else if (start > (mCoord.width - (int)mSkinRangeEnd - mWidgetTrack->getWidth())) start = (mCoord.width - (int)mSkinRangeEnd - mWidgetTrack->getWidth());
		if (mWidgetTrack->getLeft() != start) mWidgetTrack->setPosition(IntPoint(start, mWidgetTrack->getTop()));

		// ����������� ��������� ��������������� �������
		// ���� ��� �������
		int pos = start - (int)mSkinRangeStart + (getLineSize() - getTrackSize()) / (((int)mScrollRange-1) * 2);
		// ����������� ��������� �������� � ���������
		pos = pos * (int)(mScrollRange-1) / (getLineSize() - getTrackSize());

		// ��������� �� ������ � ���������
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		// �������� �������
		eventScrollChangePosition(this, (int)mScrollPosition);
	}

	void HScroll::setTrackSize(size_t _size)
	{
		mWidgetTrack->setSize(((int)_size < (int)mMinTrackSize)? (int)mMinTrackSize : (int)_size, mWidgetTrack->getHeight());
		updateTrack();
	}

	int HScroll::getTrackSize()
	{
		return mWidgetTrack->getWidth();
	}

	int HScroll::getLineSize()
	{
		return mCoord.width - (int)(mSkinRangeStart + mSkinRangeEnd);
	}

} // namespace MyGUI
