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

	Ogre::String HScroll::WidgetTypeName = "HScroll";

	HScroll::HScroll(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		VScroll(_coord, _align, _info, _parent, _creator, _name)
	{
		// ������ ��������
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("TrackRangeMargins");
		if (iter != param.end())
		{
			mSkinRangeEnd += mWidgetTrack->getWidth() - mWidgetTrack->getHeight();
		}
		else
		{
			mSkinRangeStart = 0;
			mSkinRangeEnd = mWidgetTrack->getWidth();
		}
	}

	void HScroll::updateTrack()
	{
		_forcePeek(mWidgetTrack);
		// ������ ��������� � ��������
		int pos = mCoord.width - (int)(mSkinRangeStart + mSkinRangeEnd);

		// �������� ���� �������� ��������� ��� ����� ����
		if ((mScrollRange < 2) || (1 > (int)pos)) {
			mWidgetTrack->hide();
			return;
		}
		// ���� ����� �� �������
		if (false == mWidgetTrack->isShow()) mWidgetTrack->show();

		// � ��������� �������
		pos = (int)(((size_t)pos * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart);
		if (mWidgetTrack->getLeft() != (int)pos)
		{
			mWidgetTrack->setPosition(pos, mWidgetTrack->getTop());
			if ( null != mWidgetFirstPart)
			{
				int width = pos + mWidgetTrack->getWidth()/2 - mWidgetFirstPart->getLeft();
				mWidgetFirstPart->setSize(width, mWidgetFirstPart->getHeight());
			}
			if ( null != mWidgetSecondPart)
			{
				int left = pos + mWidgetTrack->getWidth()/2;
				int width = mWidgetSecondPart->getWidth() + mWidgetSecondPart->getLeft() - left;
				mWidgetSecondPart->setPosition(left, mWidgetSecondPart->getTop(), width, mWidgetSecondPart->getHeight());
			}
		}

	}

	void HScroll::TrackMove(int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		// ����������� ������� �������
		int start = mPreActionRect.left + (_left - point.left);
		if (start < (int)mSkinRangeStart) start = (int)mSkinRangeStart;
		else if (start > (mCoord.width - (int)mSkinRangeEnd)) start = (mCoord.width - (int)mSkinRangeEnd);
		if (mWidgetTrack->getLeft() != start) mWidgetTrack->setPosition(IntPoint(start, mWidgetTrack->getTop()));

		// ����������� ��������� ��������������� �������
		// ���� ��� �������
		int pos = start - (int)mSkinRangeStart + (mCoord.width - (int)(mSkinRangeStart + mSkinRangeEnd)) / (((int)mScrollRange-1) * 2);
		// ����������� ��������� �������� � ���������
		pos = pos * (int)(mScrollRange-1) / (mCoord.width - (int)(mSkinRangeStart + mSkinRangeEnd));

		// ��������� �� ������ � ���������
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		// �������� �������
		eventScrollChangePosition(this, (int)mScrollPosition);
	}

} // namespace MyGUI
