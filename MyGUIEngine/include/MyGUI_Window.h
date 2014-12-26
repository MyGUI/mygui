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
	typedef delegates::CMultiDelegate2<Widget*, const std::string&> EventHandle_WidgetString;

	typedef delegates::CMultiDelegate2<Window*, const std::string&> EventHandle_WindowPtrCStringRef;
	typedef delegates::CMultiDelegate1<Window*> EventHandle_WindowPtr;

	/** \brief @wpage{Window}
		Window widget description should be here.
	*/
	class MYGUI_EXPORT Window :
		public TextBox, // FIXME пока для кэпшена вместо виджета текст (Bug #190)
		public MemberObsolete<Window>
	{
		MYGUI_RTTI_DERIVED( Window )

	public:
		Window();

		/** @copydoc Widget::setVisible */
		virtual void setVisible(bool _value);

		/** Hide or show window smooth */
		void setVisibleSmooth(bool _value);
		/** Hide window smooth and then destroy it */
		void destroySmooth();

		/** Enable or disable auto alpha mode */
		void setAutoAlpha(bool _value);
		/** Get auto alpha mode flag */
		bool getAutoAlpha() const;

		/** Set window caption */
		virtual void setCaption(const UString& _value);
		/** Get window caption */
		virtual const UString& getCaption();

		/** Get window caption widget */
		TextBox* getCaptionWidget();

		/** Set minimal possible window size */
		void setMinSize(const IntSize& _value);
		/** Set minimal possible window size */
		void setMinSize(int _width, int _height);
		/** Get minimal possible window size */
		IntSize getMinSize();

		/** Set maximal possible window size */
		void setMaxSize(const IntSize& _value);
		/** Set maximal possible window size */
		void setMaxSize(int _width, int _height);
		/** Get maximal possible window size */
		IntSize getMaxSize();

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
			signature : void method(MyGUI::Window* _sender, const std::string& _name)
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
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		void onMouseChangeRootFocus(bool _focus);
		void onKeyChangeRootFocus(bool _focus);
		void onMouseDrag(int _left, int _top, MouseButton _id);
		void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		void onMouseButtonReleased(int _left, int _top, MouseButton _id);

		void notifyMousePressed(MyGUI::Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(MyGUI::Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyPressedButtonEvent(MyGUI::Widget* _sender);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseWheel(MyGUI::Widget* _sender, int _rel);

		// просто обновляет альфу взависимости от флагов
		void updateAlpha();

		void animateStop(Widget* _widget, ControllerItem* _controller);

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		float getAlphaVisible() const;
		void getSnappedCoord(IntCoord& _coord);
		IntCoord _getActionScale(Widget* _widget);

		ControllerFadeAlpha* createControllerFadeAlpha(float _alpha, float _coef, bool _enable);

	private:
		TextBox* mWidgetCaption;

		// размеры окна перед началом его изменений
		IntCoord mPreActionCoord;

		// наши главные фокусы
		bool mMouseRootFocus;
		bool mKeyRootFocus;

		// автоматическое или ручное управление альфой
		bool mIsAutoAlpha;

		// минимальные и максимальные размеры окна
		IntRect mMinmax;

		bool mSnap; // прилеплять ли к краям

		IntCoord mCurrentActionScale;
		bool mAnimateSmooth;

		Widget* mClient;
		bool mMovable;
	};

} // namespace MyGUI

#endif // MYGUI_WINDOW_H_
