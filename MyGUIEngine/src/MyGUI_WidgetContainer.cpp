/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_WidgetContainer.h"
#include "MyGUI_Widget.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_RenderManager.h"

namespace MyGUI
{

	void WidgetContainer::_linkChildWidget(Widget* _widget)
	{
		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter == mWidgetChild.end(), "widget already exist");
		mWidgetChild.push_back(_widget);
	}

	void WidgetContainer::_unlinkChildWidget(Widget* _widget)
	{
		VectorWidgetPtr::iterator iter = std::remove(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter != mWidgetChild.end(), "widget not found");
		mWidgetChild.erase(iter);
	}

	void WidgetContainer::_destroyChildWidget(Widget* _widget)
	{
		MYGUI_ASSERT(nullptr != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end())
		{
			MyGUI::Widget* widget = *iter;
			mWidgetChild.erase(iter);
			WidgetManager::getInstance().unlinkFromUnlinkers(_widget);
			WidgetManager::getInstance()._deleteWidget(widget);
		}
		else
		{
			MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
		}
	}

	void WidgetContainer::_destroyAllChildWidget()
	{
		while (!mWidgetChild.empty())
		{
			Widget* widget = mWidgetChild.back();
			mWidgetChild.pop_back();
			WidgetManager::getInstance().unlinkFromUnlinkers(widget);
			WidgetManager::getInstance()._deleteWidget(widget);
		}
	}

	Widget* WidgetContainer::baseCreateWidget(
		WidgetStyle _style,
		std::string_view _type,
		std::string_view _skin,
		const IntCoord& _coord,
		Align _align,
		std::string_view _layer,
		std::string_view _name)
	{
		Widget* widget =
			WidgetManager::getInstance().createWidget(_style, _type, _skin, _coord, nullptr, nullptr, _name);
		mWidgetChild.push_back(widget);

		widget->setAlign(_align);

		if (!_layer.empty())
			LayerManager::getInstance().attachToLayerNode(_layer, widget);
		return widget;
	}

	const VectorWidgetPtr& WidgetContainer::getChildWidgets() const
	{
		return mWidgetChild;
	}

	size_t WidgetContainer::getChildCount() const
	{
		return mWidgetChild.size();
	}

	Widget* WidgetContainer::getChildAt(size_t _index) const
	{
		MYGUI_ASSERT_RANGE(_index, mWidgetChild.size(), "WidgetContainer::getChildAt");
		return mWidgetChild[_index];
	}

	EnumeratorWidgetPtr WidgetContainer::getEnumerator() const
	{
		return EnumeratorWidgetPtr(mWidgetChild);
	}

} // namespace MyGUI
