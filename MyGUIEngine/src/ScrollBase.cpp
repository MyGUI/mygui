
#include "ScrollBase.h"
#include "ScrollParser.h"

namespace MyGUI
{
	// ������ ������
	namespace parser { ScrollParser ScrollParserInstance; }

	ScrollBase::ScrollBase(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		mWidgetStart(null), mWidgetEnd(null), mWidgetTrack(null),
		mScrollPosition(0), mScrollRange(0)
	{

		// �������� ������ �����
		IntSize size = _info->getSize();
		// ��� ����, ����� ������������ ������
		mScrollPage = 1;

		// ������ ��������
		const SkinParam & param = _info->getParams();

		// ������ ��������� ������
		mWidgetStart = parseSubWidget(param, "Button", "SkinStart", "OffsetStart", "AlignStart", size);
		ASSERT(mWidgetStart);
		// �������� ��� �������
		mWidgetStart->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);

		// ������ �������� ������
		mWidgetEnd = parseSubWidget(param, "Button", "SkinEnd", "OffsetEnd", "AlignEnd", size);
		ASSERT(mWidgetEnd);
		// �������� ��� �������
		mWidgetEnd->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);

		// ������ ����
		mWidgetTrack = parseSubWidget(param, "Button", "SkinTrack", "OffsetTrack", "AlignTrack", size);
		ASSERT(mWidgetTrack);
		// �������� ��� �������
		mWidgetTrack->eventMouseMove = newDelegate(this, &ScrollBase::notifyMouseMove);
		mWidgetTrack->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);
		mWidgetTrack->eventMouseButtonReleased = newDelegate(this, &ScrollBase::notifyMouseReleased);
		mWidgetTrack->show(false);

		SkinParam::const_iterator iter = param.find("SkinTrackRange");
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
			m_preActionRect.left = _sender->left();
			m_preActionRect.top = _sender->top();
		}
	}

	void ScrollBase::notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left)
	{
		updateTrack();
	}

	void ScrollBase::notifyMouseMove(MyGUI::WidgetPtr _sender, int _x, int _y)
	{
		notifyTrackMove(_x, _y);
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

	void ScrollBase::size(int _cx, int _cy)
	{
		Widget::size(_cx, _cy);
		// ��������� ����
		updateTrack();
	}

	void ScrollBase::move(int _x, int _y, int _cx, int _cy)
	{
		Widget::move(_x, _y, _cx, _cy);
		// ��������� ����
		updateTrack();
	}

} // namespace MyGUI