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

	class MYGUI_EXPORT Button : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<Button>;

		MYGUI_RTTI_CHILD_HEADER( Button, Widget );

	public:
		//! OLD Set button check state
		void setButtonPressed(bool _check) { setStateCheck(_check); }
		//! OLD Get buton check 
		bool getButtonPressed() { return getStateCheck(); }

		//! Set button check state
		void setStateCheck(bool _check)
		{
			if (mIsStateCheck == _check) return;
			mIsStateCheck = _check;
			updateButtonState();
		}

		//! Get buton check
		bool getStateCheck() {return mIsStateCheck;}

		//! Set image index (image should be defined in skin)
		void setImageIndex(size_t _index);
		//! Get image index
		size_t getImageIndex();

		/** Get pointer to glyph image for this button (if it exist in button skin) */
		StaticImagePtr getStaticImage() { return mImage; }

		//! @copydoc Widget::setEnabled(bool _enabled)
		virtual void setEnabled(bool _enabled);

	/*internal:*/
		void _setMouseFocus(bool _focus)
		{
			mIsMouseFocus = _focus;
			updateButtonState();
		}

	protected:
		Button(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~Button();

		virtual void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

		virtual void onMouseLostFocus(WidgetPtr _new);
		virtual void onMouseSetFocus(WidgetPtr _old);
		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);

	private:
		void updateButtonState();

		void shutdownWidgetSkin();
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);

	private:
		// нажата ли кнопка
		bool mIsMousePressed;
		// в фокусе ли кнопка
		bool mIsMouseFocus;
		// статус кнопки нажата или нет
		bool mIsStateCheck;

		StaticImagePtr mImage;

	}; // class Button : public Widget

} // namespace MyGUI

#endif // __MYGUI_BUTTON_H__
