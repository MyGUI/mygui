/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_H__
#define __MYGUI_WIDGET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_CroppedRectangleInterface.h"
#include "MyGUI_SubWidgetTextInterface.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_LayerItemInfo.h"
#include "MyGUI_WidgetUserData.h"
#include "MyGUI_WidgetEvent.h"
#include "MyGUI_WidgetFactory.h"

namespace MyGUI
{

	class Gui;

	class _MyGUIExport Widget : public CroppedRectangleInterface , public LayerItemInfo, public UserData, public WidgetEvent
	{
		// ��� ������ �������� ������������
		friend WidgetManager;
		friend Gui;
		// ��� ������ ��������� ������������
		friend factory::WidgetFactory;

	protected:
		// ��� �������� ������ ����� �������
		Widget(int _left, int _top, int _width, int _height, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);
		virtual ~Widget();

		void _updateView(); // ���������� ���� � �����

		void _setAlign(int _width, int _height, bool _update);
		void _setAlign(int _left, int _top, int _width, int _height, bool _update);

		// �������������� � ����
		void _attachChild(CroppedRectanglePtr _basis, bool _child);

		// ������� ������, �������� ������ �� ��������
		virtual void _frameStarted(float _frame, float _event) {}
		virtual void _frameEnded(float _frame, float _event) {}

		// ������� ������ �������� ������� ��������
		void _destroyChildWidget(WidgetPtr & _widget);
		// ������� ���� �����
		void _destroyAllChildWidget();

		// ����������� � ���������������� ��� ��������� ������������� ��������������
		bool isWidget() {return true;}

		// ��������������� ����� ��� ������������� ������������
		WidgetPtr parseSubWidget(const MapString & _param, const std::string & _type, const std::string & _skin, const std::string & _offset, const std::string & _align, const IntSize &_size);

	public:
		// ������ � ������� ��� �������� �������
		virtual WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name = "");
		inline WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, IntRect _rect, Align _align, const Ogre::String & _name = "")
		{
			return createWidget(_type, _skin, _rect.left, _rect.top, _rect.getWidth(), _rect.getHeight(), _align, _name);
		}

		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, FloatRect _rect, Align _align, const Ogre::String & _name = "")
		{
			return createWidgetReal(_type, _skin, _rect.left, _rect.top, _rect.getWidth(), _rect.getHeight(), _align, _name);
		}

		// ������� ��� �������� ������� �� ����
		template <class T>
		inline T* createWidgetT(const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name = "")
		{
			Widget * widget = createWidget(T::getType(), _skin, _left, _top, _width, _height, _align, _name);
			MYGUI_TYPE(T*, widget);
			return static_cast<T*>(widget);
		}
		template <class T>
		inline T* createWidgetT(const Ogre::String & _skin, IntRect _rect, Align _align, const Ogre::String & _name = "")
		{
			return createWidgetT<T>(_skin, _rect.left, _rect.top, _rect.getWidth(), _rect.getHeight(), _align, _name);
		}
		template <class T>
		inline T* createWidgetRealT(const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _name = "")
		{
			Widget * widget = createWidgetReal(T::getType(), _skin, _left, _top, _width, _height, _align, _name);
			MYGUI_TYPE(T*, widget);
			return static_cast<T*>(widget);
		}
		template <class T>
		inline T* createWidgetRealT(const Ogre::String & _skin, FloatRect _rect, Align _align, const Ogre::String & _name = "")
		{
			return createWidgetRealT<T>(_skin, _rect.left, _rect.top, _rect.getWidth(), _rect.getHeight(), _align, _name);
		}


		inline const Ogre::String & getName() {return mName;}
		inline static const Ogre::String & getType() {static Ogre::String type("Widget"); return type;}

		virtual void setPosition(int _left, int _top);
		virtual void setPosition(int _left, int _top, int _width, int _height);
		virtual void setSize(int _width, int _height);

		void show();
		void hide();

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

		// ���������� ��������� �� ����� � ���� ����� ��������� � ������ (������������ �� LayerItemInfo)
		LayerItemInfoPtr findItem(int _left, int _top);

		inline bool isNeedKeyFocus() {return mNeedKeyFocus;}
		inline void setNeedKeyFocus(bool _need) {mNeedKeyFocus = _need;}
		inline bool isEnabled() {return mEnabled;}
		void setEnabled(bool _enabled);

		// ���������� ����������� ��������� ��� ����� ������
		size_t _getCountSharedOverlay();
		Ogre::OverlayElement* _getSharedOverlayElement();

		// ���������� ������ ���������� ���� ���� ����� ����
		virtual IntRect getClientRect() {return IntRect(mLeft, mTop, mLeft + mWidth, mTop + mHeight);}

	protected:

		// ������� � ��������� ��� ���� �������
		CroppedRectanglePtr addSubSkin(const SubWidgetInfo& _info, const Ogre::String& _material, size_t & _id);
		// ���������� �������� ��� ��������
		void _setVisible(bool _visible);

	protected:
		// ������ ���� �������
		const MapWidgetStateInfo & mStateInfo;

		// ������ ���� ����� ��������
		VectorWidgetPtr mWidgetChild;
		// ������ ���� ����� ���������
		VectorCroppedRectanglePtr mSubSkinChild;

		// ��������� �� ���� ������
		SubWidgetTextPtr mText;

		// �������� �� �� ������
		bool mEnabled;
		// ������ �� ��� �������� ��� ������ �� �������
		bool mVisible;
		// ������������ ������ �������
		float mAlpha;
		// ���� ������
		Ogre::ColourValue mColour;
		// ��� �������
		Ogre::String mName;
		// ����������� �������� ������� ���� ����������
		size_t mCountSharedOverlay;

	}; // class Widget

} // namespace MyGUI

#endif // __MYGUI_WIDGET_H__