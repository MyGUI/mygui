#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Widget.h"

namespace MyGUI
{

	class _MyGUIExport Window : public Widget, public Ogre::FrameListener
	{
		// ��� ������ ��������� ������������
		friend WidgetFactory<Window>;

	protected:
		Window(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("Window"); return type;};

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		void show(bool _smoot = true, bool _reset = true);
		void hide(bool _smoot = true, bool _destroy = false);

		inline bool getAutoAlpha() {return m_bIsAutoAlpha;}
		inline void setAutoAlpha(bool _auto) {m_bIsAutoAlpha=_auto; if(!_auto)setAlpha(1.0f);else updateAlpha();}

		virtual void setCaption(const Ogre::DisplayString & _caption) {if (mWidgetCaption!=null)mWidgetCaption->setCaption(_caption);};
		virtual const Ogre::DisplayString & getCaption() {if (mWidgetCaption!=null) return mWidgetCaption->getCaption();return Widget::getCaption();};

		void setMinMax(IntRect _minmax) {m_minmax = _minmax;}
		IntRect getMinMax() {return m_minmax;}

		virtual void size(int _cx, int _cy);

		// ����� ������� �� ����
		EventSimple eventWindowXPressed;

	protected:

		virtual void _onMouseChangeRootFocus(bool _focus);
		virtual void _onKeyChangeRootFocus(bool _focus);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMousePressedX(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMovedCaption(MyGUI::WidgetPtr _sender, int _x, int _y);
		void notifyMouseMovedResize(MyGUI::WidgetPtr _sender, int _x, int _y);

		void setDoAlpha(float _alpha);

		// ������ ��������� ����� ������������ �� ������
		void updateAlpha();

		inline void _showWindowCaption(bool _show);
		inline void _showWindowX(bool _show);
		inline void _showWindowResize(bool _show);

	private:
		std::string mSkinCaption, mSkinX, mSkinResize;
		WidgetPtr mWidgetCaption, mWidgetX, mWidgetResize;
		FloatRect mOffsetCaption, mOffsetX, mOffsetResize;
		char mAlignCaption, mAlignX, mAlignResize;

		IntSize mSkinSize;

		// ������� ���� ����� ������� ��� ���������
		IntRect m_preActionRect;

		// ���������� �� �� �� ����� �����
		bool m_bIsListenerAlpha;
		// �����, ������� �� �������� �������
		float m_doAlpha;
		// ���������� �� ����� �������
		bool m_isDestroy;

		// ���� ������� ������
		bool m_mouseRootFocus;
		bool m_keyRootFocus;

		// �������������� ��� ������ ���������� ������
		bool m_bIsAutoAlpha;

		// ����������� � ������������ ������� ����
		IntRect m_minmax;


	}; // class Window : public Widget

	typedef Window * WindowPtr;

} // namespace MyGUI

#endif // _WINDOW_H_