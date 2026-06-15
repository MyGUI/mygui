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
	class MYGUI_EXPORT Button : public TextBox, public MemberObsolete<Button>
	{
		MYGUI_RTTI_DERIVED(Button)

	public:
		//! Set button selected state
		void setStateSelected(bool _check);
		//! Get buton selected
		bool getStateSelected() const;

		/** Enable or disable Image mode\n
			Image mode: when button state changed Image on button also change it's picture.\n
			Disabled (false) by default.
		*/
		void setModeImage(bool _value);
		/** Get Image mode flag */
		bool getModeImage() const;

		void setImageResource(std::string_view _name);

		void setImageGroup(std::string_view _name);

		void setImageName(std::string_view _name);

		/*internal:*/
		void _setKeyFocus(bool _focus);
		void _setMouseFocus(bool _focus);

		ImageBox* _getImageBox() const;

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void onKeyLostFocus(Widget* _new) override;
		void onKeySetFocus(Widget* _old) override;
		void onMouseLostFocus(Widget* _new) override;
		void onMouseSetFocus(Widget* _old) override;
		void onMouseButtonPressed(int _left, int _top, MouseButton _id) override;
		void onMouseButtonReleased(int _left, int _top, MouseButton _id) override;

		void baseUpdateEnable() override;

		bool _setState(std::string_view _value);

		void setPropertyOverride(std::string_view _key, std::string_view _value) override;

	private:
		void updateButtonState();

	private:
		bool mIsMousePressed{false};
		bool mIsKeyFocus{false};
		bool mIsMouseFocus{false};
		// is fixed in pressed position
		bool mStateSelected{false};

		ImageBox* mImage{nullptr};
		bool mModeImage{false};
	};

} // namespace MyGUI

#endif // MYGUI_BUTTON_H_
