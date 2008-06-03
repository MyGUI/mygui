/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_BUTTON_H__
#define __MYGUI_BUTTON_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class _MyGUIExport Button : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::ButtonFactory;

	protected:
		Button(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		//! Set button pressed state
		inline void setButtonPressed(bool _pressed)
		{
			if (mIsStatePressed == _pressed) return;
			mIsStatePressed = _pressed;
			updateButtonState();
		}

		//! Get buton pressed
		inline bool getButtonPressed() {return mIsStatePressed;}

		//! Set button focused state
		inline void _setMouseFocus(bool _focus)
		{
			mIsFocus = _focus;
			updateButtonState();
		}

		void setImageIndex(size_t _index);

		virtual void setEnabled(bool _enabled);

	protected:

		virtual void _onMouseLostFocus(WidgetPtr _new);
		virtual void _onMouseSetFocus(WidgetPtr _old);
		virtual void _onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void _onMouseButtonReleased(int _left, int _top, MouseButton _id);

		void updateButtonState();


	private:
		// нажата ли кнопка
		bool mIsPressed;
		// в фокусе ли кнопка
		bool mIsFocus;
		// статус кнопки нажата или нет
		bool mIsStatePressed;

		StaticImagePtr mImage;

	}; // class Button : public Widget

} // namespace MyGUI

#endif // __MYGUI_BUTTON_H__
