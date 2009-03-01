/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
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
#include "MyGUI_Common.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"


namespace MyGUI
{

	const std::string XML_TYPE("Layout");

	MYGUI_INSTANCE_IMPLEMENT(LayoutManager);

	void LayoutManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LayoutManager::_load);
		layoutPrefix = "";
		layoutParent = nullptr;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LayoutManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	VectorWidgetPtr LayoutManager::load(const std::string & _file, const std::string & _group)
	{
		mVectorWidgetPtr.clear();
		ResourceManager::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
		return mVectorWidgetPtr;
	}

	void LayoutManager::_load(xml::ElementPtr _node, const std::string & _file, Version _version)
	{
#if MYGUI_DEBUG_MODE == 1
		MYGUI_LOG(Info, "load layout '" << _file << "'");
#endif
		parseLayout(mVectorWidgetPtr, _node);
	}

	VectorWidgetPtr LayoutManager::loadLayout(const std::string & _file, const std::string & _prefix, WidgetPtr _parent, const std::string & _group)
	{
		layoutPrefix = _prefix;
		layoutParent = _parent;
		VectorWidgetPtr widgets = load(_file, _group);
		layoutPrefix = "";
		layoutParent = nullptr;
		return widgets;
	}

	void LayoutManager::unloadLayout(VectorWidgetPtr & _widgets)
	{
		WidgetManager::getInstance().destroyWidgets(_widgets);
	}

	void LayoutManager::parseLayout(VectorWidgetPtr & _widgets, xml::ElementPtr _root)
	{
		// берем детей и крутимся
		xml::ElementEnumerator widget = _root->getElementEnumerator();
		while (widget.next("Widget")) parseWidget(_widgets, widget, layoutParent);
	}

	void LayoutManager::parseWidget(VectorWidgetPtr & _widgets, xml::ElementEnumerator & _widget, WidgetPtr _parent)
	{
		// парсим атрибуты виджета
		std::string widgetType, widgetSkin, widgetName, widgetLayer, tmp;
		IntCoord coord;
		Align align = Align::Default;

		_widget->findAttribute("type", widgetType);
		_widget->findAttribute("skin", widgetSkin);
		_widget->findAttribute("name", widgetName);
		_widget->findAttribute("layer", widgetLayer);
		if (_widget->findAttribute("align", tmp)) align = Align::parse(tmp);
		if (_widget->findAttribute("position", tmp)) coord = IntCoord::parse(tmp);
		//FIXME парент может быть и не кроппед
		if (_widget->findAttribute("position_real", tmp)) coord = WidgetManager::getInstance().convertRelativeToInt(FloatCoord::parse(tmp), _parent);

		if (!widgetName.empty()) widgetName = layoutPrefix + widgetName;

		WidgetPtr wid;
		if (nullptr == _parent)
		{
			wid = Gui::getInstance().createWidgetT(widgetType, widgetSkin, coord, align, widgetLayer, widgetName);
		}
		else
		{
			WidgetStyle style = WidgetStyle::Child;
			if (_widget->findAttribute("style", tmp))
			{
				style = WidgetStyle::parse(tmp);
			}
			if (style != WidgetStyle::Popup && !widgetLayer.empty()) widgetLayer.clear();
			wid = _parent->createWidgetT(style, widgetType, widgetSkin, coord, align, widgetLayer, widgetName);
		}

		if (layoutParent == _parent) _widgets.push_back(wid);

		// берем детей и крутимся
		xml::ElementEnumerator widget = _widget->getElementEnumerator();
		while (widget.next())
		{

			std::string key, value;

			if (widget->getName() == "Widget")
			{
				parseWidget(_widgets, widget, wid);
			}
			else if (widget->getName() == "Property")
			{
				// парсим атрибуты
				if (false == widget->findAttribute("key", key)) continue;
				if (false == widget->findAttribute("value", value)) continue;
				// и парсим свойство
				WidgetManager::getInstance().parse(wid, key, value);
			}
			else if (widget->getName() == "UserString")
			{
				// парсим атрибуты
				if (false == widget->findAttribute("key", key)) continue;
				if (false == widget->findAttribute("value", value)) continue;
				wid->setUserString(key, value);
			}

		}
	}

} // namespace MyGUI
