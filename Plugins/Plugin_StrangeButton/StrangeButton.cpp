/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/

#include "StrangeButton.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_ISubWidgetText.h"

namespace plugin
{

	StrangeButton::StrangeButton(MyGUI::WidgetStyle _style, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const MyGUI::WidgetSkinInfoPtr _info, MyGUI::WidgetPtr _parent, MyGUI::ICroppedRectangle * _croppedParent, MyGUI::IWidgetCreator * _creator, const std::string & _name) :
		MyGUI::Widget(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mIsMousePressed(false),
		mIsMouseFocus(false),
		mIsStateCheck(false)
	{

		// парсим свойства
		const MyGUI::MapString & properties = _info->getProperties();
		if (!properties.empty()) {
			MyGUI::MapString::const_iterator iter = properties.find("ButtonPressed");
			if (iter != properties.end()) setButtonPressed(MyGUI::utility::parseBool(iter->second));
		}
	}

	void StrangeButton::onMouseSetFocus(MyGUI::WidgetPtr _old)
	{
		Widget::onMouseSetFocus(_old);
		mIsMouseFocus = true;

		if (mText == nullptr) return;
		Ogre::String s, str = mText->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];

		mText->setCaption(s);

		updateButtonState();
	}

	void StrangeButton::onMouseLostFocus(MyGUI::WidgetPtr _new)
	{
		Widget::onMouseLostFocus(_new);
		mIsMouseFocus = false;

		if (mText == nullptr) return;
		Ogre::String s, str = mText->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];
		
		mText->setCaption(s);

		updateButtonState();
	}

	void StrangeButton::onMouseButtonPressed(int _left, int _top, MyGUI::MouseButton _id)
	{
		MyGUI::Widget::onMouseButtonPressed(_left, _top, _id);
		if (MyGUI::MouseButton::Left != _id) return;
		mIsMousePressed = true;
		updateButtonState();
	}

	void StrangeButton::onMouseButtonReleased(int _left, int _top, MyGUI::MouseButton _id)
	{
		MyGUI::Widget::onMouseButtonReleased(_left, _top, _id);
		if (MyGUI::MouseButton::Left != _id) return;
		mIsMousePressed = false;
		updateButtonState();
	}

} // namespace plugin