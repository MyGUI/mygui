/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/
#ifndef __WORD_WRAP_PANEL_H__
#define __WORD_WRAP_PANEL_H__

#include "MyGUI_Prerequest.h"
#include "Panel.h"

namespace MyGUI
{

	class WordWrapPanel :
		public Panel
	{
		MYGUI_RTTI_DERIVED( WordWrapPanel )

	public:
		WordWrapPanel();

		//! @copydoc Widget::setPosition(const IntPoint& _value)
		virtual void setPosition(const IntPoint& _value);
		//! @copydoc Widget::setSize(const IntSize& _value)
		virtual void setSize(const IntSize& _value);
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		virtual void setCoord(const IntCoord& _value);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top);
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height);
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height);

		int getHeightByWidth(int _width);

	protected:
		// еще нужен метод удаления
		virtual void onWidgetCreated(Widget* _widget);

	private:
		void onSizeChanged(const IntSize& _size);
		void updateContent();

		void alignChildLine(size_t _startIndex, size_t _stopIndex, int _top, int _height);
		IntSize getWidgetSize(Widget* _widget);

	private:
		int mOldWidth;
		int mCalcHeght;
	};

} // namespace MyGUI

#endif // __WORD_WRAP_PANEL_H__
