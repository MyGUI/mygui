/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Window.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Gui.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	const float WINDOW_ALPHA_MAX = ALPHA_MAX;
	const float WINDOW_ALPHA_MIN = ALPHA_MIN;
	const float WINDOW_ALPHA_ACTIVE = ALPHA_MAX;
	const float WINDOW_ALPHA_FOCUS = 0.7f;
	const float WINDOW_ALPHA_DEACTIVE = 0.3f;
	const float WINDOW_SPEED_COEF = 3.0f;

	const int WINDOW_SNAP_DISTANSE = 10;

	Window::Window(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mWidgetCaption(null), mWidgetClient(null),
		mMouseRootFocus(false), mKeyRootFocus(false),
		mIsAutoAlpha(false),
		mSnap(false)
	{
		// ��� ����� ����� �����
		mNeedKeyFocus = true;

		// ��������� �������
		mMinmax.set(0, 0, 3000, 3000);

		// ������ ��������
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("Snap");
		if (iter != param.end()) mSnap = utility::parseBool(iter->second);
		iter = param.find("MainMove");
		if (iter != param.end()) setUserString("Scale", "1 1 0 0");

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetClient = (*iter);
			}
			else if ((*iter)->_getInternalString() == "Caption") {
				mWidgetCaption = (*iter);
				mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
				mWidgetCaption->eventMouseDrag = newDelegate(this, &Window::notifyMouseDrag);
			}
			else if ((*iter)->_getInternalString() == "Button") {
				(*iter)->eventMouseButtonClick = newDelegate(this, &Window::notifyPressedButtonEvent);
			}
			else if ((*iter)->_getInternalString() == "Action") {
				(*iter)->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
				(*iter)->eventMouseDrag = newDelegate(this, &Window::notifyMouseDrag);
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

	void Window::_onMouseDrag(int _left, int _top)
	{
		// �� ��� ������, ���� ������� ����, ����� �� ����� ����� �������
		notifyMouseDrag(this, _left, _top);

		// !!! ����������� �������� � ����� ������
		Widget::_onMouseDrag(_left, _top);
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

	void Window::notifyPressedButtonEvent(MyGUI::WidgetPtr _sender)
	{
		eventWindowButtonPressed(this, _sender->getUserString("Event"));
	}

	void Window::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		IntCoord coord = mCurrentActionScale;
		coord.left *= (_left - point.left);
		coord.top *= (_top - point.top);
		coord.width *= (_left - point.left);
		coord.height *= (_top - point.top);

		setPosition(mPreActionCoord + coord);
		
		// �������� ������� � ��������� ������� � �������
		eventWindowChangeCoord(this);
	}

	void Window::updateAlpha()
	{
		if (false == mIsAutoAlpha) return;

		float alpha;
		if (mKeyRootFocus) alpha = WINDOW_ALPHA_ACTIVE;
		else if (mMouseRootFocus) alpha = WINDOW_ALPHA_FOCUS;
		else alpha = WINDOW_ALPHA_DEACTIVE;
		//MYGUI_OUT(alpha);

		ControllerManager::getInstance().addItem(this, new ControllerFadeAlpha(alpha, WINDOW_SPEED_COEF, ControllerFadeAlpha::ACTION_NONE, true));
	}

	void Window::setAutoAlpha(bool _auto)
	{
		mIsAutoAlpha = _auto;
		if (false == _auto) setAlpha(ALPHA_MAX);
		else {
			if (mKeyRootFocus) setAlpha(WINDOW_ALPHA_ACTIVE);
			else if (mMouseRootFocus) setAlpha(WINDOW_ALPHA_FOCUS);
			else setAlpha(WINDOW_ALPHA_DEACTIVE);
		}
	}

	void Window::setPosition(const IntPoint& _pos)
	{
		IntPoint pos = _pos;
		// ���������� � �����
		if (mSnap) {
			if (abs(pos.left) <= WINDOW_SNAP_DISTANSE) pos.left = 0;
			if (abs(pos.top) <= WINDOW_SNAP_DISTANSE) pos.top = 0;

			int width = (int)Gui::getInstance().getViewWidth();
			int height = (int)Gui::getInstance().getViewHeight();

			if ( abs(pos.left + mCoord.width - width) < WINDOW_SNAP_DISTANSE) pos.left = width - mCoord.width;
			if ( abs(pos.top + mCoord.height - height) < WINDOW_SNAP_DISTANSE) pos.top = height - mCoord.height;
		}
		Widget::setPosition(pos);
	}

	void Window::setPosition(const IntCoord& _coord)
	{
		IntPoint pos = _coord.point();
		IntSize size = _coord.size();
		// ���������� � �����
		if (mSnap) {
			if (abs(pos.left) <= WINDOW_SNAP_DISTANSE) pos.left = 0;
			if (abs(pos.top) <= WINDOW_SNAP_DISTANSE) pos.top = 0;

			int width = (int)Gui::getInstance().getViewWidth();
			int height = (int)Gui::getInstance().getViewHeight();

			if ( abs(pos.left + mCoord.width - width) < WINDOW_SNAP_DISTANSE) pos.left = width - mCoord.width;
			if ( abs(pos.top + mCoord.height - height) < WINDOW_SNAP_DISTANSE) pos.top = height - mCoord.height;

			if ( abs(mCoord.left + size.width - width) < WINDOW_SNAP_DISTANSE) size.width = width - mCoord.left;
			if ( abs(mCoord.top + size.height - height) < WINDOW_SNAP_DISTANSE) size.height = height - mCoord.top;
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
		if (mSnap) {
			int width = (int)Gui::getInstance().getViewWidth();
			int height = (int)Gui::getInstance().getViewHeight();

			if ( abs(mCoord.left + size.width - width) < WINDOW_SNAP_DISTANSE) size.width = width - mCoord.left;
			if ( abs(mCoord.top + size.height - height) < WINDOW_SNAP_DISTANSE) size.height = height - mCoord.top;
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

		ControllerManager::getInstance().addItem(
			this, new ControllerFadeAlpha((mIsAutoAlpha && !mKeyRootFocus) ? WINDOW_ALPHA_DEACTIVE : WINDOW_ALPHA_MAX,
			WINDOW_SPEED_COEF, ControllerFadeAlpha::ACTION_NONE, true));
	}

	void Window::hideSmooth()
	{
		ControllerManager::getInstance().addItem(
			this, new ControllerFadeAlpha(WINDOW_ALPHA_MIN, WINDOW_SPEED_COEF, ControllerFadeAlpha::ACTION_HIDE, false));
	}

	void Window::destroySmooth()
	{
		ControllerManager::getInstance().addItem(
			this, new ControllerFadeAlpha(WINDOW_ALPHA_MIN, WINDOW_SPEED_COEF, ControllerFadeAlpha::ACTION_DESTROY, false));
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
