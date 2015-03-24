/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SCROLL_VIEW_BASE_H_
#define MYGUI_SCROLL_VIEW_BASE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class MYGUI_EXPORT ScrollViewBase
	{
	protected:
		ScrollViewBase();
		virtual ~ScrollViewBase();

		void updateScrollSize();
		void updateScrollPosition();

		// размер данных
		virtual IntSize getContentSize();

		// смещение данных
		virtual IntPoint getContentPosition();
		virtual void setContentPosition(const IntPoint& _value);

		// размер окна, через которые видно данные
		virtual IntSize getViewSize();

		// размер на который прокручиваются данные при щелчке по скролу
		virtual size_t getHScrollPage();
		virtual size_t getVScrollPage();

		virtual Align getContentAlign();

		virtual void eraseContent();

	protected:
		ScrollBar* mVScroll;
		ScrollBar* mHScroll;
		Widget* mClient;

		bool mVisibleHScroll;
		bool mVisibleVScroll;

		size_t mVRange;
		size_t mHRange;

		// изменяется ли содержимое при ресайзе
		bool mChangeContentByResize;
	};

} // namespace MyGUI

#endif // MYGUI_SCROLL_VIEW_BASE_H_
