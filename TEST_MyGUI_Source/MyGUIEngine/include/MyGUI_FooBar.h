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
		void _onMouseDrag(int _left, int _top);
		void _onMouseButtonReleased(bool _left);

		// show/hide with alpha smoothing
		void showSmooth(bool _reset = false);
		void hideSmooth();

	protected:
		bool checkPoint(int left, int top, WidgetPtr widget);

		

	protected:
		// id of widget under mouse
		int mMouseWidget;
		// dragging flag
		bool mDragging;

		
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
		void addItem(const Ogre::String &name, const Ogre::String &texture);
		void removeItem(const Ogre::String &name);

	protected:
		//	add child item
		void _addChildItem(const Ogre::String &name, WidgetPtr widget);
		//	remove child item
		void _removeChildItem(const Ogre::String &name);
		//	check if item exists
		bool _isChildItem(const Ogre::String &name);
		// remove all child items
		void _removeAllChildItems();

	protected:
		// map of child widgets
		typedef std::map<Ogre::String, WidgetPtr> WidgetMap;
		WidgetMap mItems;

		typedef std::vector<WidgetPtr> WidgetVector;
		WidgetVector mItemsOrder;


	
	};
}


#endif