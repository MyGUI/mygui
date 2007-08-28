#pragma once

#include <vector>
#include <Ogre.h>
#include "MainSkin.h"
#include "delegate.h"

#include "WidgetSkinInfo.h"
#include "LayerItemInfo.h"

namespace widget
{
	using namespace Ogre;

	class WidgetManager;
	class Gui;

	class Widget : public BasisWidget , public LayerItemInfo
	{
		// ��� ������ �������� ������������
		// ������ �������� � ������
		friend WidgetManager;
		// ��� ��� ���� ����� ���������� m_overlay
		friend Gui;

	public:
		// ��� �������� ������ ����� �������
		Widget(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

		// ������� �����
		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _name = "");
		WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _x, float _y, float _cx, float _cy, char _align, const Ogre::String & _name = "");

		void move(int _x, int _y);
		void move(int _x, int _y, int _cx, int _cy);
		void size(int _cx, int _cy);

		void show(bool _show);

		virtual void setCaption(const Ogre::DisplayString & _caption);
		virtual const Ogre::DisplayString & getCaption();

		void setColour(const Ogre::ColourValue & _color);
		const Ogre::ColourValue & getColour() {return m_color;};

		void setFontName(const Ogre::String & _font);
		void setFontName(const Ogre::String & _font, Ogre::ushort _height);
		const Ogre::String & getFontName();

		void setCharHeight(Ogre::ushort _height);
		Ogre::ushort getCharHeight();

		void setAlpha(float _alpha);
		inline float getAlpha() {return m_alpha;};

		void update(); // ���������� ���� � �����

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);


		void setState(const Ogre::String & _state);
		inline const static Ogre::String & getType() {static Ogre::String type("Widget"); return type;};

		void detach(WidgetPtr _child);
		inline const Ogre::String & getName() {return m_name;};

		void attach(BasisWidgetPtr _basis, bool _child);
		OverlayElementPtr getOverlayElement();

	protected:

		// ������� � ��������� ��� ���� �������
		BasisWidgetPtr addSubSkin(const tagBasisWidgetInfo &_info, const String & _material);
		// �������� ����������
		virtual ~Widget();

	protected:

		// ������ ���� �������
		const StateInfo & m_stateInfo;
		// ���������� �������� ��� ��������
		void visible(bool _visible);

		// ������ ���� ����� ��������
		WidgetChild m_widgetChild;
		// ������ ���� ����� ���������
		BasisChild m_subSkinChild;

		// ��������� �� ���� ������
		BasisWidgetPtr m_text;

		// ������ �� ��� �������� ��� ������ �� �������
		bool m_visible;
		// ������������ ������ �������
		float m_alpha;
		// ���� ������
		Ogre::ColourValue m_color;
		// ��� �������
		Ogre::String m_name;
	}; // class Widget

} // namespace widget