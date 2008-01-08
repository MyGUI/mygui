/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Window.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_ControllerFadeAlpha.h"

namespace MyGUI
{

	const float WINDOW_ALPHA_MAX = ALPHA_MAX;
	const float WINDOW_ALPHA_MIN = ALPHA_MIN;
	const float WINDOW_ALPHA_ACTIVE = ALPHA_MAX;
	const float WINDOW_ALPHA_FOCUS = 0.7f;
	const float WINDOW_ALPHA_DEACTIVE = 0.3f;
	const float WINDOW_SPEED_COEF = 3.0f;

	const int WINDOW_TO_STICK = 10;

	Window::Window(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mWidgetCaption(null), mWidgetClient(null),
		mMouseRootFocus(false), mKeyRootFocus(false),
		mIsAutoAlpha(false),
		mIsToStick(false)
	{
		// ��� ����� ����� �����
		mNeedKeyFocus = true;

		// ��������� �������
		mMinmax.set(0, 0, 3000, 3000);

		// ������ ��������
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("ToStick");
		if (iter != param.end()) mIsToStick = util::parseBool(iter->second);
		iter = param.find("MainMove");
		if (iter != param.end()) setUserString("Scale", "1 1 0 0");

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->getInternalString() == "Client") {
				mWidgetClient = (*iter);
			}
			else if ((*iter)->getInternalString() == "Caption") {
				mWidgetCaption = (*iter);
				mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
				mWidgetCaption->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedAction);
			}
			else if ((*iter)->getInternalString() == "Button") {
				(*iter)->eventMouseButtonClick = newDelegate(this, &Window::notifyPressedButtonEvent);
			}
			else if ((*iter)->getInternalString() == "Action") {
				(*iter)->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
				(*iter)->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedAction);
			}
		}

	}

	// �������������� ��� ���������� �������
	WidgetPtr Window::createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name)
	{
		if (mWidgetClient != null) return mWidgetClient->createWidgetT(_type, _skin, _coord, _align, _name);
		return Widget::createWidgetT(_type, _skin, _coord, _align, _name);
	}

	void Window::_onMouseChangeRootFocus(bool _focus)
	{
		mMouseRootFocus = _focus;
		updateAlpha();

		// !!! ����������� �������� � ����� ������
		Widget::_onMouseChangeRootFocus(_focus);
	}

	void Window::_onKeyChangeRootFocus(bool _focus)
	{
		mKeyRootFocus = _focus;
		updateAlpha();

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyChangeRootFocus(_focus);
	}

	void Window::_onMouseMove(int _left, int _top)
	{
		// �� ��� ������, ���� ������� ����, ����� �� ����� ����� �������
		notifyMouseMovedAction(this, _left, _top);

		// !!! ����������� �������� � ����� ������
		Widget::_onMouseMove(_left, _top);
	}

	void Window::_onMouseButtonPressed(bool _left)
	{
		notifyMousePressed(this, _left);
		// !!! ����������� �������� � ����� ������
		Widget::_onMouseButtonPressed(_left);
	}

	void Window::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (_left) {
			mPreActionCoord = mCoord;
			mCurrentActionScale = IntCoord::parse(_sender->getUserString("Scale"));
		}
	}

	void Window::notifyPressedButtonEvent(MyGUI::WidgetPtr _sender, bool _double)
	{
		if (false == _double) eventWindowButtonPressed(this, _sender->getUserString("Button"));
	}

	void Window::notifyMouseMovedAction(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		IntCoord coord = mCurrentActionScale;
		coord.left *= (_left - point.left);
		coord.top *= (_top - point.top);
		coord.width *= (_left - point.left);
		coord.height *= (_top - point.top);

		setPosition(mPreActionCoord + coord);
	}

	void Window::updateAlpha()
	{
		if (false == mIsAutoAlpha) return;

		if (mKeyRootFocus) ControllerFadeAlpha::getInstance().addItem(this, WINDOW_ALPHA_ACTIVE, WINDOW_SPEED_COEF);
		else if (mMouseRootFocus) ControllerFadeAlpha::getInstance().addItem(this, WINDOW_ALPHA_FOCUS, WINDOW_SPEED_COEF);
		else ControllerFadeAlpha::getInstance().addItem(this, WINDOW_ALPHA_DEACTIVE, WINDOW_SPEED_COEF);
	}

	void Window::setPosition(const IntPoint& _pos)
	{
		IntPoint pos = _pos;
		// ���������� � �����
		if (mIsToStick) {
			if (pos.left > 0) {if ( (pos.left - WINDOW_TO_STICK) <= 0) pos.left = 0;}
			else {if ( (pos.left + WINDOW_TO_STICK) >= 0) pos.left = 0;}
			if (pos.top > 0) {if ( (pos.top - WINDOW_TO_STICK) <= 0) pos.top = 0;}
			else {	if ( (pos.top + WINDOW_TO_STICK) >= 0) pos.top = 0;}

			int width = (int)Gui::getInstance().getViewWidth();
			int height = (int)Gui::getInstance().getViewHeight();

			if ( (pos.left + mCoord.width) < width) {if ( (pos.left + mCoord.width + WINDOW_TO_STICK) > width ) pos.left = width - mCoord.width;	}
			else {	if ( (pos.left + mCoord.width - WINDOW_TO_STICK) < width ) pos.left = width - mCoord.width;}
			if ( (pos.top + mCoord.height) < height) {if ( (pos.top + mCoord.height + WINDOW_TO_STICK) > height ) pos.top = height - mCoord.height;}
			else {	if ( (pos.top + mCoord.height - WINDOW_TO_STICK) < height ) pos.top = height - mCoord.height;}
		}
		Widget::setPosition(pos);
	}

	void Window::setPosition(const IntCoord& _coord)
	{
		IntPoint pos = _coord.point();
		IntSize size = _coord.size();
		// ���������� � �����
		if (mIsToStick) {
			if (pos.left > 0) {if ( (pos.left - WINDOW_TO_STICK) <= 0) pos.left = 0;}
			else {if ( (pos.left + WINDOW_TO_STICK) >= 0) pos.left = 0;}
			if (pos.top > 0) {if ( (pos.top - WINDOW_TO_STICK) <= 0) pos.top = 0;}
			else {	if ( (pos.top + WINDOW_TO_STICK) >= 0) pos.top = 0;}

			int width = (int)Gui::getInstance().getViewWidth();
			int height = (int)Gui::getInstance().getViewHeight();

			if ( (pos.left + mCoord.width) < width) {if ( (pos.left + mCoord.width + WINDOW_TO_STICK) > width ) pos.left = width - mCoord.width;	}
			else {	if ( (pos.left + mCoord.width - WINDOW_TO_STICK) < width ) pos.left = width - mCoord.width;}
			if ( (pos.top + mCoord.height) < height) {if ( (pos.top + mCoord.height + WINDOW_TO_STICK) > height ) pos.top = height - mCoord.height;}
			else {	if ( (pos.top + mCoord.height - WINDOW_TO_STICK) < height ) pos.top = height - mCoord.height;}

			if ( (mCoord.left + size.width) < width) {if ( (mCoord.left + size.width + WINDOW_TO_STICK) > width ) size.width = width - mCoord.left;	}
			else {	if ( (mCoord.left + size.width - WINDOW_TO_STICK) < width ) size.width = width - mCoord.left;}
			if ( (mCoord.top + size.height) < height) {if ( (mCoord.top + size.height + WINDOW_TO_STICK) > height ) size.height = height - mCoord.top;}
			else {	if ( (mCoord.top + size.height - WINDOW_TO_STICK) < height ) size.height = height - mCoord.top;}
		}

		if (size.width < mMinmax.left) {
			int offset = mMinmax.left - size.width;
			size.width = mMinmax.left;
			if ((pos.left - mCoord.left) > offset) pos.left -= offset;
			else pos.left = mCoord.left;
		}
		else if (size.width > mMinmax.right) {
			int offset = mMinmax.right - size.width;
			size.width = mMinmax.right;
			if ((pos.left - mCoord.left) < offset) pos.left -= offset;
			else pos.left = mCoord.left;
		}
		if (size.height < mMinmax.top) {
			int offset = mMinmax.top - size.height;
			size.height = mMinmax.top;
			if ((pos.top - mCoord.top) > offset) pos.top -= offset;
			else pos.top = mCoord.top;
		}
		else if (size.height > mMinmax.bottom) {
			int offset = mMinmax.bottom - size.height;
			size.height = mMinmax.bottom;
			if ((pos.top - mCoord.top) < offset) pos.top -= offset;
			else pos.top = mCoord.top;
		}

		IntCoord coord(pos, size);
		if (coord == mCoord) return;

		Widget::setPosition(coord);
	}

	void Window::setSize(const IntSize& _size)
	{
		IntSize size = _size;
		// ���������� � �����
		if (mIsToStick) {
			int width = (int)Gui::getInstance().getViewWidth();
			int height = (int)Gui::getInstance().getViewHeight();

			if ( (mCoord.left + size.width) < width) {if ( (mCoord.left + size.width + WINDOW_TO_STICK) > width ) size.width = width - mCoord.left;	}
			else {	if ( (mCoord.left + size.width - WINDOW_TO_STICK) < width ) size.width = width - mCoord.left;}
			if ( (mCoord.top + size.height) < height) {if ( (mCoord.top + size.height + WINDOW_TO_STICK) > height ) size.height = height - mCoord.top;}
			else {	if ( (mCoord.top + size.height - WINDOW_TO_STICK) < height ) size.height = height - mCoord.top;}
		}

		if (size.width < mMinmax.left) size.width = mMinmax.left;
		else if (size.width > mMinmax.right) size.width = mMinmax.right;
		if (size.height < mMinmax.top) size.height = mMinmax.top;
		else if (size.height > mMinmax.bottom) size.height = mMinmax.bottom;
		if ((size.width == mCoord.width) && (size.height == mCoord.height) ) return;

		Widget::setSize(size);
	}

	// ��� ������������ ������ � �������
	void Window::showSmooth(bool _reset)
	{
		if (_reset) {
			setAlpha(ALPHA_MIN);
			show();
		}

		ControllerFadeAlpha::getInstance().addItem(this,
			(mIsAutoAlpha && !mKeyRootFocus) ? WINDOW_ALPHA_DEACTIVE : WINDOW_ALPHA_MAX,
			WINDOW_SPEED_COEF, false, true, false);
	}

	void Window::hideSmooth()
	{
		ControllerFadeAlpha::getInstance().addItem(
			this, WINDOW_ALPHA_MIN, WINDOW_SPEED_COEF, true, false, false);
	}

	void Window::destroySmooth()
	{
		ControllerFadeAlpha::getInstance().addItem(
			this, WINDOW_ALPHA_MIN, WINDOW_SPEED_COEF, false, false, true);
	}

	const IntCoord& Window::getClientRect()
	{
		if (null == mWidgetClient) return Widget::getClientRect();
		return mWidgetClient->getClientRect();
	}

	VectorWidgetPtr Window::getChilds()
	{
		if (null == mWidgetClient) return Widget::getChilds();
		return mWidgetClient->getChilds();
	}

} // namespace MyGUI
