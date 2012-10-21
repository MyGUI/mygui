/*!
	@file
	@author		Albert Semenov
	@date		03/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_ToolTipManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	template <> ToolTipManager* Singleton<ToolTipManager>::msInstance = nullptr;
	template <> const char* Singleton<ToolTipManager>::mClassTypeName = "ToolTipManager";

	ToolTipManager::ToolTipManager() :
		mDelayVisible(0.5f),
		mOldFocusWidget(nullptr),
		mToolTipVisible(false),
		mCurrentTime(0),
		mOldIndex(ITEM_NONE),
		mNeedToolTip(false),
		mIsInitialise(false)
	{
	}

	void ToolTipManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		mDelayVisible = 0.5f;
		mOldFocusWidget = nullptr;
		mToolTipVisible = false;
		mCurrentTime = 0;
		mOldIndex = ITEM_NONE;
		mNeedToolTip = false;

		Gui::getInstance().eventFrameStart += newDelegate(this, &ToolTipManager::notifyEventFrameStart);
		WidgetManager::getInstance().registerUnlinker(this);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void ToolTipManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		WidgetManager::getInstance().unregisterUnlinker(this);
		Gui::getInstance().eventFrameStart -= newDelegate(this, &ToolTipManager::notifyEventFrameStart);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void ToolTipManager::notifyEventFrameStart(float _time)
	{
		Widget* widget = InputManager::getInstance().getMouseFocusWidget();
		if (mOldFocusWidget != widget)
		{
			if (mToolTipVisible)
			{
				mToolTipVisible = false;
				hideToolTip(mOldFocusWidget);
			}
			mOldFocusWidget = widget;
			mNeedToolTip = false;

			if (mOldFocusWidget != nullptr)
			{
				mCurrentTime = 0;
				mOldMousePoint = InputManager::getInstance().getMousePositionByLayer();
				mOldIndex = getToolTipIndex(mOldFocusWidget);
				mNeedToolTip = isNeedToolTip(mOldFocusWidget);
			}
		}
		else if (mNeedToolTip)
		{
			bool capture = InputManager::getInstance().isCaptureMouse();
			if (capture)
			{
				if (mToolTipVisible)
				{
					mToolTipVisible = false;
					hideToolTip(mOldFocusWidget);
				}
			}
			else
			{
				IntPoint point = InputManager::getInstance().getMousePositionByLayer();
				if (!mToolTipVisible && point != mOldMousePoint)
				{
					if (mToolTipVisible)
					{
						mToolTipVisible = false;
						hideToolTip(mOldFocusWidget);
					}
					mCurrentTime = 0;
					mOldMousePoint = point;
					mOldIndex = getToolTipIndex(mOldFocusWidget);
				}
				else
				{
					size_t index = getToolTipIndex(mOldFocusWidget);
					if (mOldIndex != index)
					{
						if (mToolTipVisible)
						{
							mToolTipVisible = false;
							hideToolTip(mOldFocusWidget);
						}
						mCurrentTime = 0;
						mOldIndex = index;
					}
					else
					{
						if (!mToolTipVisible)
						{
							mCurrentTime += _time;
							if (mCurrentTime >= mDelayVisible)
							{
								mToolTipVisible = true;
								showToolTip(mOldFocusWidget, mOldIndex, point);
							}
						}
						else if (point != mOldMousePoint)
						{
							moveToolTip(mOldFocusWidget, mOldIndex, point);
						}
					}
				}
			}
		}
	}

	void ToolTipManager::_unlinkWidget(Widget* _widget)
	{
		if (mOldFocusWidget == _widget)
		{
			if (mToolTipVisible)
			{
				mToolTipVisible = false;
				hideToolTip(mOldFocusWidget);
			}
			mOldFocusWidget = nullptr;
			mNeedToolTip = false;
		}
	}

	void ToolTipManager::hideToolTip(Widget* _widget)
	{
		Widget* container = _widget->_getContainer();
		if (container != nullptr)
			container->eventToolTip(container, ToolTipInfo(ToolTipInfo::Hide));
		else
			_widget->eventToolTip(_widget, ToolTipInfo(ToolTipInfo::Hide));
	}

	void ToolTipManager::showToolTip(Widget* _widget, size_t _index, const IntPoint& _point)
	{
		Widget* container = _widget->_getContainer();
		if (container != nullptr)
			container->eventToolTip(container, ToolTipInfo(ToolTipInfo::Show, _index, _point));
		else
			_widget->eventToolTip(_widget, ToolTipInfo(ToolTipInfo::Show, ITEM_NONE, _point));
	}

	void ToolTipManager::moveToolTip(Widget* _widget, size_t _index, const IntPoint& _point)
	{
		Widget* container = _widget->_getContainer();
		if (container != nullptr)
			container->eventToolTip(container, ToolTipInfo(ToolTipInfo::Move, _index, _point));
		else
			_widget->eventToolTip(_widget, ToolTipInfo(ToolTipInfo::Move, ITEM_NONE, _point));
	}

	bool ToolTipManager::isNeedToolTip(Widget* _widget)
	{
		Widget* container = _widget->_getContainer();
		if (container != nullptr)
			return container->getNeedToolTip();
		return _widget->getNeedToolTip();
	}

	size_t ToolTipManager::getToolTipIndex(Widget* _widget) const
	{
		Widget* container = _widget->_getContainer();
		if (container != nullptr)
			return container->_getItemIndex(_widget);
		return ITEM_NONE;
	}

	void ToolTipManager::setDelayVisible(float _value)
	{
		mDelayVisible = _value;
	}

	float ToolTipManager::getDelayVisible() const
	{
		return mDelayVisible;
	}

} // namespace MyGUI
