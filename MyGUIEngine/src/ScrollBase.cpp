
#include "ScrollBase.h"
#include "ScrollParser.h"

namespace MyGUI
{
	// парсер команд
	namespace parser { ScrollParser ScrollParserInstance; }

	ScrollBase::ScrollBase(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		mWidgetStart(null), mWidgetEnd(null), mWidgetTrack(null),
		mScrollPosition(0), mScrollRange(0)
	{

		// запомием размер скина
		intSize skinSize = _info->getSize();

		std::string skin;
		floatRect offset;
		char align=0;

		const SkinParam & param = _info->getParams();
		SkinParam::const_iterator iter = param.find("SkinStart");
		if (iter != param.end()) skin = iter->second;
		iter = param.find("OffsetStart");
		if (iter != param.end()) offset = floatRect::parse(iter->second);
		iter = param.find("AlignStart");
		if (iter != param.end()) align = SkinManager::getInstance().parseAlign(iter->second);

		offset = WidgetManager::convertOffset(offset, align, skinSize, m_cx, m_cy);
		mWidgetStart = createWidget("Button", skin, offset.left, offset.top, offset.right, offset.bottom, align);
		mWidgetStart->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);

		iter = param.find("SkinEnd");
		if (iter != param.end()) skin = iter->second;
		iter = param.find("OffsetEnd");
		if (iter != param.end()) offset = floatRect::parse(iter->second);
		iter = param.find("AlignEnd");
		if (iter != param.end()) align = SkinManager::getInstance().parseAlign(iter->second);

		offset = WidgetManager::convertOffset(offset, align, skinSize, m_cx, m_cy);
		mWidgetEnd = createWidget("Button", skin, offset.left, offset.top, offset.right, offset.bottom, align);
		mWidgetEnd->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);

		iter = param.find("SkinTrack");
		if (iter != param.end()) skin = iter->second;
		iter = param.find("OffsetTrack");
		if (iter != param.end()) offset = floatRect::parse(iter->second);
		iter = param.find("AlignTrack");
		if (iter != param.end()) align = SkinManager::getInstance().parseAlign(iter->second);
		iter = param.find("SkinTrackRange");
		if (iter != param.end()) {
			intSize size = intSize::parse(iter->second);
			mSkinRangeStart = size.width;
			mSkinRangeEnd = size.height;
		}

		offset = WidgetManager::convertOffset(offset, align, skinSize, m_cx, m_cy);
		mWidgetTrack = createWidget("Button", skin, offset.left, offset.top, offset.right, offset.bottom, align);
		mWidgetTrack->eventMouseMove = newDelegate(this, &ScrollBase::notifyMouseMove);
		mWidgetTrack->eventMouseButtonPressed = newDelegate(this, &ScrollBase::notifyMousePressed);
		mWidgetTrack->eventMouseButtonReleased = newDelegate(this, &ScrollBase::notifyMouseReleased);
		mWidgetTrack->show(false);
	}

	void ScrollBase::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (!_left) return;
		if (_sender == mWidgetStart) {
			if (mScrollPosition == 0) return;
			mScrollPosition --;
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();
		} else if (_sender == mWidgetEnd){
			if ( (mScrollRange < 2) || (mScrollPosition == (mScrollRange-1)) ) return;
			mScrollPosition ++;
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
		// обновляем трек
		updateTrack();
	}

} // namespace MyGUI