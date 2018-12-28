/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SCROLL_VIEW_H_
#define MYGUI_SCROLL_VIEW_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_ScrollViewBase.h"

namespace MyGUI
{

	/** \brief @wpage{ScrollView}
		ScrollView widget description should be here.
	*/
	class MYGUI_EXPORT ScrollView :
		public Widget,
		protected ScrollViewBase,
		public MemberObsolete<ScrollView>
	{
		MYGUI_RTTI_DERIVED( ScrollView )

	public:
		ScrollView();

		//! @copydoc Widget::setPosition(const IntPoint& _value)
		void setPosition(const IntPoint& _value) override;
		//! @copydoc Widget::setSize(const IntSize& _value)
		void setSize(const IntSize& _value) override;
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		void setCoord(const IntCoord& _value) override;

		using Widget::setPosition;
		using Widget::setSize;
		using Widget::setCoord;

		/** Show VScroll when content size larger than view */
		void setVisibleVScroll(bool _value);
		/** Get Show VScroll flag */
		bool isVisibleVScroll() const;

		/** Show HScroll when content size larger than view */
		void setVisibleHScroll(bool _value);
		/** Get Show HScroll flag */
		bool isVisibleHScroll() const;

		/** Set canvas align */
		void setCanvasAlign(Align _value);
		/** Get canvas align */
		Align getCanvasAlign() const;

		/** Set canvas size */
		void setCanvasSize(const IntSize& _value);
		/** Set canvas size */
		void setCanvasSize(int _width, int _height);
		/** Get canvas size */
		IntSize getCanvasSize();

		/** Get view area coordinates. */
		IntCoord getViewCoord() const;

		/** Set view area offset. */
		void setViewOffset(const IntPoint& _value);
		/** Get view area offset. */
		IntPoint getViewOffset() const;

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void notifyScrollChangePosition(ScrollBar* _sender, size_t _position);
		void notifyMouseWheel(Widget* _sender, int _rel);

		void updateView();

		void setPropertyOverride(const std::string& _key, const std::string& _value) override;

		ScrollBar* getVScroll();

	private:
		// размер данных
		IntSize getContentSize() override;
		// смещение данных
		IntPoint getContentPosition() override;
		// размер окна, через которые видно данные
		IntSize getViewSize() override;
		void setContentPosition(const IntPoint& _point) override;
		// размер на который прокручиваются данные при щелчке по скролу
		size_t getVScrollPage() override;
		size_t getHScrollPage() override;

		Align getContentAlign() override;

	protected:
		Align mContentAlign;
	};

} // namespace MyGUI

#endif // MYGUI_SCROLL_VIEW_H_
