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
		virtual ~ScrollViewBase() = default;

		void updateScrollSize();
		void updateScrollPosition();

		virtual IntSize getContentSize() const;

		virtual IntPoint getContentPosition() const;
		virtual void setContentPosition(const IntPoint& _value);

		virtual IntSize getViewSize() const;

		// Scroll change value, applied when clicking on a scroll
		virtual size_t getHScrollPage() const;
		virtual size_t getVScrollPage() const;

		virtual Align getContentAlign() const;

		virtual void eraseContent();

	protected:
		ScrollBar* mVScroll{nullptr};
		ScrollBar* mHScroll{nullptr};
		Widget* mScrollViewClient{nullptr};

		bool mVisibleHScroll{true};
		bool mVisibleVScroll{true};

		size_t mVRange{0};
		size_t mHRange{0};

		// Whether content changes on resize
		bool mChangeContentByResize{false};
	};

} // namespace MyGUI

#endif // MYGUI_SCROLL_VIEW_BASE_H_
