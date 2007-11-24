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
		Widget(int _left, int _top, int _width, int _height, Align _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		// ������� �����
		virtual WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _name = "");

		virtual void move(int _left, int _top);
		virtual void move(int _left, int _top, int _width, int _height);
		virtual void size(int _width, int _height);

		void show(bool _show);

		virtual void setCaption(const Ogre::DisplayString & _caption);
		virtual const Ogre::DisplayString & getCaption();

		void setColour(const Ogre::ColourValue & _colour);
		const Ogre::ColourValue & getColour() {return mColour;};

		void setFontName(const Ogre::String & _font);
		void setFontName(const Ogre::String & _font, Ogre::ushort _height);
		const Ogre::String & getFontName();

		void setFontHeight(Ogre::ushort _height);
		Ogre::ushort getFontHeight();

		void setTextAlign(Align _align);

		void setAlpha(float _alpha);
		inline float getAlpha() {return mAlpha;};
	
		void setState(const Ogre::String & _state);

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);


		// ��������� ����� �������� ������
		inline WidgetPtr getParent() {return static_cast<WidgetPtr>(mParent);}

	protected:
		void update(); // ���������� ���� � �����

		void align(int _width, int _height, bool _update);
		void align(int _left, int _top, int _width, int _height, bool _update);

		inline const Ogre::String & getName() {return mName;};

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
		LayerItemInfoPtr findItem(int _left, int _top);

		inline bool isNeedKeyFocus() {return mNeedKeyFocus;}
		inline void setNeedKeyFocus(bool _need) {mNeedKeyFocus = _need;}
		inline bool isEnable() {return mEnable;}

	protected:

		// ������� � ��������� ��� ���� �������
		BasisWidgetPtr addSubSkin(const BasisWidgetInfo& _info, const Ogre::String& _material);

	public:
		// �������� ����������
		virtual ~Widget();

	protected:
		// ���������� �������� ��� ��������
		void visible(bool _visible);

	protected:
		// ������ ���� �������
		const MapWidgetStateInfo & mStateInfo;

		// ������ ���� ����� ��������
		VectorWidgetPtr mWidgetChild;
		// ������ ���� ����� ���������
		VectorBasisWidgetPtr mSubSkinChild;

		// ��������� �� ���� ������
		BasisWidgetPtr mText;

		// �������� �� �� ������
		bool mEnable;
		// ������ �� ��� �������� ��� ������ �� �������
		bool mVisible;
		// ������������ ������ �������
		float mAlpha;
		// ���� ������
		Ogre::ColourValue mColour;
		// ��� �������
		Ogre::String mName;

	}; // class Widget

} // namespace MyGUI

#endif // __WIDGET_H__