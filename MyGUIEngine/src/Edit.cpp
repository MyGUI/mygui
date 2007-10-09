
#include "Edit.h"
#include "EditParser.h"

namespace MyGUI
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<Edit> EditFactoryInstance("Edit"); }
	// парсер команд
	namespace parser { EditParser EditParserInstance; }

	Edit::Edit(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		m_isPressed(false),
		m_isFocus(false)
	{

		// парсим свойства
//		const SkinParam & param = _info->getParams();
//		if (!param.empty()) {
//			SkinParam::const_iterator iter = param.find("ButtonPressed");
//			if (iter != param.end()) setButtonPressed(iter->second == "true");
//		}
	}

	void Edit::_onMouseSetFocus(WidgetPtr _old)
	{
		Widget::_onMouseSetFocus(_old);
		m_isFocus = true;
		updateEditState();
	}

	void Edit::_onMouseLostFocus(WidgetPtr _new)
	{
		Widget::_onMouseLostFocus(_new);
		m_isFocus = false;
		updateEditState();
	}

	void Edit::_onMouseButtonPressed(bool _left)
	{
		Widget::_onMouseButtonPressed(_left);
//		if (!_left) return;
//		m_isPressed = true;
//		updateEditState();
	}

	void Edit::_onMouseButtonReleased(bool _left)
	{
		Widget::_onMouseButtonReleased(_left);
//		if (!_left) return;
//		m_isPressed = false;
//		updateEditState();
	}

	void Edit::_onKeySetFocus(WidgetPtr _old)
	{
		Widget::_onKeySetFocus(_old);
		m_isPressed = true;
		updateEditState();
	}

	void Edit::_onKeyLostFocus(WidgetPtr _new)
	{
		Widget::_onKeyLostFocus(_new);
		m_isPressed = false;
		updateEditState();
	}

	void Edit::_onKeyButtonPressed(int _key, wchar_t _char)
	{
		Widget::_onKeyButtonPressed(_key, _char);
		if (_key == OIS::KC_ESCAPE) InputManager::getInstance().setKeyFocusWidget(null);
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

	void Edit::_onKeyButtonReleased(int _key)
	{
		Widget::_onKeyButtonReleased(_key);
	}

} // namespace MyGUI