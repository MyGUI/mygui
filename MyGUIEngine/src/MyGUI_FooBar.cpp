/*!
	@file
	@author		Denis Koronchik
	@date		3/2008
	@module
*/
#include "MyGUI_FooBar.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Gui.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_ControllerFadeAlpha.h"

const float WINDOW_ALPHA_MAX = ALPHA_MAX;
const float WINDOW_ALPHA_MIN = ALPHA_MIN;
const float WINDOW_ALPHA_ACTIVE = ALPHA_MAX;
const float WINDOW_ALPHA_FOCUS = 0.7f;
const float WINDOW_ALPHA_DEACTIVE = 0.3f;
const float WINDOW_SPEED_COEF = 3.0f;

namespace MyGUI
{

	FooBar::FooBar(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) : 
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mSnapDistance(0),
		mLayout(FBL_COORDS),
		mWidth(70),
		mMouseWidget(-1),
		mDragging(false)
	{
		Gui::getInstance().addFrameListener(this);
	}

	FooBar::~FooBar()
	{
		Gui::getInstance().removeFrameListener(this);

		_removeAllChildItems();
	}

	void FooBar::setPosition(const IntPoint& _pos)
	{
		updatePosition(_pos);
	}

	void FooBar::setPosition(const IntCoord& _coord)
	{
		//updatePosition(pt);
	}

	void FooBar::setSize(const IntSize& _size)
	{
		updateSize(_size);
	}

	void FooBar::showSmooth(bool _reset)
	{
		if (_reset) {
			setAlpha(ALPHA_MIN);
			show();
		}

		ControllerManager::getInstance().addItem(
			this, new ControllerFadeAlpha(WINDOW_ALPHA_DEACTIVE,
			WINDOW_SPEED_COEF, ControllerFadeAlpha::ACTION_NONE, true));
	}

	void FooBar::hideSmooth()
	{
		ControllerManager::getInstance().addItem(
			this, new ControllerFadeAlpha(WINDOW_ALPHA_MIN, WINDOW_SPEED_COEF, ControllerFadeAlpha::ACTION_HIDE, false));
	}

	void FooBar::_frameEntered(float _time)
	{
		IntPoint pt = InputManager::getInstance().getMousePosition();

		if ((mCoord.left > pt.left || (mCoord.left + mCoord.width) < pt.left) ||
			(mCoord.top > pt.top || (mCoord.top + mCoord.height) < pt.top))
		{
			if (mMouseWidget != -1)
			{
				mMouseWidget = -1;
				updateItemsLayout();
			}
			mMouseWidget = -1;
			
			return;
		}

				
		int n = (int)mItemsOrder.size();
		for (int i = 0; i < n; i++)
			if (checkPoint(pt.left, pt.top, mItemsOrder[i]))
				if (mMouseWidget != i)
				{
					mMouseWidget = i;
					updateItemsLayout();
					return;
				}


	}

	void FooBar::_onMouseDrag(int _left, int _top)
	{
		if (_checkPoint(_left, _top))
			mDragging = true;
		
		if (!mDragging)
			return;

		int width = (int)Gui::getInstance().getViewWidth();
		int height = (int)Gui::getInstance().getViewHeight();

		if (_left < mWidth)
			setLayout(FBL_SNAP_LEFT);
		else
			if ((width - mWidth) < _left)
				setLayout(FBL_SNAP_RIGHT);
			else
				if (_top < mWidth)
					setLayout(FBL_SNAP_TOP);
				else
					if ((height - mWidth) < _top)
						setLayout(FBL_SNAP_BOTTOM);
		
		Widget::_onMouseDrag(_left, _top);
	}

	void FooBar::_onMouseButtonReleased(bool _left)
	{
		mDragging = false;

		Widget::_onMouseButtonReleased(_left);
	}

	bool FooBar::checkPoint(int left, int top, WidgetPtr widget)
	{
		IntPoint pt = widget->getPosition();
		IntSize sz = widget->getSize();
		IntPoint p = getPosition();

		int l, t;

		l = left - p.left;
		t = top - p.top;

		if (l < pt.left || l > (pt.left + sz.width))
			return false;
		if (t < pt.top || t > (pt.top + sz.height))
			return false;

		return true;
	}

	void FooBar::setSnapDistance(const Ogre::Real &sd)
	{
		mSnapDistance = sd;
	}

	const Ogre::Real& FooBar::getSnapDistance() const
	{
		return mSnapDistance;
	}

	void FooBar::setLayout(FooBar::Layout layout)
	{
		if (mLayout != layout)
		{
			mLayout = layout;
			updatePosition(getPosition());
			updateSize(IntSize(mCoord.width, mCoord.height));
			updateItemsLayout();
		}else
			mLayout = layout;
	}

	FooBar::Layout FooBar::getLayout() const
	{
		return mLayout;
	}

	void FooBar::setWidth(int width)
	{
		mWidth = width;
		if (mLayout != FBL_COORDS)
			updateSize(IntSize(mCoord.width, mCoord.height));
	}

	int FooBar::getWidth() const
	{
		return mWidth;
	}

