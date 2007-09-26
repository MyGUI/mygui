#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "Prerequest.h"
#include <vector>
#include "MainSkin.h"

#include "WidgetSkinInfo.h"
#include "LayerItemInfo.h"
#include "WidgetUserData.h"
#include "WidgetEvent.h"
#include "WidgetFactory.h"

namespace MyGUI
{

	

	class _MyGUIExport Widget : public BasisWidget , public LayerItemInfo, public UserData, public WidgetEvent
	{
		// ��� ������ �������� ������������
		friend WidgetManager;
		// ��� ������ ��������� ������������
		friend WidgetFactory<Widget>;


	protected:
		// ��� �������� ������ ����� �������
		Widget(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		// ������� �����
		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _name = "");
		WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _x, float _y, float _cx, float _cy, char _align, const Ogre::String & _name = "");

		void move(int _x, int _y);
		virtual void move(int _x, int _y, int _cx, int _cy);
		virtual void size(int _cx, int _cy);

		void show(bool _show);

		virtual void setCaption(const Ogre::DisplayString & _caption);
		virtual const Ogre::DisplayString & getCaption();

		void setColour(const Ogre::ColourValue & _color);
		const Ogre::ColourValue & getColour() {return m_color;};

		void setFontName(const Ogre::String & _font);
		void setFontName(const Ogre::String & _font, Ogre::ushort _height);
		const Ogre::String & getFontName();

		void setFontHeight(Ogre::ushort _height);
		Ogre::ushort getFontHeight();

		void setTextAlign(char _align);

		void setAlpha(float _alpha);
		inline float getAlpha() {return m_alpha;};
	
		inline const static Ogre::String & getType() {static Ogre::String type("Widget"); return type;};

		void setState(const Ogre::String & _state);

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);


		// ��������� ����� �������� ������
		inline WidgetPtr getParent() {return static_cast<WidgetPtr>(m_parent);}

	protected:
		void update(); // ���������� ���� � �����

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		inline const Ogre::String & getName() {return m_name;};

		// �������������� � ����
		void attach(BasisWidgetPtr _basis, bool _child);

		// ������� ������ �������� ������� ��������
		void destroyWidget(WidgetPtr & _widget);
		// ������� ���� �����
		void destroyWidget();

		// ����������� � ���������������� ��� ��������� ������������� ��������������
		bool isWidget() {return true;}

	public:
		// ���������� ��������� �� ����� � ���� ����� ��������� � ������ (������������ �� LayerItemInfo)
		LayerItemInfoPtr findItem(int _x, int _y);

		inline bool isNeedKeyFocus() {return m_needKeyFocus;}
		inline void setNeedKeyFocus(bool _need) {m_needKeyFocus = _need;}
		inline bool isEnable() {return m_enable;}

	protected:

		// ������� � ��������� ��� ���� �������
		BasisWidgetPtr addSubSkin(const tagBasisWidgetInfo & _info, const String & _material);

	public:
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

		// �������� �� �� ������
		bool m_enable;
		// ������ �� ��� �������� ��� ������ �� �������
		bool m_visible;
		// ������������ ������ �������
		float m_alpha;
		// ���� ������
		Ogre::ColourValue m_color;
		// ��� �������
		Ogre::String m_name;

	}; // class Widget

} // namespace MyGUI


#endif
