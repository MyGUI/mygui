/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourceLayout.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	ResourceLayout::ResourceLayout(xml::ElementPtr _node, std::string_view _fileName)
	{
		// FIXME hardcoded version
		deserialization(_node, Version(1, 0, 0));
		mResourceName = _fileName;
	}

	void ResourceLayout::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);

		mLayoutData.clear();

		xml::ElementEnumerator widget = _node->getElementEnumerator();
		while (widget.next("Widget"))
			mLayoutData.push_back(parseWidget(widget));
	}

	WidgetInfo ResourceLayout::parseWidget(xml::ElementEnumerator& _widget)
	{
		WidgetInfo widgetInfo;

		std::string tmp;

		_widget->findAttribute("type", widgetInfo.type);
		_widget->findAttribute("skin", widgetInfo.skin);
		_widget->findAttribute("layer", widgetInfo.layer);

		if (_widget->findAttribute("align", tmp))
			widgetInfo.align = Align::parse(tmp);

		_widget->findAttribute("name", widgetInfo.name);

		if (_widget->findAttribute("style", tmp))
			widgetInfo.style = WidgetStyle::parse(tmp);

		if (_widget->findAttribute("position", tmp))
		{
			widgetInfo.intCoord = IntCoord::parse(tmp);
			widgetInfo.positionType = WidgetInfo::Pixels;
		}
		else if (_widget->findAttribute("position_real", tmp))
		{
			widgetInfo.floatCoord = FloatCoord::parse(tmp);
			widgetInfo.positionType = WidgetInfo::Relative;
		}

		// берем детей и крутимся
		xml::ElementEnumerator node = _widget->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Widget")
			{
				widgetInfo.childWidgetsInfo.push_back(parseWidget(node));
			}
			else if (node->getName() == "Property")
			{
				widgetInfo.properties.emplace_back(node->findAttribute("key"), node->findAttribute("value"));
			}
			else if (node->getName() == "UserString")
			{
				std::string_view key = node->findAttribute("key");
				std::string_view value = node->findAttribute("value");
				mapSet(widgetInfo.userStrings, key, value);
			}
			else if (node->getName() == "Controller")
			{
				ControllerInfo controllerInfo;
				controllerInfo.type = node->findAttribute("type");

				xml::ElementEnumerator prop = node->getElementEnumerator();
				while (prop.next("Property"))
				{
					std::string_view key = prop->findAttribute("key");
					std::string_view value = prop->findAttribute("value");
					mapSet(controllerInfo.properties, key, value);
				}

				widgetInfo.controllers.push_back(controllerInfo);
			}
		}

		return widgetInfo;
	}

	VectorWidgetPtr ResourceLayout::createLayout(std::string_view _prefix, Widget* _parent)
	{
		VectorWidgetPtr widgets;

		for (auto& iter : mLayoutData)
		{
			Widget* widget = createWidget(iter, _prefix, _parent);
			widgets.push_back(widget);
		}

		return widgets;
	}

	Widget* ResourceLayout::createWidget(
		const WidgetInfo& _widgetInfo,
		std::string_view _prefix,
		Widget* _parent,
		bool _template)
	{
		std::string widgetName;
		WidgetStyle style = _widgetInfo.style;
		std::string_view widgetLayer = _widgetInfo.layer;

		if (!_widgetInfo.name.empty())
		{
			widgetName = _prefix;
			widgetName += _widgetInfo.name;
		}

		if (_parent != nullptr && style != WidgetStyle::Popup)
			widgetLayer = {};
		if (_parent == nullptr && widgetLayer.empty())
		{
			MYGUI_LOG(
				Warning,
				"Root widget's layer is not specified, widget won't be visible. Specify layer or parent or attach it "
				"to another widget after load."
					<< " [" << LayoutManager::getInstance().getCurrentLayout() << "]");
		}

		IntCoord coord;
		if (_widgetInfo.positionType == WidgetInfo::Pixels)
			coord = _widgetInfo.intCoord;
		else if (_widgetInfo.positionType == WidgetInfo::Relative)
		{
			if (_parent == nullptr || style == WidgetStyle::Popup)
				coord = CoordConverter::convertFromRelative(
					_widgetInfo.floatCoord,
					RenderManager::getInstance().getViewSize());
			else
				coord = CoordConverter::convertFromRelative(_widgetInfo.floatCoord, _parent->getClientCoord().size());
		}

		Widget* wid;
		if (nullptr == _parent)
			wid = Gui::getInstance().createWidgetT(
				_widgetInfo.type,
				_widgetInfo.skin,
				coord,
				_widgetInfo.align,
				widgetLayer,
				widgetName);
		else if (_template)
			wid = _parent->_createSkinWidget(
				style,
				_widgetInfo.type,
				_widgetInfo.skin,
				coord,
				_widgetInfo.align,
				widgetLayer,
				widgetName);
		else
			wid = _parent->createWidgetT(
				style,
				_widgetInfo.type,
				_widgetInfo.skin,
				coord,
				_widgetInfo.align,
				widgetLayer,
				widgetName);

		for (const auto& property : _widgetInfo.properties)
		{
			wid->setProperty(property.first, property.second);
		}

		for (const auto& userString : _widgetInfo.userStrings)
		{
			wid->setUserString(userString.first, userString.second);
			if (!_template)
				LayoutManager::getInstance().eventAddUserString(wid, userString.first, userString.second);
		}

		for (const auto& iter : _widgetInfo.childWidgetsInfo)
		{
			createWidget(iter, _prefix, wid);
		}

		for (const auto& iter : _widgetInfo.controllers)
		{
			MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance().createItem(iter.type);
			if (item)
			{
				for (const auto& property : iter.properties)
				{
					item->setProperty(property.first, property.second);
				}
				MyGUI::ControllerManager::getInstance().addItem(wid, item);
			}
			else
			{
				MYGUI_LOG(Warning, "Controller '" << iter.type << "' not found");
			}
		}

		LayoutManager::getInstance().eventCreateWidget(wid, _widgetInfo);

		return wid;
	}

	const VectorWidgetInfo& ResourceLayout::getLayoutData() const
	{
		return mLayoutData;
	}

} // namespace MyGUI
