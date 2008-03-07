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
		// ��� ������ ��������� ������������
		friend class factory::ButtonFactory;

	protected:
		Button(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// ��� ������� �������
		inline static const Ogre::String & _getType() {static Ogre::String type("Button"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		inline void setButtonPressed(bool _pressed)
		{
			if (mIsStatePressed == _pressed) return;
			mIsStatePressed = _pressed;
			updateButtonState();
		}

		inline bool getButtonPressed() {return mIsStatePressed;}

		inline void _setMouseFocus(bool _focus)
		{
			mIsFocus = _focus;
			updateButtonState();
		}

	protected:

		virtual void _onMouseLostFocus(WidgetPtr _new);
		virtual void _onMouseSetFocus(WidgetPtr _old);
		virtual void _onMouseButtonPressed(bool _left);
		virtual void _onMouseButtonReleased(bool _left);

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
		// ������ �� ������
		bool mIsPressed;
		// � ������ �� ������
		bool mIsFocus;
		// ������ ������ ������ ��� ���
		bool mIsStatePressed;

	}; // class Button : public Widget

} // namespace MyGUI

#endif // __MYGUI_BUTTON_H__
