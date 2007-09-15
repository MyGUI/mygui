
#include "Edit.h"
#include "EditParser.h"

namespace MyGUI
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<Edit> EditFactoryInstance; }
	// парсер команд
	namespace parser { EditParser EditParserInstance; }

	Edit::Edit(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		m_isPressed(false),
		m_isFocus(false)
	{

		// парсим свойства
		const SkinParam & param = _info->getParams();
		if (!param.empty()) {
//			SkinParam::const_iterator iter = param.find("ButtonPressed");
//			if (iter != param.end()) setButtonPressed(iter->second == "true");
		}
	}

	void Edit::OnMouseSetFocus(WidgetPtr _old)
	{
		Widget::OnMouseSetFocus(_old);
		m_isFocus = true;
		updateEditState();
	}

	void Edit::OnMouseLostFocus(WidgetPtr _new)
	{
		Widget::OnMouseLostFocus(_new);
		m_isFocus = false;
		updateEditState();
	}

	void Edit::OnMouseButtonPressed(bool _left)
	{
		Widget::OnMouseButtonPressed(_left);
//		if (!_left) return;
//		m_isPressed = true;
//		updateEditState();
	}

	void Edit::OnMouseButtonReleased(bool _left)
	{
		Widget::OnMouseButtonReleased(_left);
//		if (!_left) return;
//		m_isPressed = false;
//		updateEditState();
	}

	void Edit::OnKeySetFocus(WidgetPtr _old)
	{
		Widget::OnKeySetFocus(_old);
		m_isPressed = true;
		updateEditState();
	}

	void Edit::OnKeyLostFocus(WidgetPtr _new)
	{
		Widget::OnKeyLostFocus(_new);
		m_isPressed = false;
		updateEditState();
	}

	void Edit::OnKeyButtonPressed(int _key, wchar_t _char)
	{
		Widget::OnKeyButtonPressed(_key, _char);
		if (_key == OIS::KC_ESCAPE) Gui::getInstance().setKeyFocusWidget(null);
		else if (_key == OIS::KC_BACK) {
			ASSERT(m_text);
			const Ogre::DisplayString & text = m_text->getCaption();
			if (!text.empty()) m_text->setCaption(text.substr(0, text.length()-1)); 
		} else if (_char != 0) {
			ASSERT(m_text);
			const Ogre::DisplayString & text = m_text->getCaption();
			m_text->setCaption(text + _char); 
		}
	}

	void Edit::OnKeyButtonReleased(int _key)
	{
		Widget::OnKeyButtonReleased(_key);
	}

} // namespace MyGUI