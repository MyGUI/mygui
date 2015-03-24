/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_BUTTON_H_
#define MYGUI_BUTTON_H_

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

#endif // MYGUI_BUTTON_H_
