/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Common.h"
#include "MyGUI_Gui.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"


namespace MyGUI
{

	const std::string XML_TYPE("Layout");

	INSTANCE_IMPLEMENT(LayoutManager);

	void LayoutManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LayoutManager::_load);
		layoutPrefix = "";

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LayoutManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	VectorWidgetPtr LayoutManager::load(const std::string & _file, const std::string & _group)
	{
		mVectorWidgetPtr.clear();
		Gui::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
		return mVectorWidgetPtr;
	}

	void LayoutManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		parseLayout(mVectorWidgetPtr, _node);
	}

	VectorWidgetPtr LayoutManager::loadLayout(const std::string & _file, const std::string & _prefix, const std::string & _group)
	{
		layoutPrefix = _prefix;
		VectorWidgetPtr widgets = load(_file, _group);
		layoutPrefix = "";
		return widgets;
	}

	void LayoutManager::parseLayout(VectorWidgetPtr & _widgets, xml::xmlNodePtr _root)
	{
		// берем детей и крутимся
		xml::xmlNodeIterator widget = _root->getNodeIterator();
		while (widget.nextNode("Widget")) parseWidget(_widgets, widget, 0);
	}

	void LayoutManager::parseWidget(VectorWidgetPtr & _widgets, xml::xmlNodeIterator & _widget, WidgetPtr _parent)
	{
		// парсим атрибуты виджета
		Ogre::String widgetType, widgetSkin, widgetName, widgetLayer, tmp;
		IntCoord coord;
		Align align = ALIGN_DEFAULT;

		_widget->findAttribute("type", widgetType);
		_widget->findAttribute("skin", widgetSkin);
		_widget->findAttribute("name", widgetName);
		_widget->findAttribute("layer", widgetLayer);
		if (_widget->findAttribute("align", tmp)) align = SkinManager::getInstance().parseAlign(tmp);
		if (_widget->findAttribute("position", tmp)) coord = IntCoord::parse(tmp);
		if (_widget->findAttribute("position_real", tmp)) coord = Gui::getInstance().convertRelativeToInt(FloatCoord::parse(tmp), _parent);

		if (!widgetName.empty()) widgetName = layoutPrefix + widgetName;

		WidgetPtr wid;
		if (null == _parent) {
			wid = Gui::getInstance().createWidgetT(widgetType, widgetSkin, coord, align, widgetLayer, widgetName);
			_widgets.push_back(wid);
		}
		else wid = _parent->createWidgetT(widgetType, widgetSkin, coord, align, widgetName);

		// берем детей и крутимся
		xml::xmlNodeIterator widget = _widget->getNodeIterator();
		while (widget.nextNode()) {

			std::string key, value;

			if (widget->getName() == "Widget") parseWidget(_widgets, widget, wid);
			else if (widget->getName() == "Property") {

				// парсим атрибуты
				if (false == widget->findAttribute("key", key)) continue;
				if (false == widget->findAttribute("value", value)) continue;
				// и парсим свойство
				WidgetManager::getInstance().parse(wid, key, value);
			}
			else if (widget->getName() == "UserString") {
				// парсим атрибуты
				if (false == widget->findAttribute("key", key)) continue;
				if (false == widget->findAttribute("value", value)) continue;
				wid->setUserString(key, value);
			}

		};
	}

} // namespace MyGUI
