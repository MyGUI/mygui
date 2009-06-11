/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
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
		public StaticText
	{
		MYGUI_RTTI_DERIVED( Button );

	public:
		Button();

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
		bool getStateCheck() { return mIsStateCheck; }

		//! Set image index (image should be defined in skin)
		void setImageIndex(size_t _index);
		//! Get image index
		size_t getImageIndex();

		/** Get pointer to glyph image for this button (if it exist in button skin) */
		StaticImagePtr getStaticImage() { return mImage; }

		//! @copydoc Widget::setEnabled(bool _enabled)
		virtual void setEnabled(bool _enabled);

		/** @copydoc Widget::setProperty(const std::string& _key, const std::string& _value) */
		virtual void setProperty(const std::string& _key, const std::string& _value);

	/*internal:*/
		virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name);

		void _setMouseFocus(bool _focus)
		{
			mIsMouseFocus = _focus;
			updateButtonState();
		}

	protected:
		Button(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name);
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
