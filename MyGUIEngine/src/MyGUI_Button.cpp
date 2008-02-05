/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Button.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	Button::Button(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
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
		_setMouseFocus(true);
		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseSetFocus(_old);
	}

	void Button::_onMouseLostFocus(WidgetPtr _new)
	{
		_setMouseFocus(false);
		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseLostFocus(_new);
	}

	void Button::_onMouseButtonPressed(bool _left)
	{
		if (_left) {
			mIsPressed = true;
			updateButtonState();
		}
		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseButtonPressed(_left);
	}

	void Button::_onMouseButtonReleased(bool _left)
	{
		if (_left) {
			mIsPressed = false;
			updateButtonState();
		}
		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseButtonReleased(_left);
	}

} // namespace MyGUI
