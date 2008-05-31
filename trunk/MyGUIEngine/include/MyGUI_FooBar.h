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
		IntSize tileSize;

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

		FooBarItemInfo(const Ogre::String &_name, const Ogre::String &_texture, int _enabled, const IntSize &_size){
			name = _name;
			enabled = _enabled;
			disabled = _enabled;
			focused = _enabled;
			widget = 0;
			texture = _texture;
			tileSize = _size;
		}

		FooBarItemInfo(const Ogre::String &_name, const Ogre::String &_texture, int _enabled, int _disabled, int _focused, const IntSize &_size)
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
		} FooBarLayout;

	protected:
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//!	@copydoc Widget::getWidgetType
		virtual const Ogre::String & getWidgetType() { return _getType(); }


		//!	Set bar position
		virtual void setPosition(const IntPoint& _pos);
		//!	Set bar position
		virtual void setPosition(const IntCoord& _coord);

		//!	Set width of bar
		void setWidth(int width);
		//!	Get width of bar
		int getWidth() const;

		//!	Set bar size
		virtual void setSize(const IntSize& _size);

		//!	@copydoc FrameListener::_frameEntered(float)
		virtual void _frameEntered(float _time);

		//!	@copydoc Widget::_onMouseDrag(int, int)
		virtual void _onMouseDrag(int _left, int _top);
		//!	@copydoc Widget::_onMouseButtonReleased(bool)
		virtual void _onMouseButtonReleased(int _left, int _top, MouseButton _id);
		//!	@copydoc Widget::_onMouseChangeRootFocus(bool)
		virtual void _onMouseChangeRootFocus(bool _focus);

		//!	Show bar with alpha smoothing
		void showSmooth(bool _reset = false);
		//!	Hide bar with alpha smoothing
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

		/*!	Set layout type

			\n	There is available types of layout:
			\li	FBL_COORDS - use window coordinates (not works)
			\li FBL_SNAP_LEFT - snap bar to left border of screen
			\li FBL_SNAP_RIGHT - snap bar to the right border of screen
			\li FBL_SNAP_TOP - snap bar to the top border of screen
			\li	FBL_SNAP_BOTTOM - snap bar to the bottom border of screen
		*/
		void setLayout(FooBarLayout layout);
		//!	Get layout type
		FooBarLayout getLayout() const;

		//! Set snap distance
		void setSnapDistance(const Ogre::Real &sd);
		//!	Get snap distance
		const Ogre::Real& getSnapDistance() const;

		/*!	Add item to bar

			@remarks	Add item to the end of items list

			@see FooBarItemInfo
		*/
		WidgetPtr addItem(FooBarItemInfo &item);
		/*!	Add item to bar

			@param	_name		Name of item (must be unique for bar)
			@param	_texture	Name of file that contains texture for item
			@param	_enabled	Number of tile in texture file that will use for enabled state
			@param	_size		Size of tile

			@returns	Pointer to widget that will used as item.

			@remarks	Add item to the end of items list. Tiles in file start numbering from left to right and from top to bottom.
			\n \bExample:\n
			<pre>
				+---+---+---+
				| 1 | 2 | 3 |
				+---+---+---+
				| 4 | 5 | 6 |
				+---+---+---+
			</pre>

		*/
		WidgetPtr addItem(const Ogre::String &_name, const Ogre::String &_texture, int _enabled, const IntSize &_size);

		void removeItem(const Ogre::String &name);

	protected:
		//!	Check if point in widget bounds
		bool checkPoint(int left, int top, WidgetPtr widget);
		//! Count position of bar
		void updatePosition(const IntPoint& _pos);
		//!	Recalculate size of bar
		void updateSize(const IntSize& _size);
		//!	Relayout items
		void updateItemsLayout();
		//!	Recalculate items size
		void updateItemsSize();
		//!	Recalculate items positions
		void updateItemsPosition();
		//!	Update alpha value depending on focus flag
		void updateAlpha();


		//!	Add child item to map
		void _addChildItem(const Ogre::String &name, const FooBarItemInfo &item);
		//!	Remove child item from map
		void _removeChildItem(const Ogre::String &name);
		//!	Check if item exists in map
		bool _isChildItem(const Ogre::String &name);
		//! Remove all child items from map
		void _removeAllChildItems();

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
		//! Width of bar
		int mWidth;
		//!	Distance to border to snap window
		Ogre::Real mSnapDistance;
		//!	Layout type
		FooBarLayout mLayout;

			

		//!	Map of child widgets
		typedef std::map<Ogre::String, FooBarItemInfo> ItemsMap;
		//!	Map of items
		ItemsMap mItems;
		//!	Vector of widgets pointer
		typedef std::vector<WidgetPtr> WidgetVector;
		/*!	Ordered items
			\n They will be layout with this order
		*/
		WidgetVector mItemsOrder;


	
	};
}


#endif
