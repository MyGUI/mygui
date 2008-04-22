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
#include "MyGUI_LayerItem.h"
#include "MyGUI_WidgetUserData.h"
#include "MyGUI_WidgetEvent.h"
#include "MyGUI_WidgetCreator.h"

namespace MyGUI
{

	class _MyGUIExport Widget : public CroppedRectangleInterface , public LayerItem, public UserData, public WidgetEvent, public WidgetCreator
	{
		// ��� ������ �������� ������������
		friend class WidgetCreator;
		// ��� ������ ��������� ������������
		friend class factory::WidgetFactory;

	protected:
		// ��� �������� ������ ����� �������
		Widget(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~Widget();

		void _updateView(); // ���������� ���� � �����

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		// ���������� �������� ��� ��������
		void _setVisible(bool _visible);

		// ������� ������
		virtual WidgetPtr _createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		// ������� ����������
		virtual void _destroyChildWidget(WidgetPtr _widget);

		// ������� ���� �����
		virtual void _destroyAllChildWidget();

		static Ogre::String WidgetTypeName;

	public:

		// ������ � ������� ��� �������� �������
		inline WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name = "")
		{
			return _createWidget(_type, _skin, _coord, _align, "", _name);
		}
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

		//! Get name if widget
		inline const Ogre::String & getName() {return mName;}
		//! Get widget class type
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! Get widget type
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		virtual void setPosition(const IntPoint& _pos);
		virtual void setPosition(const IntCoord& _coord);
		virtual void setSize(const IntSize& _size);

		inline void setPosition(int _left, int _top) {setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		void _updateAbsolutePoint();

		virtual void show();
		virtual void hide();

		// ��� ����������� �������������
		void _inheritedShow();
		void _inheritedHide();
		inline bool _isInheritedShow() {return mInheritedShow;}

		virtual void setCaption(const Ogre::UTFString & _caption);
		virtual const Ogre::UTFString & getCaption();

		virtual void setColour(const Ogre::ColourValue & _colour);
		virtual const Ogre::ColourValue & getColour();

		virtual void setFontName(const Ogre::String & _font);
		virtual const Ogre::String & getFontName();

		virtual void setFontHeight(uint16 _height);
		virtual Ogre::ushort getFontHeight();

		virtual void setTextAlign(Align _align);
		virtual Align getTextAlign();

		IntCoord getTextCoord();
		IntSize getTextSize();

		void setAlpha(float _alpha);
		inline float getAlpha() {return mAlpha;};
		inline float _getRealAlpha() {return mRealAlpha;}
		inline bool isInheritsAlpha() {return mInheritsAlpha;}
		inline void setInheritsAlpha(bool _inherits)
		{
			mInheritsAlpha = _inherits;
			_updateAlpha();
		}
		void _updateAlpha();

		void setState(const Ogre::String & _state);
		void _setUVSet(const FloatRect& _rect);

		virtual void _setTextureName(const Ogre::String& _texture);
		virtual const Ogre::String& _getTextureName();

		// �������� �� �� ������� ��������
		inline bool isRootWidget() {return null == mParent;}

		// ��������� ����� �������� ������
		inline WidgetPtr getParent() {return static_cast<WidgetPtr>(mParent);}

		// ��� ��������� ���� �������� �� ���������� ������
		inline WidgetPtr _getOwner() {return mOwner;}
		inline void _setOwner(WidgetPtr _widget) { if (isRootWidget()) mOwner = _widget; }

		// return copy
		virtual VectorWidgetPtr getChilds();

		// ����������� �� LayerInfo
		virtual LayerItem * _findLayerItem(int _left, int _top);
		virtual void _attachToLayerItemKeeper(LayerItemKeeper * _item);
		virtual void _detachFromLayerItemKeeper();

		inline bool isNeedKeyFocus() {return mNeedKeyFocus;}
		inline void setNeedKeyFocus(bool _need) {mNeedKeyFocus = _need;}
		inline bool isNeedMouseFocus() {return mNeedMouseFocus;}
		inline void setNeedMouseFocus(bool _need) {mNeedMouseFocus = _need;}

		inline bool isEnabled() {return mEnabled;}
		void setEnabled(bool _enabled, bool _silent = false);

		// ���������� ������ ���������� ���� ���� ����� ����
		virtual const IntCoord& getClientRect();

		// ������ ��� ����� �������
		inline const std::string& getPointer()
		{
			if (false == mEnabled) {
				static std::string empty;
				return empty;
			}
			return mPointer;
		}

		inline void setPointer(const std::string& _pointer)
		{
			mPointer = _pointer;
		}

		// ���� ��������� ������� ��� ��������
		void _forcePeek(WidgetPtr _widget);

		// ���������� ��� �����, � �������� ���������� ������
		// ��������� ������ ��� �������� �������
		const std::string& getLayerName();

		inline WidgetCreator * _getWidgetCreator()
		{
			return mWidgetCreator;
		}

		// ����� ��� ������� ������ ������ � �������� ���� ������
		virtual void _getDragItemInfo(WidgetPtr & _list, size_t & _index);
		// ����� ��� ������������ ������ ������
		virtual void _setDragItemInfo(size_t _index, bool _set, bool _accept);

		inline void setNeedDragDrop(bool _need) {mNeedDragDrop = _need;}
		inline bool getNeedDragDrop() {return mNeedDragDrop;}

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
		SubWidgetTextInterfacePtr mText;
		// ��������� �� ������ �� ��������� �������
		CroppedRectangleInterface * mMainSkin;

		// �������� �� �� ������
		bool mEnabled;
		// ������ �� ��� �������� ��� ������ �� �������
		bool mVisible;
		// ��� �������������� �������
		bool mInheritedShow;
		// ������������ � ���� ������������ ����� ������ �������
		float mAlpha;
		float mRealAlpha;
		bool mInheritsAlpha;
		// ��� �������
		Ogre::String mName;
		// ������ ������� ����� ������� ��� ���������
		std::string mPointer;
		std::string mTexture;

		// ��� ��������� ����, �������� �� ����������� ������
		// ����������� ����, ������ ����������� � �.�.
		WidgetPtr mOwner;

		// ��� ��� ��� ��� ������, � ��� ��� ����� �������
		WidgetCreator * mWidgetCreator;

		// ����� �� ������� ���� � �����
		bool mNeedKeyFocus;
		// ����� �� ������� ����� ����
		bool mNeedMouseFocus;

		// ����� � ������� ��������� ���� ��� �����
		bool mNeedDragDrop;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_H__
