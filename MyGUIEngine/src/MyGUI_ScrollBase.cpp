/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_ScrollBase.h"

namespace MyGUI
{

	ScrollBase::ScrollBase(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_left, _top, _width, _height, _align, _info, _parent, _name),
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
		mWidgetTrack->show(false);

		MapString::const_iterator iter = param.find("SkinTrackRange");
		if (iter != param.end()) {
			IntSize range = util::parseIntSize(iter->second);
			mSkinRangeStart = range.width;
			mSkinRangeEnd = range.height;
		}

	}

	void ScrollBase::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (!_left) return;

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

	void ScrollBase::size(int _width, int _height)
	{
		Widget::size(_width, _height);
		// ��������� ����
		updateTrack();
	}

	void ScrollBase::move(int _left, int _top, int _width, int _height)
	{
		Widget::move(_left, _top, _width, _height);
		// ��������� ����
		updateTrack();
	}

} // namespace MyGUI