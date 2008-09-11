#ifndef __STRANGE_BUTTON_H__
#define __STRANGE_BUTTON_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	namespace factory{class StrangeButtonFactory;}

	class StrangeButton : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::StrangeButtonFactory;

	protected:
		StrangeButton(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const Ogre::String & _name);

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		inline void setButtonPressed(bool _pressed)
		{
			if (mIsStatePressed == _pressed) return;
			mIsStatePressed = _pressed;
			updateButtonState();
		}

		inline bool getButtonPressed() {return mIsStatePressed;}

	protected:

		virtual void _onMouseLostFocus(WidgetPtr _new);
		virtual void _onMouseSetFocus(WidgetPtr _old);
		virtual void _onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void _onMouseButtonReleased(int _left, int _top, MouseButton _id);

		inline void updateButtonState()
		{
			if (mIsFocus) {
				if (mIsPressed || mIsStatePressed) setState("select");
				else setState("active");
			} else {
				if (mIsPressed || mIsStatePressed) setState("pressed");
				else setState("normal");
			}
		}


	private:
		// нажата ли кнопка
		bool mIsPressed;
		// в фокусе ли кнопка
		bool mIsFocus;
		// статус кнопки нажата или нет
		bool mIsStatePressed;

	}; // class Button : public Widget

	typedef StrangeButton * StrangeButtonPtr;
}

#endif // __STRANGE_BUTTON_H__
