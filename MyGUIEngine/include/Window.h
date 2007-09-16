#pragma once

#include "Widget.h"

namespace MyGUI
{

	class Window : public Widget
	{
		// для вызова закрытого конструктора
		friend WidgetFactory<Window>;

	protected:
		Window(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("Window"); return type;};

		void showWindowCaption(bool _show);
		void showWindowX(bool _show);
		void showWindowResize(bool _show);


	protected:

		virtual void OnMouseLostFocus(WidgetPtr _new);
		virtual void OnMouseSetFocus(WidgetPtr _old);
		virtual void OnMouseButtonPressed(bool _left);
		virtual void OnMouseButtonReleased(bool _left);


	private:
		std::string mSkinCaption;
		std::string mSkinX;
		std::string mSkinResize;
		bool mShowSkinCaption;
		bool mShowSkinX;
		bool mShowSkinResize;

	}; // class Window : public Widget

	typedef Window * WindowPtr;

} // namespace MyGUI