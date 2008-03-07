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
#include "MyGUI_LayerItemInfo.h"
#include "MyGUI_WidgetUserData.h"
#include "MyGUI_WidgetEvent.h"

namespace MyGUI
{
	class _MyGUIExport Widget : public CroppedRectangleInterface , public LayerItemInfo, public UserData, public WidgetEvent
	{
		// ��� ������ �������� ������������
		friend class WidgetManager;
		friend class Gui;
		// ��� ������ ��������� ������������
		friend class factory::WidgetFactory;

	protected:
		// ��� �������� ������ ����� �������
		Widget(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);
		virtual ~Widget();

		void _updateView(); // ���������� ���� � �����

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		// �������������� � ����
		void _attachChild(CroppedRectanglePtr _basis, bool _child);

		// ������� ������ �������� ������� ��������
		void _destroyChildWidget(WidgetPtr _widget);
		// ������� ���� �����
		void _destroyAllChildWidget();

	public:
		// ������ � ������� ��� �������� �������
		virtual WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name = "")
		{
			return createWidgetT(_type, _skin, IntCoord(_left, _top, _width, _height), _align, _name);
		}

		WidgetPtr createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _name = "")
		{
			return createWidgetRealT(_type, _skin, FloatCoord(_left, _top, _width, _height), _align, _name);
		}

		// ������� ��� �������� ������� �� ����
		template <class T> inline T* createWidget(const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetT(T::_getType(), _skin, _coord, _align, _name));
		}
		template <class T> inline T* createWidget(const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetT(T::_getType(), _skin, IntCoord(_left, _top, _width, _height), _align, _name));
		}
		template <class T> inline T* createWidgetReal(const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::_getType(), _skin, _coord, _align, _name));
		}
		template <class T> inline T* createWidgetReal(const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::_getType(), _skin, _left, _top, _width, _height, _align, _name));
		}


		inline const Ogre::String & getName() {return mName;}
		inline static const Ogre::String & _getType() {static Ogre::String type("Widget"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		virtual void setPosition(const IntPoint& _pos);
		virtual void setPosition(const IntCoord& _coord);
		virtual void setSize(const IntSize& _size);

		inline void setPosition(int _left, int _top) {setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		virtual void show();
		virtual void hide();

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
		IntCoord getTextCoord();
		IntSize getTextSize();
		IntSize getTextSize(const Ogre::DisplayString& _text);

		void setAlpha(float _alpha);
		inline float getAlpha() {return mAlpha;};
		inline float _getRealAlpha() {return mRealAlpha;}
		void _updateAlpha();

		void setState(const Ogre::String & _state);

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);


		// ��������� ����� �������� ������
		inline WidgetPtr getParent() {return static_cast<WidgetPtr>(mParent);}
		// ��� ��������� ���� �������� �� ���������� ������
		inline WidgetPtr _getOwner() {return mOwner;}
		inline void _setOwner(WidgetPtr _widget) { if (null == mParent) mOwner = _widget; }

		// return copy
		virtual VectorWidgetPtr getChilds();

		// ���������� ��������� �� ����� � ���� ����� ��������� � ������ (������������ �� LayerItemInfo)
		virtual LayerItemInfoPtr findItem(int _left, int _top);

		inline bool isNeedKeyFocus() {return mNeedKeyFocus;}
		inline void setNeedKeyFocus(bool _need) {mNeedKeyFocus = _need;}
		inline bool isEnabled() {return mEnabled;}
		void setEnabled(bool _enabled, bool _silent = false);

		// ���������� ����������� ��������� ��� ����� ������
		size_t _getCountSharedOverlay();
		Ogre::OverlayElement* _getSharedOverlayElement();

		// ���������� ������ ���������� ���� ���� ����� ����
		virtual const IntCoord& getClientRect();

		// ������ ��� ����� �������
		inline const std::string& getPointer()
		{
			return mPointer;
		}
		inline void setPointer(const std::string& _pointer)
		{
			mPointer = _pointer;
		}

		// ���� ��������� ������� ��� ��������
		inline void _forcePeek(WidgetPtr _widget)
		{
			size_t size = mWidgetChild.size();
			if ( (size < 2) || (mWidgetChild[0] == _widget) ) return;
			for (size_t pos=1; pos<size; pos++) {
				if (mWidgetChild[pos] == _widget) {
					mWidgetChild[pos] = mWidgetChild[0];
					mWidgetChild[0] = _widget;
					return;
				}
			}
		}

	protected:

		// ������� � ��������� ��� ���� �������
		CroppedRectanglePtr addSubSkin(const SubWidgetInfo& _info, const Ogre::String& _material, size_t & _id);
		// ���������� �������� ��� ��������
		void _setVisible(bool _visible);

	protected:
		// ������ ���� �������
		const MapWidgetStateInfo & mStateInfo;
		// ���������� � ����� ��� �������
		const MaskPeekInfo & mMaskPeekInfo;

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
		float mRealAlpha;
		// ���� ������
		Ogre::ColourValue mColour;
		// ��� �������
		Ogre::String mName;
		// ����������� �������� ������� ���� ����������
		size_t mCountSharedOverlay;
		// ������ ������� ����� ������� ��� ���������
		std::string mPointer;

		// ��� ��������� ����, �������� �� ����������� ������
		// ����������� ����, ������ ����������� � �.�.
		WidgetPtr mOwner;

	}; // class Widget

} // namespace MyGUI

#endif // __MYGUI_WIDGET_H__
