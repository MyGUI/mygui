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
		friend class factory::BaseWidgetFactory<Button>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		Button(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

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

		//! Set button focused state
		void _setMouseFocus(bool _focus)
		{
			mIsMouseFocus = _focus;
			updateButtonState();
		}

		//! Set image index (image should be defined in skin)
		void setImageIndex(size_t _index);
		//! Get image index
		size_t getImageIndex();

		StaticImagePtr getStaticImage() { return mImage; }

		//! @copydoc Widget::setEnabled(bool _enabled)
		virtual void setEnabled(bool _enabled);

	protected:

		virtual void onMouseLostFocus(WidgetPtr _new);
		virtual void onMouseSetFocus(WidgetPtr _old);
		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);

		void updateButtonState();


	private:
		// ������ �� ������
		bool mIsMousePressed;
		// � ������ �� ������
		bool mIsMouseFocus;
		// ������ ������ ������ ��� ���
		bool mIsStateCheck;

		StaticImagePtr mImage;

		// ��� ���������� ��������� ������� ������
		bool mModeCheck;

	}; // class Button : public Widget

} // namespace MyGUI

#endif // __MYGUI_BUTTON_H__
