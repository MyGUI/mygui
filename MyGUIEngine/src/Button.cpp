
#include "Button.h"

namespace MyGUI
{

	Button::Button(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_left, _top, _width, _height, _align, _info, _parent, _name),
		mIsPressed(false),
		mIsFocus(false),
		mIsStatePressed(false)
	{

		// парсим свойства
		const MapString & param = _info->getParams();
		if (!param.empty()) {
			MapString::const_iterator iter = param.find("ButtonPressed");
			if (iter != param.end()) setButtonPressed(iter->second == "true");
		}
	}

	void Button::_onMouseSetFocus(WidgetPtr _old)
	{
		Widget::_onMouseSetFocus(_old);
		mIsFocus = true;
		updateButtonState();
	}

	void Button::_onMouseLostFocus(WidgetPtr _new)
	{
		Widget::_onMouseLostFocus(_new);
		mIsFocus = false;
		updateButtonState();
	}

	void Button::_onMouseButtonPressed(bool _left)
	{
		Widget::_onMouseButtonPressed(_left);
		if (!_left) return;
		mIsPressed = true;
		updateButtonState();
	}

	void Button::_onMouseButtonReleased(bool _left)
	{
		Widget::_onMouseButtonReleased(_left);
		if (!_left) return;
		mIsPressed = false;
		updateButtonState();
	}

} // namespace MyGUI