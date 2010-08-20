/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_BUTTON_H__
#define __MYGUI_BUTTON_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_StaticText.h"

namespace MyGUI
{

	class MYGUI_EXPORT Button :
		public StaticText,
		public MemberObsolete<Button>
	{
		MYGUI_RTTI_DERIVED( Button )

	public:
		Button();

		//! Set button selected state
		void setStateSelected(bool _value);
		//! Get buton selected
		bool getStateSelected() { return mStateSelected; }

		//! Set image index (image should be defined in skin)
		void setImageIndex(size_t _value);
		//! Get image index
		size_t getImageIndex();

		/** Enable or disable Image mode\n
			Image mode: when button state changed Image on button also change it's picture.\n
			Disabled (false) by default.
		*/
		void setModeImage(bool _value);
		/** Get Image mode flag */
		bool getModeImage() { return mModeImage; }

		/** Get pointer to glyph image for this button (if it exist in button skin) */
		StaticImage* getStaticImage() { return mImage; }

	/*internal:*/
		void _setMouseFocus(bool _focus);

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		virtual void onMouseLostFocus(Widget* _new);
		virtual void onMouseSetFocus(Widget* _old);
		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);

		virtual void baseUpdateEnable();

		bool _setState(const std::string& _value);
		void setImageResource(const std::string& _name);

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		void updateButtonState();

	private:
		// нажата ли кнопка
		bool mIsMousePressed;
		// в фокусе ли кнопка
		bool mIsMouseFocus;
		// статус кнопки нажата или нет
		bool mStateSelected;

		StaticImage* mImage;
		bool mModeImage;

	};

} // namespace MyGUI

#endif // __MYGUI_BUTTON_H__
