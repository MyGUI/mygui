
#include "Edit.h"
#include "EditParser.h"

namespace MyGUI
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<Edit> EditFactoryInstance("Edit"); }
	// парсер команд
	namespace parser { EditParser EditParserInstance; }

	const float CURSOR_TIMER = 0.7f;

	Edit::Edit(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		m_isPressed(false),
		m_isFocus(false),
		mTimer(0),
		mCursorActive(false)
	{

		ASSERT(m_text);

		// создаем прослойку, чтобы курсор был над текстом
		mWidgetUpper = createWidget("Widget", "Empty", m_text->left(), m_text->top(), m_text->width(), m_text->height(), m_text->getAlign());
		mWidgetUpper->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
		mWidgetUpper->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
		mWidgetUpper->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);

		// парсим свойства
		const SkinParam & param = _info->getParams();
		SkinParam::const_iterator iter = param.find("CursorSkin");
		String skin = "Default";
		if (iter != param.end()) skin = iter->second;

		// создаем курсор
		WidgetSkinInfo * info = SkinManager::getInstance().getSkin(skin);
		mWidgetCursor = mWidgetUpper->createWidget("Widget", skin, 0, 0, info->getSize().width, info->getSize().height, ALIGN_LEFT | ALIGN_TOP);
		mWidgetCursor->show(false);
		mWidgetCursor->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
		mWidgetCursor->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
		mWidgetCursor->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);


//		m_text->setTextSelect(10, 20);
	}

	void Edit::notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
	{
		if ( (_old == this) || (_old == mWidgetUpper) || (_old == mWidgetCursor) || (m_isFocus) ) return;
		m_isFocus = true;
		updateEditState();
	}

	void Edit::notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		if ( (_new == this) || (_new == mWidgetUpper) || (_new == mWidgetCursor) || (!m_isFocus) ) return;
		m_isFocus = false;
		updateEditState();
	}

	void Edit::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		InputManager::getInstance().setKeyFocusWidget(this);
	}

	void Edit::_onMouseSetFocus(WidgetPtr _old)
	{
		Widget::_onMouseSetFocus(_old);
		if (m_isFocus) return;
		m_isFocus = true;
		updateEditState();
	}

	void Edit::_onMouseLostFocus(WidgetPtr _new)
	{
		Widget::_onMouseLostFocus(_new);
		if ( (!m_isFocus) || (_new == mWidgetUpper) || (_new == mWidgetCursor) ) return;
		m_isFocus = false;
		updateEditState();
	}

	void Edit::_onKeySetFocus(WidgetPtr _old)
	{
		Widget::_onKeySetFocus(_old);
		if (m_isPressed) return;
		m_isPressed = true;
		updateEditState();

		mCursorActive = true;
		Ogre::Root::getSingleton().addFrameListener(this);
		mWidgetCursor->show(true);
		mTimer = 0;
	}

	void Edit::_onKeyLostFocus(WidgetPtr _new)
	{
		Widget::_onKeyLostFocus(_new);
		if ( ! m_isPressed) return;
		m_isPressed = false;
		updateEditState();

		mCursorActive = false;
		Ogre::Root::getSingleton().removeFrameListener(this);
		mWidgetCursor->show(false);
	}

	void Edit::_onKeyButtonPressed(int _key, wchar_t _char)
	{
		Widget::_onKeyButtonPressed(_key, _char);
		ASSERT(m_text);

		if ( ! mWidgetCursor->isShow()) mWidgetCursor->show(true);
		mTimer = 0.0f;

		if (_key == OIS::KC_ESCAPE) InputManager::getInstance().setKeyFocusWidget(null);
		else if (_key == OIS::KC_BACK) {
			const Ogre::DisplayString & text = m_text->getCaption();
			if (!text.empty()) m_text->setCaption(text.substr(0, text.length()-1)); 
		} else if (_key == OIS::KC_RETURN) {
			const Ogre::DisplayString & text = m_text->getCaption();
			m_text->setCaption(text + "\n"); 
		} else if (_char != 0) {
			const Ogre::DisplayString & text = m_text->getCaption();
			m_text->setCaption(text + _char); 
		}
	}

	void Edit::_onKeyButtonReleased(int _key)
	{
		Widget::_onKeyButtonReleased(_key);
	}

	bool Edit::frameStarted(const Ogre::FrameEvent& evt)
	{
		if (!mCursorActive) return true;
		mTimer += evt.timeSinceLastFrame;

		if (mTimer <= CURSOR_TIMER) return true;

		mWidgetCursor->show( ! mWidgetCursor->isShow());
		mTimer -= CURSOR_TIMER;
		return true;
	}

	bool Edit::frameEnded(const Ogre::FrameEvent& evt)
	{
		return true;
	}

} // namespace MyGUI