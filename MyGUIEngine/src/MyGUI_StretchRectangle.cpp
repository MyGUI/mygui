/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "MyGUI_StretchRectangle.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	StretchRectangle::StretchRectangle(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mIsPressed(false),
		mIsFocus(false),
		mIsStatePressed(false),
		mLeftBorder(null), mTopBorder(null), mRightBorder(null), mBottomBorder(null),
		mActionLeft(null), mActionRight(null), mActionTop(null), mActionBottom(null),
		mActionLeftTop(null), mActionLeftBottom(null), mActionRightTop(null), mActionRightBottom(null)
	{
		mNeedKeyFocus = true;

		// парсим свойства
		const MapString & param = _info->getParams();
		if (!param.empty()) {
//			MapString::const_iterator iter = param.find("ButtonPressed");
//			if (iter != param.end()) setButtonPressed(iter->second == "true");
		}

		const int size_line = 1;
		const int size_rect = 7;

		// создаем бордюр
		mLeftBorder = createWidget<Widget>("MarkerGreen", 0, 0, size_line, _coord.height, ALIGN_LEFT | ALIGN_VSTRETCH);
		mTopBorder = createWidget<Widget>("MarkerGreen", size_line, 0, _coord.width - (size_line*2), size_line, ALIGN_HSTRETCH | ALIGN_TOP);
		mRightBorder = createWidget<Widget>("MarkerGreen", _coord.width-size_line, 0, size_line, _coord.height, ALIGN_RIGHT | ALIGN_VSTRETCH);
		mBottomBorder = createWidget<Widget>("MarkerGreen", size_line, _coord.height-size_line, _coord.width - (size_line*2), size_line, ALIGN_HSTRETCH | ALIGN_BOTTOM);

		mActionLeftTop = createWidget<Button>("MarkerGreen", size_line, size_line, size_rect, size_rect, ALIGN_LEFT | ALIGN_TOP);
		mActionLeftBottom = createWidget<Button>("MarkerGreen", size_line, _coord.height - size_rect - size_line, size_rect, size_rect, ALIGN_LEFT | ALIGN_BOTTOM);
		mActionRightTop = createWidget<Button>("MarkerGreen", _coord.width - size_rect - size_line, size_line, size_rect, size_rect, ALIGN_RIGHT | ALIGN_TOP);
		mActionRightBottom = createWidget<Button>("MarkerGreen", _coord.width - size_rect - size_line, _coord.height - size_rect - size_line, size_rect, size_rect, ALIGN_RIGHT | ALIGN_BOTTOM);

		mActionLeft = createWidget<Button>("MarkerGreen", size_line, (_coord.height-size_rect)/2, size_rect, size_rect, ALIGN_LEFT | ALIGN_VCENTER);
		mActionTop = createWidget<Button>("MarkerGreen", (_coord.width-size_rect)/2, size_line, size_rect, size_rect, ALIGN_HCENTER | ALIGN_TOP);
		mActionRight = createWidget<Button>("MarkerGreen", _coord.width - size_rect - size_line, (_coord.height-size_rect)/2, size_rect, size_rect, ALIGN_RIGHT | ALIGN_VCENTER);
		mActionBottom = createWidget<Button>("MarkerGreen", (_coord.width-size_rect)/2, _coord.height - size_rect - size_line, size_rect, size_rect, ALIGN_HCENTER | ALIGN_BOTTOM);

		setSize(200, 200);

		//----------------------------------------------------------------------//
		mLeftBorder->setEnabled(false);
		mTopBorder->setEnabled(false);
		mRightBorder->setEnabled(false);
		mBottomBorder->setEnabled(false);

		mActionLeftTop->setEnabled(false);
		mActionLeftBottom->setEnabled(false);
		mActionRightTop->setEnabled(false);
		mActionRightBottom->setEnabled(false);

		mActionLeft->setEnabled(false);
		mActionTop->setEnabled(false);
		mActionRight->setEnabled(false);
		mActionBottom->setEnabled(false);

		//----------------------------------------------------------------------//
		mLeftBorder->setNeedKeyFocus(true);
		mTopBorder->setNeedKeyFocus(true);
		mRightBorder->setNeedKeyFocus(true);
		mBottomBorder->setNeedKeyFocus(true);

		mActionLeftTop->setNeedKeyFocus(true);
		mActionLeftBottom->setNeedKeyFocus(true);
		mActionRightTop->setNeedKeyFocus(true);
		mActionRightBottom->setNeedKeyFocus(true);

		mActionLeft->setNeedKeyFocus(true);
		mActionTop->setNeedKeyFocus(true);
		mActionRight->setNeedKeyFocus(true);
		mActionBottom->setNeedKeyFocus(true);

		//----------------------------------------------------------------------//
		mLeftBorder->eventKeyLostFocus = newDelegate(this, &StretchRectangle::notifyKeyLostFocus);

	}

	void StretchRectangle::notifyKeyLostFocus(WidgetPtr _sender, WidgetPtr _new)
	{
		//_onKeyLostFocus(_new);
	}

	void StretchRectangle::_onMouseSetFocus(WidgetPtr _old)
	{
		mIsFocus = true;
		updateWidgetState();
		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseSetFocus(_old);
	}

	void StretchRectangle::_onMouseLostFocus(WidgetPtr _new)
	{
		mIsFocus = false;
		updateWidgetState();
		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseLostFocus(_new);
	}

	void StretchRectangle::_onKeyLostFocus(WidgetPtr _new)
	{
		if (false == isChild(_new)) {
			mIsPressed = false;
			updateWidgetState();
		}
		else if (_new != null) {
//			InputManager::getInstance().setKeyFocusWidget(this);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onKeyLostFocus(_new);
	}

	void StretchRectangle::_onKeySetFocus(WidgetPtr _old)
	{
		mIsPressed = true;
		updateWidgetState();

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onKeySetFocus(_old);
	}

	void StretchRectangle::_onMouseButtonPressed(bool _left)
	{
		InputManager::getInstance().setKeyFocusWidget(this);

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseButtonPressed(_left);
	}

	/*void StretchRectangle::_onMouseButtonReleased(bool _left)
	{
		if (_left) {
			mIsPressed = false;
			updateWidgetState();
		}
		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseButtonReleased(_left);
	}*/

	void StretchRectangle::updateWidgetState()
	{
		Ogre::String state;
		if (mIsFocus) {
			if (mIsPressed || mIsStatePressed) state = "select";
			else state = "active";
		} else {
			if (mIsPressed || mIsStatePressed) state = "select";
			else state = "normal";
		}

		if ((false == mIsFocus) && (false == mIsPressed)) {
			mLeftBorder->setEnabled(false);
			mTopBorder->setEnabled(false);
			mRightBorder->setEnabled(false);
			mBottomBorder->setEnabled(false);
		}
		else {
			mLeftBorder->setEnabled(true);
			mTopBorder->setEnabled(true);
			mRightBorder->setEnabled(true);
			mBottomBorder->setEnabled(true);
			if (mIsPressed) {
				mLeftBorder->setState("active");
				mTopBorder->setState("active");
				mRightBorder->setState("active");
				mBottomBorder->setState("active");
			}
			else {
				mLeftBorder->setState("normal");
				mTopBorder->setState("normal");
				mRightBorder->setState("normal");
				mBottomBorder->setState("normal");
			}
		}

		mActionLeftTop->setEnabled(mIsPressed);
		mActionLeftBottom->setEnabled(mIsPressed);
		mActionRightTop->setEnabled(mIsPressed);
		mActionRightBottom->setEnabled(mIsPressed);

		mActionLeft->setEnabled(mIsPressed);
		mActionTop->setEnabled(mIsPressed);
		mActionRight->setEnabled(mIsPressed);
		mActionBottom->setEnabled(mIsPressed);

	}

} // namespace MyGUI