
#include "StrangeButton.h"
#include "StrangeButtonParser.h"

namespace MyGUI
{
	// парсер команд
	namespace parser { StrangeButtonParser ButtonParserInstance; }

	StrangeButton::StrangeButton(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
	Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		m_isPressed(false),
		m_isFocus(false),
		m_isStatePressed(false)
	{

		// парсим свойства
		const SkinParam & param = _info->getParams();
		if (!param.empty()) {
			SkinParam::const_iterator iter = param.find("ButtonPressed");
			if (iter != param.end()) setButtonPressed(iter->second == "true");
		}
	}

	void StrangeButton::_onMouseSetFocus(WidgetPtr _old)
	{
		Widget::_onMouseSetFocus(_old);
		m_isFocus = true;

		String s, str = m_text->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];

		m_text->setCaption(s);

		updateButtonState();
	}

	void StrangeButton::_onMouseLostFocus(WidgetPtr _new)
	{
		Widget::_onMouseLostFocus(_new);
		m_isFocus = false;

		String s, str = m_text->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];
		
		m_text->setCaption(s);

		updateButtonState();
	}

	void StrangeButton::_onMouseButtonPressed(bool _left)
	{
		Widget::_onMouseButtonPressed(_left);
		if (!_left) return;
		m_isPressed = true;
		updateButtonState();
	}

	void StrangeButton::_onMouseButtonReleased(bool _left)
	{
		Widget::_onMouseButtonReleased(_left);
		if (!_left) return;
		m_isPressed = false;
		updateButtonState();
	}

} // namespace MyGUI