	void FooBar::updatePosition(const IntPoint& _pos)
	{
		int width = (int)Gui::getInstance().getViewWidth();
		int height = (int)Gui::getInstance().getViewHeight();

		IntPoint pos = _pos;
		
		//if (mSnap) {
		switch(mLayout)
		{
		case FBL_COORDS:
			if (abs(pos.left) <= mSnapDistance) pos.left = 0;
			if (abs(pos.top) <= mSnapDistance) pos.top = 0;
			if ( abs(pos.left + mCoord.width - width) < mSnapDistance) pos.left = width - mCoord.width;
			if ( abs(pos.top + mCoord.height - height) < mSnapDistance) pos.top = height - mCoord.height;
			break;

		case FBL_SNAP_BOTTOM:
			pos.top = height - mWidth;
			pos.left = 0;
			break;

		case FBL_SNAP_LEFT:
			pos.top = 0;
			pos.left = 0;
			break;

		case FBL_SNAP_RIGHT:
			pos.top = 0;
			pos.left = width - mWidth;
			break;

		case FBL_SNAP_TOP:
			pos.top = 0;
			pos.left = 0;
			break;

		}
			
		//}
		Widget::setPosition(pos);

	}

	void FooBar::updateSize(const IntSize& _size)
	{
		int width = (int)Gui::getInstance().getViewWidth();
		int height = (int)Gui::getInstance().getViewHeight();

		IntSize size = _size;

		switch(mLayout)
		{
		case FBL_COORDS:
			
			break;

		case FBL_SNAP_BOTTOM:
		case FBL_SNAP_TOP:
			size.height = mWidth;
			size.width = width;
			break;

		case FBL_SNAP_LEFT:
		case FBL_SNAP_RIGHT:
			size.height = height;
			size.width = mWidth;
			break;
		}

		Widget::setSize(size);
	}

	void FooBar::updateItemsLayout()
	{
		updateItemsSize();
		updateItemsPosition();
	}

	void FooBar::updateItemsSize()
	{
		int n = (int)mItemsOrder.size();
		int sz;

		for (int i = 0; i < n; i++)
		{
			WidgetPtr widget = mItemsOrder[i];

			if (mMouseWidget != -1)
			{
				if (abs(i - mMouseWidget) < 2)
					sz = mWidth / (1.2 + 0.25 * abs(i - mMouseWidget));
				else
					sz = mWidth / 1.7;
			}else
				sz = mWidth / 1.7;

			widget->setSize(sz, sz);
		}
	}

	void FooBar::updateItemsPosition()
	{
		int w, h;
		//int wp, hp;

		switch(mLayout)
		{
		case FBL_COORDS:
			break;

		case FBL_SNAP_LEFT:
		case FBL_SNAP_RIGHT:
			w = 0;
			h = 1;
			break;

		case FBL_SNAP_TOP:
		case FBL_SNAP_BOTTOM:
			w = 1;
			h = 0;
			break;
		}

		int n = (int)mItemsOrder.size();
		int dw = 20, dh = 20;

		for (int i = 0; i < n; i++)
		{
			IntSize sz = mItemsOrder[i]->getSize();

			IntPoint pt;
			pt.left = (dw + 5) * w;
			pt.top = (dh + 5) * h;

			dw += (sz.width + 5) * w;
			dh += (sz.height + 5) * h;

			mItemsOrder[i]->setPosition(pt);
		}
	}

	void FooBar::addItem(const Ogre::String &name, const Ogre::String &texture)
	{
		StaticImagePtr item = createWidget<StaticImage>("FooBarItem", IntCoord(0, 0, mWidth, mWidth), ALIGN_DEFAULT);

		if (false == Ogre::TextureManager::getSingleton().resourceExists(texture)) 
			Ogre::TextureManager::getSingleton().load(texture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		item->setImageTexture(texture);

		_addChildItem(name, item);

		updateItemsLayout();
	}

	void FooBar::removeItem(const Ogre::String &name)
	{

	}

	void FooBar::_addChildItem(const Ogre::String &name, MyGUI::WidgetPtr widget)
	{
		if (_isChildItem(name))
			return;

		mItems[name] = widget;
		mItemsOrder.push_back(widget);
	}

	void FooBar::_removeChildItem(const Ogre::String &name)
	{
		WidgetMap::iterator it = mItems.find(name);

		if (it != mItems.end())
		{
			int n = (int)mItemsOrder.size();
			for (int i = 0; i < n; i++)
				if (mItemsOrder[i] == it->second)
				{
					mItemsOrder.erase(mItemsOrder.begin() + i);
					break;
				}

			mItems.erase(it);
		}
	}

	bool FooBar::_isChildItem(const Ogre::String &name)
	{
		WidgetMap::iterator it = mItems.find(name);

		if (it != mItems.end())
			return true;

		return false;
	}

	void FooBar::_removeAllChildItems()
	{
		while (!mItems.empty())
		{
			_destroyChildWidget(mItems.begin()->second);
			mItems.erase(mItems.begin());
		}

		mItemsOrder.clear();
	}

}
