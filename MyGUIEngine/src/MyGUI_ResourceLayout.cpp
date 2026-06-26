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

	ResourceLayout::ResourceLayout(pugi::xml_node _node, std::string_view _fileName)
	{
		// FIXME hardcoded version
		deserialize(_node, Version(1, 0, 0));
		mResourceName = _fileName;
	}

	void ResourceLayout::deserialize(pugi::xml_node _node, Version _version)
	{
		Base::deserialize(_node, _version);

		mLayoutData.clear();

		for (auto widget : _node.children("Widget"))
			mLayoutData.push_back(parseWidget(widget));
	}

	WidgetInfo ResourceLayout::parseWidget(pugi::xml_node _widget)
	{
		WidgetInfo widgetInfo;

		widgetInfo.type = _widget.attribute("type").value();
		widgetInfo.skin = _widget.attribute("skin").value();
		widgetInfo.layer = _widget.attribute("layer").value();

		std::string tmp{_widget.attribute("align").value()};
		if (!tmp.empty())
			widgetInfo.align = Align::parse(tmp);

		widgetInfo.name = _widget.attribute("name").value();

		tmp = _widget.attribute("style").value();
		if (!tmp.empty())
			widgetInfo.style = WidgetStyle::parse(tmp);

		tmp = _widget.attribute("position").value();
		if (!tmp.empty())
		{
			widgetInfo.intCoord = IntCoord::parse(tmp);
			widgetInfo.positionType = WidgetInfo::Pixels;
		}
		else
		{
			tmp = _widget.attribute("position_real").value();
			if (!tmp.empty())
			{
				widgetInfo.floatCoord = FloatCoord::parse(tmp);
				widgetInfo.positionType = WidgetInfo::Relative;
			}
		}

		for (auto node : _widget)
		{
			if (node.name() == std::string_view("Widget"))
			{
				widgetInfo.childWidgetsInfo.push_back(parseWidget(node));
			}
			else if (node.name() == std::string_view("Property"))
			{
				widgetInfo.properties.emplace_back(node.attribute("key").value(), node.attribute("value").value());
			}
			else if (node.name() == std::string_view("UserString"))
			{
				std::string_view key = node.attribute("key").value();
				std::string_view value = node.attribute("value").value();
				mapSet(widgetInfo.userStrings, key, value);
			}
			else if (node.name() == std::string_view("Controller"))
			{
				ControllerInfo controllerInfo;
				controllerInfo.type = node.attribute("type").value();

				for (auto prop : node.children("Property"))
				{
					std::string_view key = prop.attribute("key").value();
					std::string_view value = prop.attribute("value").value();
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
