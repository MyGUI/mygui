/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_STRETCH_RECTANGLE_H__
#define __MYGUI_STRETCH_RECTANGLE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_StretchRectangleFactory.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Button.h"

namespace MyGUI
{

	class StretchRectangle;
	typedef StretchRectangle* StretchRectanglePtr;

	class _MyGUIExport StretchRectangle : public Widget
	{
		// для вызова закрытого конструктора
		friend factory::StretchRectangleFactory;

	protected:
		StretchRectangle(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// тип данного виджета
		inline static const Ogre::String & getType() {static Ogre::String type("StretchRectangle"); return type;}

		inline void setWidgetPressed(bool _pressed)
		{
			if (mIsStatePressed == _pressed) return;
			mIsStatePressed = _pressed;
			updateWidgetState();
		}

		inline bool isChild(WidgetPtr _widget)
		{
			return ((mLeftBorder == _widget) ||
				(mTopBorder == _widget) ||
				(mRightBorder == _widget) ||
				(mBottomBorder == _widget) ||

				(mActionLeft == _widget) ||
				(mActionRight == _widget) ||
				(mActionTop == _widget) ||
				(mActionBottom == _widget) ||

				(mActionLeftTop == _widget) ||
				(mActionLeftBottom == _widget) ||
				(mActionRightTop == _widget) ||
				(mActionRightBottom == _widget));
		}

	protected:

		virtual void _onMouseLostFocus(WidgetPtr _new);
		virtual void _onMouseSetFocus(WidgetPtr _old);
		virtual void _onKeyLostFocus(WidgetPtr _new);
		virtual void _onKeySetFocus(WidgetPtr _old);

		virtual void _onMouseButtonPressed(bool _left);

		void notifyKeyLostFocus(WidgetPtr _sender, WidgetPtr _new);


		void updateWidgetState();


	private:
		// нажата ли кнопка
		bool mIsPressed;
		// в фокусе ли кнопка
		bool mIsFocus;
		// статус кнопки нажата или нет
		bool mIsStatePressed;

		// рамка по краям виджета
		WidgetPtr mLeftBorder, mTopBorder, mRightBorder, mBottomBorder;

		// квадратики для изменений размеров
		ButtonPtr mActionLeft, mActionRight, mActionTop, mActionBottom;
		ButtonPtr mActionLeftTop, mActionLeftBottom, mActionRightTop, mActionRightBottom;

	}; // class StretchRectangle : public Widget

} // namespace MyGUI

#endif // __MYGUI_STRETCH_RECTANGLE_H__