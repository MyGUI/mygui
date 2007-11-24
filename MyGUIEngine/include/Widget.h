#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "Prerequest.h"
#include "BasisWidget.h"
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
		friend factory::WidgetFactory;

	protected:
		// ��� �������� ������ ����� �������
		Widget(int _x, int _y, int _cx, int _cy, Align _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		// ������� �����
		virtual WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, Align _align, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _x, float _y, float _cx, float _cy, Align _align, const Ogre::String & _name = "");

		virtual void move(int _x, int _y);
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

		void setTextAlign(Align _align);

		void setAlpha(float _alpha);
		inline float getAlpha() {return m_alpha;};
	
		void setState(const Ogre::String & _state);

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);


		// ��������� ����� �������� ������
		inline WidgetPtr getParent() {return static_cast<WidgetPtr>(mParent);}

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

		// ��������������� ����� ��� ������������� ������������
		WidgetPtr parseSubWidget(const MapString & _param, const std::string & _type, const std::string & _skin, const std::string & _offset, const std::string & _align, const IntSize &_size);

	public:
		// ���������� ��������� �� ����� � ���� ����� ��������� � ������ (������������ �� LayerItemInfo)
		LayerItemInfoPtr findItem(int _x, int _y);

		inline bool isNeedKeyFocus() {return m_needKeyFocus;}
		inline void setNeedKeyFocus(bool _need) {m_needKeyFocus = _need;}
		inline bool isEnable() {return m_enable;}

	protected:

		// ������� � ��������� ��� ���� �������
		BasisWidgetPtr addSubSkin(const BasisWidgetInfo& _info, const Ogre::String& _material);

	public:
		// �������� ����������
		virtual ~Widget();

	protected:

		// ������ ���� �������
		const MapWidgetStateInfo & m_stateInfo;
		// ���������� �������� ��� ��������
		void visible(bool _visible);

		// ������ ���� ����� ��������
		VectorWidgetPtr mWidgetChild;
		// ������ ���� ����� ���������
		VectorBasisWidgetPtr m_subSkinChild;

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


#endif // __WIDGET_H__