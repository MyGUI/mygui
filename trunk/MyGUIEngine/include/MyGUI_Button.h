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
#include "MyGUI_TextBox.h"

namespace MyGUI
{

	/** \brief @wpage{Button}
		Button widget description should be here.
	*/
	class MYGUI_EXPORT Button :
		public TextBox,
		public MemberObsolete<Button>
	{
		MYGUI_RTTI_DERIVED( Button )

	public:
		Button();

		//! Set button selected state
		void setStateSelected(bool _value);
		//! Get buton selected
		bool getStateSelected() const;

		/** Enable or disable Image mode\n
			Image mode: when button state changed Image on button also change it's picture.\n
			Disabled (false) by default.
		*/
		void setModeImage(bool _value);
		/** Get Image mode flag */
		bool getModeImage() const;

		void setImageResource(const std::string& _name);

		void setImageGroup(const std::string& _name);

		void setImageName(const std::string& _name);

		/*internal:*/
		void _setMouseFocus(bool _focus);

		ImageBox* _getImageBox();

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		virtual void onMouseLostFocus(Widget* _new);
		virtual void onMouseSetFocus(Widget* _old);
		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);

		virtual void baseUpdateEnable();

		bool _setState(const std::string& _value);

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

		ImageBox* mImage;
		bool mModeImage;
	};

} // namespace MyGUI

#endif // __MYGUI_BUTTON_H__
