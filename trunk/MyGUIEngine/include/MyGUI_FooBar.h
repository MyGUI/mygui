/*!
	@file
	@author		Denis Koronchik
	@date		3/2008
	@module
*/
#ifndef __MYGUI_FOOBAR_H__
#define __MYGUI_FOOBAR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_FrameListener.h"

namespace MyGUI
{
	// class to store items information
	class _MyGUIExport FooBarItemInfo
	{
		friend class FooBar;

	public:
		// texture name
		Ogre::String texture;
		// number of image that will use when item enabled
		int enabled;
		// number of image that will use when item disabled
		int disabled;
		// number of image that will use when item focused (under mouse)
		int focused;
		// name of item
		Ogre::String name;
		// image size
		FloatSize tileSize;

		FooBarItemInfo()
		{
			name = "";
			enabled = -1;
			disabled = -1;
			focused = -1;
			widget = 0;
			texture = "";
			tileSize = FloatSize(0.f, 0.f);
		}

		FooBarItemInfo(const Ogre::String &_name, const Ogre::String &_texture, int _enabled, const FloatSize &_size){
			name = _name;
			enabled = _enabled;
			disabled = _enabled;
			focused = _enabled;
			widget = 0;
			texture = _texture;
			tileSize = _size;
		}

		FooBarItemInfo(const Ogre::String &_name, const Ogre::String &_texture, int _enabled, int _disabled, int _focused, const FloatSize &_size)
		{
			name = _name;
			enabled = _enabled;
			disabled = _disabled;
			focused = _focused;
			widget = 0;
			texture = _texture;
			tileSize = _size;
		}

	protected:
		// pointer to widget
		WidgetPtr widget;
	};



	typedef delegates::CDelegate2<WidgetPtr, const std::string&> EventInfo_WidgetString;

	class _MyGUIExport FooBar : public Widget, public FrameListener
	{
		friend class factory::FooBarFactory;

	protected:
		FooBar(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~FooBar();

	public:
		typedef enum{
			FBL_COORDS = 0,
			FBL_SNAP_LEFT,
			FBL_SNAP_RIGHT,
			FBL_SNAP_BOTTOM,
			FBL_SNAP_TOP
		} Layout;

	public:
		// get type of widget
		inline static const Ogre::String & _getType() {static Ogre::String type("FooBar"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }


		// set/get position
		virtual void setPosition(const IntPoint& _pos);
		virtual void setPosition(const IntCoord& _coord);

		// set/get size
		virtual void setSize(const IntSize& _size);

		// frame event
		virtual void _frameEntered(float _time);

		// mouse events
		virtual void _onMouseDrag(int _left, int _top);
		virtual void _onMouseButtonReleased(bool _left);
		virtual void _onMouseChangeRootFocus(bool _focus);

		// show/hide with alpha smoothing
		void showSmooth(bool _reset = false);
		void hideSmooth();

		//! Set auto alpha
		void setAutoAlpha(bool _alpha);
		//!	Get auto alpha flag
		bool getAutoAlpha() const;

		//!	Set inactive alpha value
		void setAlphaInactive(const Ogre::Real &_alpha);
		//!	Get inactive alpha value
		Ogre::Real getAlphaInactive() const;
		//!	Set focused alpha value
		void setAlphaFocused(const Ogre::Real &_alpha);
		//!	Get focused alpha value
		Ogre::Real getAlphaFocused() const;
		//!	Set speed to alpha change
		void setAlphaSpeed(const Ogre::Real &_alpha);
		//!	Get speed to alpha change
		Ogre::Real getAlphaSpeed() const;

	protected:
		bool checkPoint(int left, int top, WidgetPtr widget);

		

	protected:
		//! Id of widget under mouse
		int mMouseWidget;
		//! Dragging flag
		bool mDragging;
		//! Focused flag
		bool mFocus;
		//! Auto alpha flag
		bool mAutoAlpha;
		//! Inactive alpha value
		Ogre::Real mAlphaIncative;
		//!	Focused alpha value
		Ogre::Real mAlphaFocused;
		//!	Speed to alpha change
		Ogre::Real mAlphaSpeed;


		
	public:
		// set/get width of bar
		void setWidth(int width);
		int getWidth() const;

	protected:
		// width of bar
		int mWidth;

	protected:
		// count position and size
		void updatePosition(const IntPoint& _pos);
		void updateSize(const IntSize& _size);
		void updateItemsLayout();
		void updateItemsSize();
		void updateItemsPosition();

		void updateAlpha();

	public:
		// set/get snap distance
		void setSnapDistance(const Ogre::Real &sd);
		const Ogre::Real& getSnapDistance() const;

	protected:
		Ogre::Real mSnapDistance;

	public:
		// set/get layout
		void setLayout(Layout layout);
		Layout getLayout() const;

	protected:
		Layout mLayout;

	public:
		WidgetPtr addItem(FooBarItemInfo &item);
		void removeItem(const Ogre::String &name);

	protected:
		//	add child item
		void _addChildItem(const Ogre::String &name, const FooBarItemInfo &item);
		//	remove child item
		void _removeChildItem(const Ogre::String &name);
		//	check if item exists
		bool _isChildItem(const Ogre::String &name);
		// remove all child items
		void _removeAllChildItems();

	protected:
		// map of child widgets
		typedef std::map<Ogre::String, FooBarItemInfo> ItemsMap;
		ItemsMap mItems;

		typedef std::vector<WidgetPtr> WidgetVector;
		WidgetVector mItemsOrder;


	
	};
}


#endif
