/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WINDOW_H__
#define __MYGUI_WINDOW_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WindowFactory.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class Window;
	typedef Window * WindowPtr;

	class _MyGUIExport Window : public Widget, public Ogre::FrameListener
	{
		// ��� ������ ��������� ������������
		friend factory::WindowFactory;

	protected:
		Window(int _left, int _top, int _width, int _height, Align _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);
		~Window();

	public:
		// �������������� ��� ���������� �������
		virtual WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name = "");

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		void show(bool _smoot = true, bool _reset = true);
		void hide(bool _smoot = true, bool _destroy = false);

		inline bool getAutoAlpha() {return mIsAutoAlpha;}
		inline void setAutoAlpha(bool _auto) {mIsAutoAlpha=_auto; if(!_auto)setAlpha(1.0f);else updateAlpha();}

		virtual void setCaption(const Ogre::DisplayString & _caption) {if (mWidgetCaption!=null)mWidgetCaption->setCaption(_caption);};
		virtual const Ogre::DisplayString & getCaption() {if (mWidgetCaption!=null) return mWidgetCaption->getCaption();return Widget::getCaption();};

		void setMinMax(IntRect _minmax) {mMinmax = _minmax;}
		IntRect getMinMax() {return mMinmax;}

		virtual void move(int _left, int _top);
		virtual void move(int _left, int _top, int _width, int _height);
		virtual void size(int _width, int _height);

		inline bool getIsToStick() {return mIsToStick;}
		inline void setIsToStick(bool _stick) {mIsToStick = _stick;}

		// ����� ������� �� ����
		EventSimple eventWindowXPressed;

	protected:

		virtual void _onMouseChangeRootFocus(bool _focus);
		virtual void _onKeyChangeRootFocus(bool _focus);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMousePressedX(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMovedCaption(MyGUI::WidgetPtr _sender, int _left, int _top);
		void notifyMouseMovedResize(MyGUI::WidgetPtr _sender, int _left, int _top);

		void setDoAlpha(float _alpha);

		// ������ ��������� ����� ������������ �� ������
		void updateAlpha();

	private:
		WidgetPtr mWidgetCaption, mWidgetX, mWidgetResize, mWidgetClient;

		// ������� ���� ����� ������� ��� ���������
		IntRect mPreActionRect;

		// ���������� �� �� �� ����� �����
		bool mIsListenerAlpha;
		// �����, ������� �� �������� �������
		float mDoAlpha;
		// ���������� �� ����� �������
		bool mIsDestroy;

		// ���� ������� ������
		bool mMouseRootFocus;
		bool mKeyRootFocus;

		// �������������� ��� ������ ���������� ������
		bool mIsAutoAlpha;

		// ����������� � ������������ ������� ����
		IntRect mMinmax;

		bool mIsToStick; // ���������� �� � �����


	}; // class Window : public Widget

} // namespace MyGUI

#endif // __MYGUI_WINDOW_H__