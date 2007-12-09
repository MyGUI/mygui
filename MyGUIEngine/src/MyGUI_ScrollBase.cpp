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

		// �������� ������ �����
		IntSize size = _info->getSize();
		// ��� ����, ����� ������������ ������
		mScrollPage = 1;

		// ������ ��������
		const MapString & param = _info->getParams();

		// ������ ��������� ������
		mWidgetStart = parseSubWidget(param, "Button", "SkinStart", "OffsetStart", "AlignStart", size);
		MYGUI_ASSERT(null != mWidgetStart);
		// �������� ��� �������
		mWidgetStart->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);

		// ������ �������� ������
		mWidgetEnd = parseSubWidget(param, "Button", "SkinEnd", "OffsetEnd", "AlignEnd", size);
		MYGUI_ASSERT(null != mWidgetEnd);
		// �������� ��� �������
		mWidgetEnd->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);

		// ������ ����
		mWidgetTrack = parseSubWidget(param, "Button", "SkinTrack", "OffsetTrack", "AlignTrack", size);
		MYGUI_ASSERT(null != mWidgetTrack);
		// �������� ��� �������
		mWidgetTrack->eventMouseMove = newDelegate(this, &ScrollBase::notifyMouseMove);
		mWidgetTrack->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);
		mWidgetTrack->eventMouseButtonReleased = newDelegate(this, &ScrollBase::notifyMouseReleased);
		mWidgetTrack->hide();

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

		// ��������������� ������� ����� ����� ��� ����
		eventMouseButtonPressed(this, _left);

		if (_sender == mWidgetStart) {
			// ����������� ��������
			if (mScrollPosition == 0) return;

			// ����������� ��������� ���������
			if (mScrollPosition > mScrollPage) mScrollPosition -= mScrollPage;
			else mScrollPosition = 0;

			// ���������
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();

		} else if (_sender == mWidgetEnd){
			// ������������ ��������
			if ( (mScrollRange < 2) || (mScrollPosition >= (mScrollRange-1)) ) return;

			// ����������� ��������� ���������
			if ((mScrollPosition + mScrollPage) < (mScrollRange-1)) mScrollPosition += mScrollPage;
			else mScrollPosition = mScrollRange - 1;

			// ���������
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
//		eventScrollChangePosition(this, (int)mScrollPosition);
		updateTrack();
	}

	void ScrollBase::setSize(const IntSize& _size)
	{
		Widget::setSize(_size);
		// ��������� ����
		updateTrack();
	}

	void ScrollBase::setPosition(const IntCoord& _coord)
	{
		Widget::setPosition(_coord);
		// ��������� ����
		updateTrack();
	}

} // namespace MyGUI