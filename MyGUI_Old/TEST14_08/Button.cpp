
#include "Button.h"
#include "ButtonParser.h"

namespace widget
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<Button> ButtonFactoryInstance; }
	// парсер команд
	namespace parser { ButtonParser ButtonParserInstance; }

	Button::Button(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
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

	void Button::OnMouseSetFocus(WidgetPtr _old)
	{
		Widget::OnMouseSetFocus(_old);
		m_isFocus = true;
		updateButtonState();
	}

	void Button::OnMouseLostFocus(WidgetPtr _new)
	{
		Widget::OnMouseLostFocus(_new);
		m_isFocus = false;
		updateButtonState();
	}

	void Button::OnMouseButtonPressed(bool _left)
	{
		Widget::OnMouseButtonPressed(_left);
		if (!_left) return;
		m_isPressed = true;
		updateButtonState();
	}

	void Button::OnMouseButtonReleased(bool _left)
	{
		Widget::OnMouseButtonReleased(_left);
		if (!_left) return;
		m_isPressed = false;
		updateButtonState();
	}

} // namespace widget