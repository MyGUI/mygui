
#include "MyGUI_Prerequest.h"
#include "StrangeButton.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_SubWidgetTextInterface.h"

namespace MyGUI
{

	Ogre::String StrangeButton::WidgetTypeName = "StrangeButton";

	StrangeButton::StrangeButton(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
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

	void StrangeButton::_onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		Widget::_onMouseButtonPressed(_left, _top, _id);
		if (MB_Left != _id) return;
		mIsPressed = true;
		updateButtonState();
	}

	void StrangeButton::_onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		Widget::_onMouseButtonReleased(_left, _top, _id);
		if (MB_Left != _id) return;
		mIsPressed = false;
		updateButtonState();
	}

} // namespace MyGUI