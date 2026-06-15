/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_WINDOW_H_
#define MYGUI_WINDOW_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_ControllerFadeAlpha.h"

namespace MyGUI
{

	// OBSOLETE
	using EventHandle_WidgetString = EventPairConvertStringView<
		delegates::MultiDelegate<Widget*, const std::string&>,
		delegates::MultiDelegate<Widget*, std::string_view>>;

	using EventHandle_WindowPtrCStringRef = EventPairConvertStringView<
		delegates::MultiDelegate<Window*, const std::string&>,
		delegates::MultiDelegate<Window*, std::string_view>>;
	using EventHandle_WindowPtr = delegates::MultiDelegate<Window*>;

	/** \brief @wpage{Window}
		Window widget description should be here.
	*/
	class MYGUI_EXPORT Window :
		public TextBox, // FIXME пока для кэпшена вместо виджета текст (Bug #190)
		public MemberObsolete<Window>
	{
		MYGUI_RTTI_DERIVED(Window)

	public:
		/** @copydoc Widget::setVisible */
		void setVisible(bool _visible) override;

		/** Hide or show window smooth */
		void setVisibleSmooth(bool _visible);
		/** Hide window smooth and then destroy it */
		void destroySmooth();

		/** Enable or disable auto alpha mode */
		void setAutoAlpha(bool _auto);
		/** Get auto alpha mode flag */
		bool getAutoAlpha() const;

		/** Set window caption */
		void setCaption(const UString& _caption) override;
		/** Get window caption */
		const UString& getCaption() const override;

		/** Get window caption widget */
		TextBox* getCaptionWidget() const;

		/** Set minimal possible window size */
		void setMinSize(const IntSize& _value);
		/** Set minimal possible window size */
		void setMinSize(int _width, int _height);
		/** Get minimal possible window size */
		IntSize getMinSize() const;

		/** Set maximal possible window size */
		void setMaxSize(const IntSize& _value);
		/** Set maximal possible window size */
		void setMaxSize(int _width, int _height);
		/** Get maximal possible window size */
		IntSize getMaxSize() const;

		//! @copydoc Widget::setPosition(const IntPoint& _point)
		void setPosition(const IntPoint& _point) override;
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size) override;
		//! @copydoc Widget::setCoord(const IntCoord& _coord)
		void setCoord(const IntCoord& _coord) override;

		using Widget::setPosition;
		using Widget::setSize;
		using Widget::setCoord;

		/** Enable or disable snap to borders mode */
		void setSnap(bool _value);
		/** Get snap to borders mode flag */
		bool getSnap() const;

		/** Get current action applied to move/resize window. */
		const IntCoord& getActionScale() const;

		/** Enable or disable possibility to move window. */
		void setMovable(bool _value);
		/** Get possibility to move window. */
		bool getMovable() const;

		/*events:*/
		/** Event : Window button pressed.\n
			signature : void method(MyGUI::Window* _sender, std::string_view _name)
			@param _sender widget that called this event
			@param _name of pressed button
		*/
		EventPair<EventHandle_WidgetString, EventHandle_WindowPtrCStringRef> eventWindowButtonPressed;

		/** Event : Window coordinate changed (window was resized or moved).\n
			signature : void method(MyGUI::Window* _sender)
			@param _sender widget that called this event
		*/
		EventPair<EventHandle_WidgetVoid, EventHandle_WindowPtr> eventWindowChangeCoord;

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void onMouseChangeRootFocus(bool _focus) override;
		void onKeyChangeRootFocus(bool _focus) override;
		void onMouseDrag(int _left, int _top, MouseButton _id) override;
		void onMouseButtonPressed(int _left, int _top, MouseButton _id) override;
		void onMouseButtonReleased(int _left, int _top, MouseButton _id) override;

		void notifyMousePressed(MyGUI::Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(MyGUI::Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyPressedButtonEvent(MyGUI::Widget* _sender);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseWheel(MyGUI::Widget* _sender, int _rel);

		// просто обновляет альфу взависимости от флагов
		void updateAlpha();

		void animateStop(Widget* _widget, ControllerItem* _controller);

		void setPropertyOverride(std::string_view _key, std::string_view _value) override;

	private:
		float getAlphaVisible() const;
		enum class Snap
		{
			Position,
			Size
		};
		void getSnappedCoord(IntCoord& _coord, Snap snapMode) const;
		IntCoord _getActionScale(Widget* _widget) const;

		ControllerFadeAlpha* createControllerFadeAlpha(float _alpha, float _coef, bool _enable);

	private:
		TextBox* mWidgetCaption{nullptr};

		// размеры окна перед началом его изменений
		IntCoord mPreActionCoord;

		// наши главные фокусы
		bool mMouseRootFocus{false};
		bool mKeyRootFocus{false};

		// автоматическое или ручное управление альфой
		bool mIsAutoAlpha{false};

		// минимальные и максимальные размеры окна
		IntRect mMinmax;

		bool mSnap{false}; // прилеплять ли к краям

		IntCoord mCurrentActionScale;
		bool mAnimateSmooth{false};

		bool mMovable{true};
	};

} // namespace MyGUI

#endif // MYGUI_WINDOW_H_
