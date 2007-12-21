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
		friend class factory::StretchRectangleFactory;

	protected:
		enum {
			BORDER_LEFT,
			BORDER_TOP,
			BORDER_RIGHT,
			BORDER_BOTTOM,
			BORDER_COUNT
		};

		enum {
			ACTION_LEFT_TOP,
			ACTION_TOP,
			ACTION_RIGHT_TOP,
			ACTION_RIGHT,
			ACTION_RIGHT_BOTTOM,
			ACTION_BOTTOM,
			ACTION_LEFT_BOTTOM,
			ACTION_LEFT,
			ACTION_COUNT
		};

	protected:
		StretchRectangle(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// тип данного виджета
		inline static const Ogre::String & getType() {static Ogre::String type("StretchRectangle"); return type;}

		inline void setGridStep(int _grid_step) {grid_step = (_grid_step>0)?_grid_step:1;}
		inline int getGridStep() {return grid_step;}
		inline void setWidgetPressed(bool _pressed)
		{
			if (mIsStatePressed == _pressed) return;
			mIsStatePressed = _pressed;
			updateWidgetState();
		}

	protected:

		void _onMouseLostFocus(WidgetPtr _new);
		void _onMouseSetFocus(WidgetPtr _old);
		void _onMouseButtonPressed(bool _left);
		void _onMouseMove(int _left, int _top);

		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);


		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMove(MyGUI::WidgetPtr _sender, int _left, int _top);


		void updateWidgetState();


	private:
		// нажата ли кнопка
		bool mIsPressed;
		// в фокусе ли кнопка
		bool mIsFocus;
		// статус кнопки нажата или нет
		bool mIsStatePressed;

		// размеры окна перед началом его изменений
		IntCoord mPreActionCoord;

		// рамка по краям виджета
		WidgetPtr mWidgetBorder[BORDER_COUNT];

		// квадратики для изменений размеров
		ButtonPtr mWidgetAction[ACTION_COUNT];

		// шаг сетки при изменении размеров элемента
		int grid_step;

	}; // class StretchRectangle : public Widget

} // namespace MyGUI

#endif // __MYGUI_STRETCH_RECTANGLE_H__