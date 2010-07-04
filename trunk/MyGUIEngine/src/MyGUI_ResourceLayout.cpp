/*!
	@file
	@author		George Evmenov
	@date		06/2010
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
#include "MyGUI_ResourceLayout.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	ResourceLayout::ResourceLayout(xml::ElementPtr _node, const std::string& _fileName) :
		mXmlNode(_node->createCopy())
	{
		mResourceName = _fileName;
	}

	void ResourceLayout::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);
	}

	VectorWidgetPtr& ResourceLayout::create(const std::string& _prefix, Widget* _parent)
	{
		static VectorWidgetPtr widgets;
		widgets.clear();
		parseLayout(&widgets, mXmlNode, _prefix, _parent);
		return widgets;
	}

	void ResourceLayout::parseLayout(VectorWidgetPtr* _widgets, xml::ElementPtr _root, const std::string& _prefix, Widget* _parent)
	{
#if MYGUI_DEBUG_MODE == 1
		MYGUI_LOG(Info, "Create layout '" << mResourceName << "'");
#endif
		// берем детей и крутимся
		xml::ElementEnumerator widget = _root->getElementEnumerator();
		while (widget.next("Widget")) parseWidget(_widgets, widget, _parent, _prefix);
	}

	void ResourceLayout::parseWidget(VectorWidgetPtr* _widgets, xml::ElementEnumerator& _widget, Widget* _parent, const std::string& _prefix)
	{
		// парсим атрибуты виджета
		std::string widgetType, widgetSkin, widgetName, widgetLayer, tmp;

		_widget->findAttribute("type", widgetType);
		_widget->findAttribute("skin", widgetSkin);
		_widget->findAttribute("layer", widgetLayer);

		Align align = Align::Default;
		if (_widget->findAttribute("align", tmp)) align = Align::parse(tmp);

		_widget->findAttribute("name", widgetName);
		if (!widgetName.empty()) widgetName = _prefix + widgetName;

		WidgetStyle style = WidgetStyle::Child;
		if (_widget->findAttribute("style", tmp)) style = WidgetStyle::parse(tmp);
		if (_parent != nullptr && style != WidgetStyle::Popup) widgetLayer.clear();

		IntCoord coord;
		if (_widget->findAttribute("position", tmp)) coord = IntCoord::parse(tmp);
		else if (_widget->findAttribute("position_real", tmp))
		{
			if (_parent == nullptr || style == WidgetStyle::Popup)
				coord = CoordConverter::convertFromRelative(FloatCoord::parse(tmp), RenderManager::getInstance().getViewSize());
			else
				coord = CoordConverter::convertFromRelative(FloatCoord::parse(tmp), _parent->getClientCoord().size());
		}

		Widget* wid;
		if (nullptr == _parent)
			wid = Gui::getInstance().createWidgetT(widgetType, widgetSkin, coord, align, widgetLayer, widgetName);
		else
			wid = _parent->createWidgetT(style, widgetType, widgetSkin, coord, align, widgetLayer, widgetName);

		if (_widgets != nullptr) _widgets->push_back(wid);

		// берем детей и крутимся
		xml::ElementEnumerator node = _widget->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Widget")
			{
				parseWidget(nullptr, node, wid, _prefix);
			}
			else if (node->getName() == "Property")
			{
				wid->setProperty(node->findAttribute("key"), node->findAttribute("value"));
			}
			else if (node->getName() == "UserString")
			{
				wid->setUserString(node->findAttribute("key"), node->findAttribute("value"));
			}
			else if (node->getName() == "Controller")
			{
				const std::string& type = node->findAttribute("type");
				MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance().createItem(type);
				if (item)
				{
					xml::ElementEnumerator prop = node->getElementEnumerator();
					while (prop.next("Property"))
					{
						item->setProperty(prop->findAttribute("key"), prop->findAttribute("value"));
					}
					MyGUI::ControllerManager::getInstance().addItem(wid, item);
				}
				else
				{
					MYGUI_LOG(Warning, "Controller '" << type << "' not found");
				}
			}

		}
	}

} // namespace MyGUI
