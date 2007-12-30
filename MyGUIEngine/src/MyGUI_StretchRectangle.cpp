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
		grid_step(1)
	{
		mNeedKeyFocus = true;

		const int size_line = 1;
		const int size_rect = 7;

		size_t count = 0;
		WidgetPtr widget = this;

		// ищем считаем максимальную глубину виджета
		_testCount(count, count, widget);

		// теперь удал€ем всех детей
		_destroyAllChildWidget();

		// теперь создаем бутерброд, чтобы рамки были в самом верху
		for (size_t pos=0; pos<count; pos++) {
			widget = widget->createWidget<Widget>("Empty", 0, 0, widget->getWidth(), widget->getHeight(), ALIGN_STRETCH);
		}

		//widget->setNeedKeyFocus(true);

		/*const VectorWidgetPtr& childs = widget->getChilds();
		for (VectorWidgetPtr::const_iterator iter=childs.begin(); iter!=childs.end(); ++iter) {
			WidgetPtr child = const_cast<WidgetPtr >(*iter);
			child->setEnabled(false);
		}*/

		//MYGUI_OUT(count);

		// создаем бордюр
		mWidgetBorder[BORDER_LEFT] = widget->createWidget<Widget>("MarkerGreen", 0, 0, size_line, _coord.height, ALIGN_LEFT | ALIGN_VSTRETCH);
		mWidgetBorder[BORDER_TOP] = widget->createWidget<Widget>("MarkerGreen", size_line, 0, _coord.width - (size_line*2), size_line, ALIGN_HSTRETCH | ALIGN_TOP);
		mWidgetBorder[BORDER_RIGHT] = widget->createWidget<Widget>("MarkerGreen", _coord.width-size_line, 0, size_line, _coord.height, ALIGN_RIGHT | ALIGN_VSTRETCH);
		mWidgetBorder[BORDER_BOTTOM] = widget->createWidget<Widget>("MarkerGreen", size_line, _coord.height-size_line, _coord.width - (size_line*2), size_line, ALIGN_HSTRETCH | ALIGN_BOTTOM);

		mWidgetAction[ACTION_LEFT_TOP] = widget->createWidget<Button>("MarkerGreen", size_line, size_line, size_rect, size_rect, ALIGN_LEFT | ALIGN_TOP);
		mWidgetAction[ACTION_RIGHT_TOP] = widget->createWidget<Button>("MarkerGreen", _coord.width - size_rect - size_line, size_line, size_rect, size_rect, ALIGN_RIGHT | ALIGN_TOP);
		mWidgetAction[ACTION_RIGHT_BOTTOM] = widget->createWidget<Button>("MarkerGreen", _coord.width - size_rect - size_line, _coord.height - size_rect - size_line, size_rect, size_rect, ALIGN_RIGHT | ALIGN_BOTTOM);
		mWidgetAction[ACTION_LEFT_BOTTOM] = widget->createWidget<Button>("MarkerGreen", size_line, _coord.height - size_rect - size_line, size_rect, size_rect, ALIGN_LEFT | ALIGN_BOTTOM);

		mWidgetAction[ACTION_LEFT] = widget->createWidget<Button>("MarkerGreen", size_line, (_coord.height-size_rect)/2, size_rect, size_rect, ALIGN_LEFT | ALIGN_VCENTER);
		mWidgetAction[ACTION_TOP] = widget->createWidget<Button>("MarkerGreen", (_coord.width-size_rect)/2, size_line, size_rect, size_rect, ALIGN_HCENTER | ALIGN_TOP);
		mWidgetAction[ACTION_RIGHT] = widget->createWidget<Button>("MarkerGreen", _coord.width - size_rect - size_line, (_coord.height-size_rect)/2, size_rect, size_rect, ALIGN_RIGHT | ALIGN_VCENTER);
		mWidgetAction[ACTION_BOTTOM] = widget->createWidget<Button>("MarkerGreen", (_coord.width-size_rect)/2, _coord.height - size_rect - size_line, size_rect, size_rect, ALIGN_HCENTER | ALIGN_BOTTOM);

		//widget->eventMouseLostFocus = newDelegate(this, &StretchRectangle::notifyMouseLostFocus);
		//widget->eventMouseLostFocus = newDelegate(this, &StretchRectangle::notifyMouseSetFocus);

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
		widget->setUserString("Scale", "1 1 0 0");

	}

	void StretchRectangle::notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
	//	mIsFocus = false;
	//	updateWidgetState();
	}

	void StretchRectangle::notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
	{
		//mIsFocus = true;
		//updateWidgetState();
	}

	void StretchRectangle::_testCount(size_t _count, size_t & _max, WidgetPtr _widget)
	{
		_count ++;
		if (_count > _max) _max = _count;
		const VectorWidgetPtr& childs = _widget->getChilds();
		for (VectorWidgetPtr::const_iterator iter=childs.begin(); iter!=childs.end(); ++iter) {
			if (false == (*iter)->isShow()) continue;
			_testCount(_count, _max, (*iter));
		}
	}

	void StretchRectangle::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (_left) mPreActionCoord = mCoord;
	}

	void StretchRectangle::notifyMouseMove(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		#define TO_GRID(x) (x/grid_step*grid_step)
		_left = TO_GRID(_left);
		_top = TO_GRID(_top);
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
		notifyMouseMove(this, _left, _top);

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
