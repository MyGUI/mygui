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
		mIsStatePressed(false)
	{
		mNeedKeyFocus = true;

		const int size_line = 1;
		const int size_rect = 7;

		// создаем бордюр
		mWidgetBorder[BORDER_LEFT] = createWidget<Widget>("MarkerGreen", 0, 0, size_line, _coord.height, ALIGN_LEFT | ALIGN_VSTRETCH);
		mWidgetBorder[BORDER_TOP] = createWidget<Widget>("MarkerGreen", size_line, 0, _coord.width - (size_line*2), size_line, ALIGN_HSTRETCH | ALIGN_TOP);
		mWidgetBorder[BORDER_RIGHT] = createWidget<Widget>("MarkerGreen", _coord.width-size_line, 0, size_line, _coord.height, ALIGN_RIGHT | ALIGN_VSTRETCH);
		mWidgetBorder[BORDER_BOTTOM] = createWidget<Widget>("MarkerGreen", size_line, _coord.height-size_line, _coord.width - (size_line*2), size_line, ALIGN_HSTRETCH | ALIGN_BOTTOM);

		mWidgetAction[ACTION_LEFT_TOP] = createWidget<Button>("MarkerGreen", size_line, size_line, size_rect, size_rect, ALIGN_LEFT | ALIGN_TOP);
		mWidgetAction[ACTION_RIGHT_TOP] = createWidget<Button>("MarkerGreen", _coord.width - size_rect - size_line, size_line, size_rect, size_rect, ALIGN_RIGHT | ALIGN_TOP);
		mWidgetAction[ACTION_RIGHT_BOTTOM] = createWidget<Button>("MarkerGreen", _coord.width - size_rect - size_line, _coord.height - size_rect - size_line, size_rect, size_rect, ALIGN_RIGHT | ALIGN_BOTTOM);
		mWidgetAction[ACTION_LEFT_BOTTOM] = createWidget<Button>("MarkerGreen", size_line, _coord.height - size_rect - size_line, size_rect, size_rect, ALIGN_LEFT | ALIGN_BOTTOM);

		mWidgetAction[ACTION_LEFT] = createWidget<Button>("MarkerGreen", size_line, (_coord.height-size_rect)/2, size_rect, size_rect, ALIGN_LEFT | ALIGN_VCENTER);
		mWidgetAction[ACTION_TOP] = createWidget<Button>("MarkerGreen", (_coord.width-size_rect)/2, size_line, size_rect, size_rect, ALIGN_HCENTER | ALIGN_TOP);
		mWidgetAction[ACTION_RIGHT] = createWidget<Button>("MarkerGreen", _coord.width - size_rect - size_line, (_coord.height-size_rect)/2, size_rect, size_rect, ALIGN_RIGHT | ALIGN_VCENTER);
		mWidgetAction[ACTION_BOTTOM] = createWidget<Button>("MarkerGreen", (_coord.width-size_rect)/2, _coord.height - size_rect - size_line, size_rect, size_rect, ALIGN_HCENTER | ALIGN_BOTTOM);

		//----------------------------------------------------------------------//
		for (size_t pos=0; pos<BORDER_COUNT; pos++) {
			mWidgetBorder[pos]->setEnabled(false);
		}

		for (size_t pos=0; pos<ACTION_COUNT; pos++) {
			mWidgetAction[pos]->setEnabled(false);
			mWidgetAction[pos]->eventMouseButtonPressed = newDelegate(this, &StretchRectangle::notifyMousePressed);
			mWidgetAction[pos]->eventMouseMove = newDelegate(this, &StretchRectangle::notifyMouseMove);
		}

		//----------------------------------------------------------------------//
		mWidgetAction[ACTION_LEFT_TOP]->setUserString("Scale", "1 1 -1 -1");
		mWidgetAction[ACTION_TOP]->setUserString("Scale", "0 1 0 -1");
		mWidgetAction[ACTION_RIGHT_TOP]->setUserString("Scale", "0 1 1 -1");
		mWidgetAction[ACTION_RIGHT]->setUserString("Scale", "0 0 1 0");
		mWidgetAction[ACTION_RIGHT_BOTTOM]->setUserString("Scale", "0 0 1 1");
		mWidgetAction[ACTION_BOTTOM]->setUserString("Scale", "0 0 0 1");
		mWidgetAction[ACTION_LEFT_BOTTOM]->setUserString("Scale", "1 0 -1 1");
		mWidgetAction[ACTION_LEFT]->setUserString("Scale", "1 0 -1 0");

	}

	void StretchRectangle::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (_left) mPreActionCoord = mCoord;
	}

	void StretchRectangle::notifyMouseMove(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		IntCoord coord(_sender->getUserString("Scale"));
		coord.left *= (_left - point.left);
		coord.top *= (_top - point.top);
		coord.width *= (_left - point.left);
		coord.height *= (_top - point.top);
		
		Widget::setPosition(mPreActionCoord + coord);
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
		mIsPressed = false;
		updateWidgetState();

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
		if (_left) mPreActionCoord = mCoord;

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseButtonPressed(_left);
	}

	void StretchRectangle::_onMouseMove(int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
		Widget::setPosition(mPreActionCoord + IntPoint(_left - point.left, _top - point.top));

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseMove(_left, _top);
	}

	void StretchRectangle::updateWidgetState()
	{
		if ((false == mIsFocus) && (false == mIsPressed)) {
			for (size_t pos=0; pos<BORDER_COUNT; pos++) {
				mWidgetBorder[pos]->setEnabled(false);
			}
		}
		else {
			for (size_t pos=0; pos<BORDER_COUNT; pos++) {
				mWidgetBorder[pos]->setEnabled(true);
				if (mIsPressed) mWidgetBorder[pos]->setState("active");
				else mWidgetBorder[pos]->setState("normal");
			}
		}

		for (size_t pos=0; pos<ACTION_COUNT; pos++) {
			mWidgetAction[pos]->setEnabled(mIsPressed);
		}

	}

} // namespace MyGUI