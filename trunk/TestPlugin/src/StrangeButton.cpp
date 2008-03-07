
#include "MyGUI_Prerequest.h"
#include "StrangeButton.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_SubWidgetTextInterface.h"

namespace MyGUI
{

	StrangeButton::StrangeButton(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
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

	void StrangeButton::_onMouseSetFocus(WidgetPtr _old)
	{
		Widget::_onMouseSetFocus(_old);
		mIsFocus = true;

		if (mText == null) return;
		Ogre::String s, str = mText->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];

		mText->setCaption(s);

		updateButtonState();
	}

	void StrangeButton::_onMouseLostFocus(WidgetPtr _new)
	{
		Widget::_onMouseLostFocus(_new);
		mIsFocus = false;

		if (mText == null) return;
		Ogre::String s, str = mText->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];
		
		mText->setCaption(s);

		updateButtonState();
	}

	void StrangeButton::_onMouseButtonPressed(bool _left)
	{
		Widget::_onMouseButtonPressed(_left);
		if (!_left) return;
		mIsPressed = true;
		updateButtonState();
	}

	void StrangeButton::_onMouseButtonReleased(bool _left)
	{
		Widget::_onMouseButtonReleased(_left);
		if (!_left) return;
		mIsPressed = false;
		updateButtonState();
	}

} // namespace MyGUI